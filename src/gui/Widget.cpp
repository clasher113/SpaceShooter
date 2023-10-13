#include "Widget.hpp"
#include "../system/Input.hpp"
#include "SetOrigin.hpp"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Widget::Widget() :
	m_currentState(State::IDLE),
	m_lastState(State::IDLE),
	m_origin(Origin::CENTER),
	m_p_shape(new sf::RectangleShape(sf::Vector2f(350.f, 50.f)))
{
}

Widget::~Widget() {
	delete m_p_shape;
}

void Widget::input(const sf::Vector2f& cursorPos) {
	if (m_p_shape->getGlobalBounds().contains(cursorPos)) {
		if (!Input::clicked(sf::Mouse::Left))
			m_currentState = State::HOVER;
	}
}

void Widget::setPosition(const sf::Vector2f& position) {
	m_p_shape->setPosition(position);
}

void Widget::setSize(const sf::Vector2f& size) {
	m_p_shape->setSize(size);
	setOrigin(m_origin);
}

void Widget::setOrigin(Origin origin) {
	m_origin = origin;
	SetOrigin::setOrigin(*m_p_shape, m_p_shape->getLocalBounds(), origin);
}

const sf::Vector2f& Widget::getSize() {
	return m_p_shape->getSize();
}

const sf::Vector2f Widget::getPosition() {
	sf::Vector2f outLine(m_p_shape->getOutlineThickness(), m_p_shape->getOutlineThickness());
	return m_p_shape->getPosition() - outLine;
}

const sf::FloatRect Widget::getLocalBounds() {
	return m_p_shape->getLocalBounds();
}
