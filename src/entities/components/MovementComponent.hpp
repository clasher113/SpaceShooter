#ifndef MOVE_COMPONENT_HPP
#define MOVE_COMPONENT_HPP

#include "IComponent.hpp"

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <fparser_mpfr.hh>

namespace sf {
	class Sprite;
}
class EntitySystem;
class Entity;

struct FunctionData {
	FunctionParser_f m_p_function;
	std::vector<float> m_functionVariables;
};

struct MovementData {
	//~MovementData() { if (m_p_function != nullptr) delete m_p_function; };
	bool m_aimEnemy = false;
	bool m_followEnemy = false;
	float m_maxSpeed = 100.f;
	float m_startSpeed = m_maxSpeed;
	float m_accelerationSpeed = m_maxSpeed * 100;
	sf::Vector2f m_direction{ 0, 0 };
	FunctionData m_functionData[2];
};

class MovementComponent : public IComponent {
public:
	MovementComponent(MovementData& movementData, EntitySystem* entitySystem, Entity* entity);

	void input() override {};
	void update(const float dt) override;
private:
	float* m_p_scale;
	float m_currentSpeed;
	float m_elapsed;
	sf::Vector2f m_offset;
	MovementData m_movementData;

	Entity* m_p_entity;
	EntitySystem* m_p_entitySystem;
	void aimEnemy();
	void runFunction(const float dt, size_t index, float& variable);
};

#endif // !MOVE_COMPONENT_HPP