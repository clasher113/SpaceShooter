#include "EntitySystem.hpp"
#include "../Assets.hpp"
#include "../graphics/Layers.hpp"
#include "../graphics/Layer.hpp"
#include "../system/Window.hpp"
#include "IEntity.hpp"
#include "SpaceShip.hpp"
#include "Bullet.hpp"
#include "Effect.hpp"
#include "Player.hpp"
#include "Upgrade.hpp"
#include "components/Components.hpp"
#include "../utils/Log.hpp"
#include "../utils/Random.hpp"
#ifdef _DEBUG
#include "../utils/DebugInfo.hpp"
#endif // _DEBUG

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

using Random = effolkronium::random_static;
float EntitySpawner::s_spawnSpeed = 1.f;
float EntitySpawner::s_maxSpawnSpeed = 3.f;
float EntitySpawner::s_increasDifficultyTimer = 0.f;
float EntitySpawner::s_increasDifficultyInterval = 15.f;
float EntitySpawner::s_increasDifficultyFactor = 0.1f;

EntitySystem::EntitySystem(Assets* assets) :
	m_p_assets(assets),
	m_cleaningIsRequired(false),
	m_windowBounds(sf::Vector2f(0.f, 0.f), sf::Vector2f(Window::getSize()))
{
	EntitySpawner::s_spawnSpeed = 1.f;
	for (size_t i = 0; i < EntityInfo::getSize(); i++) {
		const auto& entity = EntityInfo::getEntity(i);
		if ((typeid(SpaceShipInfo) == typeid(*entity->second) || typeid(UpgradeInfo) == typeid(*entity->second)) && 
			 entity->second->type != EntityType::PLAYER) {
			m_entitySpawner.emplace_back(EntitySpawner(m_p_assets, entity->first, this));
		}
	}
}

EntitySystem::~EntitySystem() {
	while (!m_entities.empty()) {
		delete m_entities.front();
		m_entities.erase(m_entities.begin());
	}
	m_entitySpawner.clear();
}

void EntitySystem::input() {
	for (const auto& it : m_entities) {
		it->input();
	}
}

void EntitySystem::update(Player* player, const float dt) {
	for (auto& it : m_entitySpawner) {
		it.update(dt);
	}
	if (!m_addBuffer.empty()) {
		m_entities.reserve(m_entities.size() + m_addBuffer.size());
		m_entities.insert(m_entities.end(), std::make_move_iterator(m_addBuffer.begin()), std::make_move_iterator(m_addBuffer.end()));
		m_addBuffer.clear();
	}
	for (const auto& it : m_entities) {
		// collisions
		it->update(dt);
		if (it->getType() == EntityType::UNDEFINED || it == player) continue;
		// enemies with player bullets
		for (const auto& playerBullet : m_entities) {
			if (typeid(*it) != typeid(SpaceShip)) break;
			if (it == playerBullet || typeid(*playerBullet) != typeid(Bullet) || playerBullet->getType() != EntityType::PLAYER) continue;
			if (!it->isDead() && !playerBullet->isDead() && playerBullet->getHitbox().intersects(it->getHitbox())) {
				it->giveDamage(playerBullet->getDamage());
				playerBullet->kill();
				if (it->isDead()) player->onEnemyKilled(static_cast<SpaceShipInfo*>(it->getInfo()));
			}
		}
		if (player->isDead()) continue;
		// player with enemy & enemy bullets
		if (!it->isDead() && (it->getType() == EntityType::ENEMY || it->getType() == EntityType::UPGRADE) && 
			player->getHitbox().intersects(it->getHitbox())) {
			if (it->getType() == EntityType::ENEMY) player->giveDamage(it->getDamage());
			it->kill();
		}
		// any entity with window
		sf::FloatRect entityBounds = it->getGlobalBounds();
		sf::FloatRect windowBounds = sf::FloatRect(m_windowBounds.left - entityBounds.width, m_windowBounds.top - entityBounds.height,
			m_windowBounds.width + entityBounds.width * 2.f, m_windowBounds.height + entityBounds.height * 2.f);
		if (!it->isDead() && !windowBounds.contains(it->getPosition())) {
			if (typeid(*it) == typeid(SpaceShip)) player->onEnemyLost(static_cast<SpaceShipInfo*>(it->getInfo()));
			remove(it);
			continue;
		}
	}
	if (m_cleaningIsRequired) {
		auto result = remove_if(m_entities.begin(), m_entities.end(), [player](const IEntity* e) {
			if (e->isDead() && e != player) {
				delete e;
				return true;
			}
			return false;
		});

		m_entities.erase(result, m_entities.end());
		m_cleaningIsRequired = false;
	};
	EntitySpawner::increasDifficulty(dt);
#ifdef _DEBUG
	DebugInfo::entities = m_entities.size();
#endif // _DEBUG

}

