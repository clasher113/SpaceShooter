#include "Button.hpp"
#include "../Assets.hpp"
#include "../system/Window.hpp"
#include "../system/Input.hpp"
#include "SetOrigin.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>

#define IDLE_COLOR sf::Color(147, 147, 147)

Button::Button(Assets* assets, const std::function<void()>& callback) :
	m_callback(callback),
	m_p_text(new sf::Text("I'm a button", *assets->getFont("ArialBlack")))
{
	float windowAspectRatio = Window::getAspectRatio();
	m_p_shape->setFillColor(IDLE_COLOR);
	m_p_shape->setOutlineColor(sf::Color::Red);
	m_p_shape->setOutlineThickness(5.f);
	m_p_shape->setScale(windowAspectRatio, windowAspectRatio);
	m_p_text->setFillColor(sf::Color::Yellow);
	m_p_text->setScale(windowAspectRatio, windowAspectRatio);
	setOrigin(Origin::CENTER);
}

Button::~Button() {
	delete m_p_text;
}

void Button::input(const sf::Vector2f& cursorPos) {
	Widget::input(cursorPos);
	if (m_lastState == State::HOVER) {
		if (Input::jclicked(sf::Mouse::Left)) {
			m_currentState = State::PRESSED;
		}
	}
}

void Button::update(const float dt) {
	if (m_lastState != m_currentState) {
		if (m_currentState == State::HOVER) {
			m_p_shape->setFillColor(sf::Color::Cyan);
		}
		else if (m_currentState == State::IDLE) {
			m_p_shape->setFillColor(IDLE_COLOR);
		}
		else if (m_currentState == State::PRESSED) {
			m_callback();
		}
		m_lastState = m_currentState;
	}
	m_currentState = State::IDLE;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_p_shape);
	target.draw(*m_p_text);
}

void Button::setOrigin(Origin origin) {
	Widget::setOrigin(origin);
	m_p_text->setOrigin(sf::Vector2f(m_p_text->getLocalBounds().width / 2.f, m_p_text->getLocalBounds().height / 2.f) - m_p_shape->getSize() / 2.f + m_p_shape->getOrigin());
}

void Button::setString(const sf::String& string) {
	m_p_text->setString(string);
	m_p_text->setOrigin(sf::Vector2f(m_p_text->getLocalBounds().width / 2.f, m_p_text->getLocalBounds().height / 2.f) - m_p_shape->getSize() / 2.f + m_p_shape->getOrigin());
}

void Button::setPosition(const sf::Vector2f& position) {
	sf::Vector2f outLine(m_p_shape->getOutlineThickness(), m_p_shape->getOutlineThickness());
	Widget::setPosition(position + outLine);
	m_p_text->setPosition(position + outLine);
}