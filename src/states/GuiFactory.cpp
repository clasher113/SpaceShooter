#include "GuiFactory.hpp"
#include "../Assets.hpp"
#include "../system/Window.hpp"
#include "../gui/Button.hpp"
#include "../gui/Slider.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

GuiFactory::GuiFactory(Application& application) : State(application),
	m_p_background(new sf::Sprite(m_p_assets->getTexture("Background_3")))
{
	float windowAspectRatio = Window::getAspectRatio();
	sf::Vector2u windowSize = Window::getSize();
	m_p_background->setScale(windowAspectRatio, windowAspectRatio);
	m_p_background->setOrigin(sf::Vector2f(m_p_background->getLocalBounds().width / 2, m_p_background->getLocalBounds().height / 2));
	m_p_background->setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
}

GuiFactory::~GuiFactory() {
	delete m_p_background;
	for (const auto& it : m_widgets) {
		delete it;
	}
	for (const auto& it : m_drawable) {
		delete it;
	}
}

void GuiFactory::input() {
	for (const auto& it : m_widgets) {
		it->input(sf::Vector2f(Window::getCursorPosition()));
	}
}

void GuiFactory::update(const float dt) {
	for (const auto& it : m_widgets) {
		it->update(dt);
	}
}

void GuiFactory::draw(sf::RenderWindow& window) {
	window.draw(*m_p_background);
	for (const auto& it : m_widgets) {
		window.draw(*it);
	}
	for (const auto& it : m_drawable) {
		window.draw(*it);
	}
}

Button* GuiFactory::addButton(const sf::String& string, const sf::Vector2f& position, const sf::Vector2f& size, const std::function<void()>& callback, Origin origin) {
	Button* button = static_cast<Button*>(m_widgets.emplace_back(new Button(m_p_assets, callback)));
	button->setSize(size);
	button->setPosition(position);
	button->setString(string);
	button->setOrigin(origin);
	return button;
}

Slider* GuiFactory::addSlider(const sf::String& string, const sf::Vector2f& position, const sf::Vector2f& size, const std::variant<int*, unsigned int*, float*>& value, int min, int max) {
	Slider* slider = static_cast<Slider*>(m_widgets.emplace_back(new Slider(m_p_assets, string, value, min, max)));
	slider->setSize(size);
	slider->setPosition(position);
	return slider;
}

sf::Text* GuiFactory::addText(const sf::String& string, const sf::Vector2f& position, const sf::Color& color, unsigned int fontSize, Origin origin) {
	sf::Text* text = static_cast<sf::Text*>(m_drawable.emplace_back(new sf::Text(string, *m_p_assets->getFont("ArialBlack"), fontSize)));
	text->setPosition(position);
	text->setFillColor(color);
	text->setScale(Window::getAspectRatio(), Window::getAspectRatio());
	SetOrigin::setOrigin(*text, text->getLocalBounds(), origin);
	return text;
}
