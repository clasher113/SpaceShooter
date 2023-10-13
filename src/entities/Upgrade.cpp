#include "Upgrade.hpp"
#include "Player.hpp"
#include "EntitySystem.hpp"
#include "components/EffectComponent.hpp"
#include "EntityInfo.hpp"
#include "EntitySystem.hpp"
#include "../utils/Random.hpp"

#include "../utils/Log.hpp"

std::unordered_map<std::string, std::function<void(const std::string& args, Upgrade* upgrade)>> Upgrade::s_m_commands;
typedef std::vector<std::string> Tokens;
using Random = effolkronium::random_static;

Upgrade::Upgrade(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem) : Entity(assets, name, x, y, entitySystem),
	m_p_upgradeInfo(static_cast<UpgradeInfo*>(m_p_entityInfo))
{
}

Upgrade::~Upgrade() {
}

void Upgrade::giveDamage(int damage) {
	EffectComponent* effect = static_cast<EffectComponent*>(getComponent<EffectComponent>());
	if (effect != nullptr) {
		effect->onDeathEffect();
		removeComponent(effect);
	}
	for (const auto& it : m_p_upgradeInfo->command) {
		runCommand(it.first, it.second, this);
	}
	m_p_entitySystem->remove(this);
}

void splitString(const std::string& string, Tokens& dst) {
	std::istringstream iss(string);
	std::string s;
	while (getline(iss, s, ' ')) {
		dst.emplace_back(s);
	}
}

void Upgrade::initialize(EntitySystem* entitySystem, Player* player) {
	s_m_commands.emplace("heal", [entitySystem] (const std::string& args, Upgrade* upgrade) {
		Tokens tokens;
		splitString(args, tokens);
		if (tokens.empty()) return;
		IEntity* entity = entitySystem->getClosestEntity(upgrade->getPosition(), EntityType::UNDEFINED, (tokens.size() > 1 ? tokens[1] : ""));
		if (entity == nullptr) return;
		else entity->giveHitPoints(stoi(tokens[0]));
	});
	s_m_commands.emplace("bomb", [entitySystem, player] (const std::string& args, Upgrade* upgrade) {
		std::vector<IEntity*> enemies;
		for (size_t i = 0; i < entitySystem->getSize(); i++) {
			IEntity* entity = entitySystem->getEntity(i);
			if (entity->getType() != EntityType::ENEMY || typeid(*entity) != typeid(SpaceShip)) continue;
			enemies.emplace_back(entity);
		}
		if (enemies.size() >= 1) {
			size_t random = Random::get<size_t>(0U, enemies.size() - 1);
			enemies[random]->kill();
			player->onEnemyKilled(static_cast<SpaceShipInfo*>(enemies[random]->getInfo()));
		}
		else return;
	});
}

void Upgrade::runCommand(const std::string& command, const std::string& args, Upgrade* upgrade) {
	try {
		const auto& function = s_m_commands.at(command);
		try {
			function(args, upgrade);
		}
		catch (const std::exception& e) {
			ERR(e.what());
		}
	}
	catch (const std::exception&) {
		ERR(command << ": command doe's not exist!");
	}
}

void Upgrade::finalize() {
	s_m_commands.clear();
}
