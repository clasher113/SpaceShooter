#include "MovementComponent.hpp"
#include "../Entity.hpp"
#include "../EntitySystem.hpp"
#include "../EntityInfo.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics/Sprite.hpp>

void normalize(sf::Vector2f& vector) {
	float w = static_cast<float>(sqrt(vector.x * vector.x + vector.y * vector.y));
	vector.x /= w;
	vector.y /= w;
}

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
	normalize(m_movementData.m_direction);
	entity->setRotation(static_cast<float>(atan2(-m_movementData.m_direction.x, m_movementData.m_direction.y) * 180.f / M_PI));
}

void MovementComponent::update(const float dt) {
	m_elapsed += dt;
	if (m_movementData.m_functionData[0].m_p_function.GetParseErrorType() == FunctionParser::ParseErrorType::FP_NO_ERROR) {
		runFunction(dt, 0, m_offset.x);
	}
	if (m_movementData.m_functionData[1].m_p_function.GetParseErrorType() == FunctionParser::ParseErrorType::FP_NO_ERROR) {
		runFunction(dt, 1, m_offset.y);
	}
	if (m_movementData.m_followEnemy) aimEnemy();
	m_currentSpeed += m_movementData.m_accelerationSpeed * dt * *m_p_scale;
	if (m_currentSpeed > m_movementData.m_maxSpeed) m_currentSpeed = m_movementData.m_maxSpeed;
	m_p_entity->move(m_currentSpeed * m_movementData.m_direction.x * dt * *m_p_scale,
					 m_currentSpeed * m_movementData.m_direction.y * dt * *m_p_scale);
}

void MovementComponent::aimEnemy() {
	sf::Vector2f position(m_p_entity->getPosition());
	IEntity* enemy = m_p_entitySystem->getClosestEntity(position, (m_p_entity->getType() == EntityType::ENEMY ? EntityType::PLAYER : EntityType::ENEMY));
	sf::Vector2f target(enemy == nullptr ? position : enemy->getPosition());
	float distance = static_cast<float>(sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y)));
	if (distance > 1) {
		m_movementData.m_direction.x = (target.x - position.x) / distance;
		m_movementData.m_direction.y = (target.y - position.y) / distance;
		normalize(m_movementData.m_direction);
	}
	normalize(m_movementData.m_direction);
	m_p_entity->setRotation(static_cast<float>(atan2(-m_movementData.m_direction.x, m_movementData.m_direction.y) * 180.f / M_PI));
}

void MovementComponent::runFunction(const float dt,  size_t index, float& variable) {
	sf::Vector2f pos(m_p_entity->getPosition() - m_offset);
	float newElements[4] = { pos.x, pos.y, m_elapsed, dt };
	FunctionData* functionData = &m_movementData.m_functionData[index];
	std::copy(std::begin(newElements), std::end(newElements), std::back_inserter(functionData->m_functionVariables));
	size_t variablesSize = functionData->m_functionVariables.size();
	variable = functionData->m_p_function.Eval(functionData->m_functionVariables.data());
	m_p_entity->setPosition(sf::Vector2f(pos + m_offset));
	if (variablesSize >= 4) functionData->m_functionVariables.resize(variablesSize - 4);
}