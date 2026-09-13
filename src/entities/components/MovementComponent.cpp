#include "MovementComponent.hpp"
#include "../Entity.hpp"
#include "../EntitySystem.hpp"
#include "../EntityInfo.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics/Sprite.hpp>

MovementComponent::MovementComponent(MovementData& movementData, EntitySystem* entitySystem, Entity* entity) : IComponent(),
	m_movementData(movementData),
	m_p_entitySystem(entitySystem),
	m_p_entity(entity),
	m_currentSpeed(movementData.m_startSpeed),
	m_p_scale(entity->getScale()),
	m_elapsed(0.f)
{
	if (m_movementData.m_aimEnemy) {
		aimEnemy();
	}
	m_movementData.m_direction = m_movementData.m_direction.normalized();
	entity->setRotation(static_cast<float>(atan2(-m_movementData.m_direction.x, m_movementData.m_direction.y) * 180.f / M_PI));
}

void MovementComponent::update(const float dt) {
	m_elapsed += dt;
	if (m_movementData.parser[0].has_value()) {
		runFunction(dt, 0, m_offset.x);
	}
	if (m_movementData.parser[1].has_value()) {
		runFunction(dt, 1, m_offset.y);
	}

	if (m_movementData.m_followEnemy) aimEnemy();
	m_currentSpeed += m_movementData.m_accelerationSpeed * dt * *m_p_scale;
	if (m_currentSpeed > m_movementData.m_maxSpeed) m_currentSpeed = m_movementData.m_maxSpeed;
	m_p_entity->move(sf::Vector2f(m_currentSpeed * m_movementData.m_direction.x * dt * *m_p_scale,
					 m_currentSpeed * m_movementData.m_direction.y * dt * *m_p_scale));
}

void MovementComponent::aimEnemy() {
	sf::Vector2f position(m_p_entity->getPosition());
	IEntity* enemy = m_p_entitySystem->getClosestEntity(position, (m_p_entity->getType() == EntityType::ENEMY ? EntityType::PLAYER : EntityType::ENEMY));
	sf::Vector2f target(enemy == nullptr ? position : enemy->getPosition());
	float distance = static_cast<float>(sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y)));
	if (distance > 1) {
		m_movementData.m_direction.x = (target.x - position.x) / distance;
		m_movementData.m_direction.y = (target.y - position.y) / distance;
	}
	m_movementData.m_direction = m_movementData.m_direction.normalized();
	m_p_entity->setRotation(static_cast<float>(atan2(-m_movementData.m_direction.x, m_movementData.m_direction.y) * 180.f / M_PI));
}

void MovementComponent::runFunction(const float dt,  size_t index, float& variable) {
	sf::Vector2f pos(m_p_entity->getPosition() - m_offset);
	mu::Parser& parser = m_movementData.parser[index].value();
	parser.DefineConst("posX", pos.x);
	parser.DefineConst("posY", pos.y);
	parser.DefineConst("elapsed", m_elapsed);
	parser.DefineConst("dt", dt);

	try {
		variable = parser.Eval();
		m_p_entity->setPosition(sf::Vector2f(pos + m_offset));
	}
	catch (mu::Parser::exception_type& e) {
		std::cout << e.GetMsg() << std::endl;
	}
}