#include "ProgressBar.hpp"
#include "../system/Window.hpp"
#include "SetOrigin.hpp"
#include "../graphics/Layers.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#define ANIMATION_SPEED 95.f

ProgressBar::ProgressBar(const unsigned int& value, int max, float width, float height):
	m_value(value),
	m_max(max),
	m_showValue(static_cast<float>(value)),
	m_p_border(new sf::RectangleShape(sf::Vector2f(width, height))),
	m_p_level(new sf::RectangleShape(sf::Vector2f(width / max * value, height)))
{
	float aspectRatio = Window::getAspectRatio();
	m_p_border->setFillColor(sf::Color::Transparent);
	m_p_border->setOutlineColor(sf::Color::White);
	m_p_border->setOutlineThickness(3.f);
	m_p_border->setScale(aspectRatio, aspectRatio);
	SetOrigin::setOrigin(*m_p_border, m_p_border->getLocalBounds(), Origin::CENTER);
	m_p_level->setFillColor(sf::Color::Red);
	m_p_level->setScale(aspectRatio, aspectRatio);
	SetOrigin::setOrigin(*m_p_level, m_p_border->getLocalBounds(), Origin::CENTER);
}

ProgressBar::~ProgressBar() {
	delete m_p_border;
	delete m_p_level;
}

void ProgressBar::update(const float dt) {
	if (static_cast<unsigned int>(m_showValue) == m_value) return;
	sf::Vector2f size = m_p_border->getSize();
	float step = (size.x / 100) * ANIMATION_SPEED * dt;
	float difference = m_value - m_showValue;
	if (std::abs(difference) < step) m_showValue = m_value;
	else m_showValue += step * std::clamp(difference, -1.f, 1.f);
	m_p_level->setSize(sf::Vector2f(size.x / m_max * m_showValue, size.y));
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_p_border);
	target.draw(*m_p_level);
}

void ProgressBar::setPosition(const sf::Vector2f& position) {
	m_p_border->setPosition(position);
	m_p_level->setPosition(position);
}

void ProgressBar::setSize(const sf::Vector2f& size) {
	m_p_border->setSize(size);
	m_p_level->setSize(sf::Vector2f(size.x / m_max * m_value, size.y));
}

void ProgressBar::setOrigin(Origin origin) {
	SetOrigin::setOrigin(*m_p_border, m_p_border->getLocalBounds(), origin);
	SetOrigin::setOrigin(*m_p_level, m_p_border->getLocalBounds(), origin);
}
