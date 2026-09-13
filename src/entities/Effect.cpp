#include "Effect.hpp"
#include "EntityInfo.hpp"
#include "components/AnimationComponent.hpp"
#include "EntitySystem.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

Effect::Effect(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem) : Entity(assets, name, x, y, entitySystem),
	m_p_effectInfo(static_cast<EffectInfo*>(m_p_entityInfo)),
	m_loopAnimation(false),
	m_p_pinToEntity(nullptr)
{
}

Effect::~Effect() {
}

void Effect::update(const float dt) {
	Entity::update(dt);
	if (!m_loopAnimation) {
		AnimationComponent* animation = getComponent<AnimationComponent>();
		if (animation != nullptr && animation->getCycle() >= 1)
			m_isDead = true;
	}
	if (m_p_pinToEntity != nullptr) {
		m_p_sprite->setPosition(m_p_pinToEntity->getPosition() + m_offset);
	}
}

void Effect::pinTo(const IEntity* const entity) {
	m_p_pinToEntity = entity;
}

void Effect::setOffset(const sf::Vector2f& offset) {
	m_offset = offset;
}

void Effect::setLoop(bool loop) {
	m_loopAnimation = loop;
}