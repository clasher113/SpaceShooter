#include "GameMenu.hpp"
#include "GuiFactoryElements.hpp"
#include "Settings.hpp"
#include "Game.hpp"
#include "../system/Input.hpp"

#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

GameMenu::GameMenu(Application& application) : GuiFactory(application),
	m_p_tapToBegin(nullptr),
	m_p_animationClock(new sf::Clock()),
	m_p_logo(new sf::Sprite(m_p_assets->getTexture("Logo"))),
	m_p_sceneMusic(AudioManager::playMusic("MenuMusic", true))
{
	GET_WINDOW_SIZE_AND_ASPECTRATIO
	m_p_tapToBegin = addText(m_p_assets->getString("gui.string.tapToBegin"), sf::Vector2f(windowSize.x / 2.f, windowSize.y * 65.f / 100.f),
			sf::Color(255, 255, 153, 255), 50U);
	m_p_tapToBegin->setRotation(6.f);
	addButton(m_p_assets->getString("gui.button.settings"), sf::Vector2f(Window::getSize().x / 2.f, Window::getSize().y * 5.f / 100.f),
			  WIDGET_SIZE_NARROW, [&app = application]() { app.pushState<Settings>(); });
	m_p_logo->setScale(Window::getAspectRatio(), Window::getAspectRatio());
	m_p_logo->setOrigin(m_p_logo->getLocalBounds().width / 2.f, m_p_logo->getLocalBounds().height / 2.f);
	m_p_logo->setPosition(windowSize.x / 2, windowSize.y * 30.f / 100.f);
}

GameMenu::~GameMenu() {
	AudioManager::setStatus(m_p_sceneMusic, Status::STOP);
	delete m_p_animationClock;
	delete m_p_logo;
}

void GameMenu::input() {
	GuiFactory::input();
	if (Input::jclicked(sf::Mouse::Left)) {
		for (const auto& it : m_widgets) {
			if (it->getState() == Widget::State::PRESSED)
				return;
		}
		m_p_application->changeState<Game>();
	}
}

void GameMenu::update(const float dt) {
	GuiFactory::update(dt);
	sf::Color color = m_p_tapToBegin->getFillColor();
	m_p_tapToBegin->setFillColor(sf::Color(color.r, color.g, color.b,
								  (static_cast<int>(m_p_animationClock->getElapsedTime().asSeconds()) % 2 ? 255U : 0U)));
}

void GameMenu::draw(sf::RenderWindow& window) {
	GuiFactory::draw(window);
	window.draw(*m_p_logo);
}