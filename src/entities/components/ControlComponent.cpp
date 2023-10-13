#include "ControlComponent.hpp"
#include "AnimationComponent.hpp"
#include "../Entity.hpp"
#include "../../system/Window.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

ControlComponent::ControlComponent(Entity* entity, sf::Vector2f maxSpeed, sf::Vector2f accelerationSpeed) :
	m_left(false),
	m_right(false),
	m_forward(false),
	m_back(false),
	m_dx(0.f),
	m_dy(0.f),
	m_p_scale(entity->getScale()),
	m_p_entity(entity),
	m_maxSpeed(maxSpeed),
	m_accelerationSpeed(accelerationSpeed)
{
};

ControlComponent::~ControlComponent() {
}

void ControlComponent::input() {
	m_left = sf::Keyboard().isKeyPressed(sf::Keyboard::Left) || sf::Keyboard().isKeyPressed(sf::Keyboard::A);
	m_right = sf::Keyboard().isKeyPressed(sf::Keyboard::Right) || sf::Keyboard().isKeyPressed(sf::Keyboard::D);
	m_forward = sf::Keyboard().isKeyPressed(sf::Keyboard::Up) || sf::Keyboard().isKeyPressed(sf::Keyboard::W);
	m_back = sf::Keyboard().isKeyPressed(sf::Keyboard::Down) || sf::Keyboard().isKeyPressed(sf::Keyboard::S);
}

void ControlComponent::update(const float dt) {
	AnimationComponent* animation = m_p_entity->getComponent<AnimationComponent>();
	if(animation != nullptr)
		animation->setOffset(0.f);
	// right
	if (m_right && !m_left) {
		if (m_dx < m_maxSpeed.x) {
			m_dx += m_accelerationSpeed.x * dt * *m_p_scale;
			if (m_dx > m_maxSpeed.x) m_dx = m_maxSpeed.x;
		}
		if (animation != nullptr)
			animation->setOffset(14.f);
	}
	else if (m_dx > 0) {
		m_dx -= m_accelerationSpeed.x * dt * *m_p_scale;
		if (m_dx < 0) m_dx = 0;
	}
	// left
	if (m_left && !m_right) {
		if (m_dx > -m_maxSpeed.x) {
			m_dx -= m_accelerationSpeed.x * dt * *m_p_scale;
			if (m_dx < -m_maxSpeed.x) m_dx = -m_maxSpeed.x;
		}
		if (animation != nullptr)
			animation->setOffset(7.f);
	}
	else if (m_dx < 0) {
		m_dx += m_accelerationSpeed.x * dt * *m_p_scale;
		if (m_dx > 0) m_dx = 0;
	}
	// up
	if (m_forward && !m_back) {
		if (m_dy > -m_maxSpeed.y) {
			m_dy -= m_accelerationSpeed.y * dt * *m_p_scale;
			if (m_dy < -m_maxSpeed.y) m_dy = -m_maxSpeed.y;
		}
	}
	else if (m_dy <= 0) {
		m_dy += m_accelerationSpeed.y * dt * *m_p_scale;
		if (m_dy > 0) m_dy = 0;
	}
	// down
	if (m_back && !m_forward) {
		if (m_dy < m_maxSpeed.y) {
			m_dy += m_accelerationSpeed.y * dt * *m_p_scale;
			if (m_dy > m_maxSpeed.y) m_dy = m_maxSpeed.y;
		}
	}
	else if (m_dy >= 0) {
		m_dy -= m_accelerationSpeed.y * dt * *m_p_scale;
		if (m_dy < 0) m_dy = 0;
	}
	sf::Vector2u windowSize = Window::getSize();
	sf::Vector2f pos = m_p_entity->getPosition() + sf::Vector2f(m_dx, m_dy) * dt;
	sf::Vector2f halfSize = sf::Vector2f(m_p_entity->getHitbox().width / 2.f, m_p_entity->getHitbox().height / 2.f);
	if (pos.x >= windowSize.x - halfSize.x) pos.x = windowSize.x - halfSize.x;
	if (pos.x <= halfSize.x) pos.x = halfSize.x;
	if (pos.y <= windowSize.y / 2.f + halfSize.y) pos.y = windowSize.y / 2.f + halfSize.y;
	if (pos.y >= windowSize.y - halfSize.y) pos.y = windowSize.y - halfSize.y;
	m_p_entity->setPosition(pos);
}