void EntitySystem::draw(sf::RenderWindow& window) {
	Layers layers;
	for (const auto& it : m_entities) {
		if (!it->isDead()) it->draw(layers);
	}
	layers.draw(window);
}

template<typename Type>
Type* EntitySystem::spawn(const std::string& name, float x, float y) {
	if (EntityInfo::getEntity(name) == nullptr) {
		ERR("Failed to spawn entity \"" << name << "\"!");
		return nullptr;
	}
	return static_cast<Type*>(*m_addBuffer.emplace(m_addBuffer.begin(), new Type(m_p_assets, name, x, y, this)));
}

template SpaceShip* EntitySystem::spawn<SpaceShip>(const std::string& name, float x, float y);
template Player* EntitySystem::spawn<Player>(const std::string& name, float x, float y);
template Bullet* EntitySystem::spawn<Bullet>(const std::string& name, float x, float y);
template Effect* EntitySystem::spawn<Effect>(const std::string& name, float x, float y);
template Upgrade* EntitySystem::spawn<Upgrade>(const std::string& name, float x, float y);

IEntity* EntitySystem::getClosestEntity(sf::Vector2f position, EntityType type, const std::string& name) const {
	IEntity* closest = nullptr;
	long closestDistance = LONG_MAX;
	for (const auto& it : m_entities) {
		if (!(it->getType() == type || type == EntityType::UNDEFINED) || !(typeid(*it) == typeid(SpaceShip) || 
			typeid(*it) == typeid(Player)) || it->isDead()) continue;
		if (!name.empty() && it->getName() != name) continue;
		const sf::Vector2f& target = it->getPosition();
		double distance = sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y));
		if (closestDistance > distance) {
			closestDistance = static_cast<long>(distance);
			closest = it;
		}
	}
	return closest;
}

IEntity* EntitySystem::getEntity(size_t index) const {
	if (index > m_entities.size()) return nullptr;
	else return m_entities[index];
}

void EntitySystem::remove(size_t index) {
	if (index > m_entities.size()) return;
	remove(m_entities[index]);
}

void EntitySystem::remove(IEntity* entity) {
	if (entity == nullptr) return;
	entity->setIsDead(true);
	m_cleaningIsRequired = true;
}

EntitySpawner::EntitySpawner(Assets* assets, const std::string& name, EntitySystem* entitySystem) :
	m_p_assets(assets),
	m_entityName(name),
	m_p_entitySystem(entitySystem)
{
	SpaceShipInfo* entity = static_cast<SpaceShipInfo*>(EntityInfo::getEntity(m_entityName));
	m_entityType = entity->type;
	m_spawnInterval = entity->spawnInterval;
	m_spawnTimer = m_spawnInterval / 2.f;
	sf::Vector2u size = m_p_assets->getTexture(entity->textureName).getSize();
	int columns = 1;
	int rows = 1;
	if (entity->animationData != nullptr) {
		columns = entity->animationData->m_columns;
		rows = entity->animationData->m_rows;
	}
	m_offset = sf::Vector2f(size.x / columns / 2.f * ENTITIES_SCALE * Window::getAspectRatio(),
							size.y / rows / 2.f * ENTITIES_SCALE * Window::getAspectRatio());
};

void EntitySpawner::update(const float dt) {
	m_spawnTimer += s_spawnSpeed * dt;
	if (m_spawnTimer >= m_spawnInterval) {
		m_spawnTimer -= m_spawnInterval;
		switch (m_entityType) {
		case EntityType::UPGRADE:
			m_p_entitySystem->spawn<Upgrade>(m_entityName, Random::get<float>(m_offset.x, Window::getSize().x - m_offset.x), -m_offset.y);
			break;
		default:
			m_p_entitySystem->spawn<SpaceShip>(m_entityName, Random::get<float>(m_offset.x, Window::getSize().x - m_offset.x), -m_offset.y);
			break;
		}
	}
}

void EntitySpawner::increasDifficulty(const float dt) {
	if (s_spawnSpeed >= s_maxSpawnSpeed) return;
	s_increasDifficultyTimer += dt;
	if (s_increasDifficultyTimer >= s_increasDifficultyInterval) {
		s_increasDifficultyTimer -= s_increasDifficultyInterval;
		s_spawnSpeed += s_increasDifficultyFactor;
	}
}
