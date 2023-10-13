#include "SpaceShip.hpp"
#include "EntityInfo.hpp"
#include "EntitySystem.hpp"
#include "../graphics/Layers.hpp"
#include "components/EffectComponent.hpp"
#include "components/AnimationComponent.hpp"
#include "Effect.hpp"
#include "../gui/ProgressBar.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cassert>

SpaceShip::SpaceShip(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem) : Entity(assets, name, x, y, entitySystem),
	m_p_spaceShipInfo(static_cast<SpaceShipInfo*>(m_p_entityInfo)),
	m_currentHealth(static_cast<SpaceShipInfo*>(m_p_entityInfo)->maxHealth),
	m_p_healthBar(nullptr)
{
	float width = m_p_sprite->getLocalBounds().width * ENTITIES_SCALE;
	m_p_healthBar = new ProgressBar(m_currentHealth, m_p_spaceShipInfo->maxHealth, width, 10.f);
}

SpaceShip::~SpaceShip() {
	delete m_p_healthBar;
}

void SpaceShip::update(const float dt) {
	Entity::update(dt);
	sf::Vector2f position = m_p_sprite->getPosition();
	m_p_healthBar->setPosition(sf::Vector2f(position.x, position.y - m_p_sprite->getGlobalBounds().height / 1.5f));
	m_p_healthBar->update(dt);
}

void SpaceShip::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Entity::draw(target, states);
	if (m_currentHealth != m_p_spaceShipInfo->maxHealth)
		target.draw(*m_p_healthBar);
}

void SpaceShip::draw(Layers& layers) const {
	Entity::draw(layers);
	if (m_currentHealth != m_p_spaceShipInfo->maxHealth)
		layers.addDrawable(GUI_LAYER, m_p_healthBar);
}

void SpaceShip::giveDamage(int damage) {
	assert(!(damage < 0) && "Negative damage not allowed!");
	if ((static_cast<int>(m_currentHealth) - damage) <= 0) {
		m_currentHealth = 0;
		EffectComponent* effectComponent = static_cast<EffectComponent*>(getComponent<EffectComponent>());
		if (effectComponent != nullptr) {
			Effect* lastEffect = effectComponent->getLastEffect(EffectState::ON_DEPLOY);
			if (lastEffect != nullptr)
				lastEffect->setIsDead(true);
			effectComponent->onDeathEffect();
		}
		m_p_entitySystem->remove(this);
	}
	m_currentHealth -= damage;
}

void SpaceShip::giveHitPoints(int hitPoints) {
	assert(!(hitPoints < 0) && "Negative hit points not allowed!");
	m_currentHealth += hitPoints;
	if (m_currentHealth > m_p_spaceShipInfo->maxHealth)
		m_currentHealth = m_p_spaceShipInfo->maxHealth;
}

unsigned int SpaceShip::getDamage() const {
	return (m_currentHealth << 1);
}