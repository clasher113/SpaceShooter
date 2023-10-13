#include "ShootingComponent.hpp"
#include "../EntityInfo.hpp"
#include "../EntitySystem.hpp"
#include "../Bullet.hpp"
#include "EffectComponent.hpp"
#include "MovementComponent.hpp"
#include "../../system/Input.hpp"

ShootingComponent::ShootingComponent(EntitySystem* entitySystem, ShootingData* shootingData, Entity* parent) :
	m_p_entitySystem(entitySystem),
	m_p_shootingData(shootingData),
	m_p_parent(parent),
	m_shootTimer(shootingData->m_shootInterval / 2.f),
	m_queueTimer(shootingData->m_queueInterval / 2.f),
	m_fire(false),
	m_queueCounter(0)
{
}

ShootingComponent::~ShootingComponent() {
}

void ShootingComponent::input() {
	if (m_p_shootingData->m_shootingMode == ShootingMode::MANUAL) m_fire = Input::pressed(m_p_shootingData->m_key);
}

void ShootingComponent::update(const float dt) {
	if (m_queueTimer < m_p_shootingData->m_queueInterval) m_queueTimer += dt;
	if (m_shootTimer < m_p_shootingData->m_shootInterval) m_shootTimer += dt;
	if (m_queueTimer >= m_p_shootingData->m_queueInterval && (m_fire || m_p_shootingData->m_shootingMode == ShootingMode::AUTOMATIC)) {
		if (m_shootTimer >= m_p_shootingData->m_shootInterval) {
			m_queueCounter++;
			m_shootTimer = 0.f;
			sf::Vector2f pos = m_p_parent->getPosition();
			float scale = *m_p_parent->getScale();
			Bullet* bullet = m_p_entitySystem->spawn<Bullet>(m_p_shootingData->m_bulletName, pos.x + m_p_shootingData->m_positionOffset.x * scale,
																							 pos.y + m_p_shootingData->m_positionOffset.y * scale);
			if (m_p_shootingData->m_p_movementData != nullptr) {
				bullet->addComponent(new MovementComponent(*m_p_shootingData->m_p_movementData, m_p_entitySystem, bullet));
			}
			EffectComponent* effect = m_p_parent->getComponent<EffectComponent>();
			if (effect != nullptr) {
				effect->onShootEffect(m_p_shootingData);
			}
			if (m_queueCounter == m_p_shootingData->m_queueSize) {
				m_queueCounter = 0;
				m_queueTimer = 0.f;
			}
		}
	}
}