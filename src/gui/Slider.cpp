#include "Slider.hpp"
#include "../Assets.hpp"
#include "../system/Window.hpp"
#include "../system/Input.hpp"
#include "SetOrigin.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

#define WIDTH 350.f
#define HEIGHT 50.f
#define IDLE_COLOR sf::Color(147, 147, 147)

Slider::Slider(Assets* assets, const sf::String& string, const Value& value, int min, int max) :
	m_pressed(false),
	m_p_string(new sf::String(string)),
	m_min(min),
	m_max(max),
	m_value(value),
	m_p_slider(new sf::RectangleShape(sf::Vector2f(HEIGHT / 2.f, HEIGHT))),
	m_p_text(new sf::Text(string + std::visit([](auto& value) { return std::to_string(static_cast<int>(*value)); }, m_value), *assets->getFont("ArialBlack")))
{
	float windowAspectRatio = Window::getAspectRatio();
	m_p_shape->setFillColor(IDLE_COLOR);
	m_p_shape->setOutlineColor(sf::Color::Red);
	m_p_shape->setOutlineThickness(5.f);
	m_p_shape->setScale(windowAspectRatio, windowAspectRatio);
	m_p_slider->setFillColor(sf::Color::Black);
	m_p_slider->setScale(windowAspectRatio, windowAspectRatio);
	m_p_slider->setPosition(sf::Vector2f(calculatePosition(), m_p_shape->getPosition().y - 
							m_p_shape->getOutlineThickness() * windowAspectRatio * m_p_shape->getScale().x));
	m_p_text->setFillColor(sf::Color::Yellow);
	m_p_text->setScale(windowAspectRatio, windowAspectRatio);
	setOrigin(m_origin);
}

Slider::~Slider() {
	delete m_p_text;
	delete m_p_string;
}

void Slider::input(const sf::Vector2f& cursorPos) {
	Widget::input(cursorPos);
	if (m_lastState == State::HOVER) {
		if (Input::jclicked(sf::Mouse::Left)) {
			m_pressed = true;
		}
	}
	if (Input::unclicked(sf::Mouse::Left)) {
		m_pressed = false;
	}
}

void Slider::update(const float dt) {
	if (m_pressed) {
		float outlineThickness = m_p_shape->getOutlineThickness() * m_p_shape->getScale().x;
		float position = static_cast<float>(Window::getCursorPosition().x);
		float left = m_p_shape->getGlobalBounds().left + m_p_slider->getGlobalBounds().width / 2.f + outlineThickness;
		float right = m_p_shape->getGlobalBounds().left + m_p_shape->getGlobalBounds().width - m_p_slider->getGlobalBounds().width / 2.f - outlineThickness;
		if (position > right) position = right;
		if (position < left) position = left;
		float percentage = (position - left) * 100.f / (right - left);
		std::visit([percentage, max = m_max, min = m_min](auto& value) {
			auto temp = *value;
			*value = static_cast<decltype(temp)>((max - min) * percentage / 100.f + min); }, m_value);
		m_p_text->setString(*m_p_string + std::visit([](auto& value) { return  std::to_string(static_cast<int>(*value)); }, m_value));
		setOrigin(m_origin);
		m_p_slider->setPosition(sf::Vector2f(position, m_p_shape->getPosition().y - outlineThickness));
	}
	else if (m_lastState != m_currentState) {
		if (m_currentState == State::IDLE) {
			m_p_shape->setFillColor(IDLE_COLOR);
		}
		else if (m_currentState == State::HOVER) {
			m_p_shape->setFillColor(sf::Color::Cyan);
		}
		m_lastState = m_currentState;
	}
	m_currentState = State::IDLE;
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_p_shape);
	target.draw(*m_p_slider);
	target.draw(*m_p_text);
}

void Slider::setPosition(const sf::Vector2f& position) {
	sf::Vector2f outLine(m_p_shape->getOutlineThickness(), m_p_shape->getOutlineThickness());
	Widget::setPosition(position + outLine);
	m_p_slider->setPosition(calculatePosition(), (position.y + outLine.y) - m_p_shape->getOutlineThickness() * m_p_shape->getScale().x);
	m_p_text->setPosition(position + outLine);
}

void Slider::setSize(const sf::Vector2f& size) {
	Widget::setSize(size);
	m_p_slider->setSize(sf::Vector2f(size.y / 2.f, size.y));
	SetOrigin::setOrigin(*m_p_slider, m_p_slider->getLocalBounds(), Origin::CENTER);
}

void Slider::setOrigin(Origin origin) {
	Widget::setOrigin(origin);
	m_p_text->setOrigin(sf::Vector2f(m_p_text->getLocalBounds().width / 2.f, m_p_text->getLocalBounds().height / 2.f) - m_p_shape->getSize() / 2.f + m_p_shape->getOrigin());
}

void Slider::setString(const sf::String& string) {
	m_p_text->setString(string);
	m_p_text->setOrigin(sf::Vector2f(m_p_text->getLocalBounds().width / 2.f, m_p_text->getLocalBounds().height / 2.f) - m_p_shape->getSize() / 2.f + m_p_shape->getOrigin());
}

float Slider::calculatePosition() {
	float percentage = std::visit([max = m_max, min = m_min](auto& value) { return (*value - min) * 100.f / (max - min); }, m_value);
	float outlineThickness = m_p_shape->getOutlineThickness() * m_p_shape->getScale().x;
	float left = m_p_shape->getGlobalBounds().left + m_p_slider->getGlobalBounds().width / 2.f + outlineThickness;
	float right = m_p_shape->getGlobalBounds().left + m_p_shape->getGlobalBounds().width - m_p_slider->getGlobalBounds().width / 2.f - outlineThickness;
	return percentage * (right - left) / 100.f + left;
}