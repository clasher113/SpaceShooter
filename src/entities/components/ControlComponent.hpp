#ifndef CONTROL_COMPONENT_HPP
#define CONTROL_COMPONENT_HPP

#include "IComponent.hpp"

#include <SFML/System/Vector2.hpp>

class Entity;

class ControlComponent : public IComponent {
public:
	ControlComponent(Entity* entity, sf::Vector2f maxSpeed, sf::Vector2f accelerationSpeed);
	~ControlComponent() override;

	void input() override;
	void update(const float dt) override;
private:
	bool m_left, m_right, m_forward, m_back;
	float m_dx, m_dy;
	float* m_p_scale;
	sf::Vector2f m_accelerationSpeed;
	sf::Vector2f m_maxSpeed;
	Entity* m_p_entity;
};

#endif // !CONTROL_COMPONENT_HPP