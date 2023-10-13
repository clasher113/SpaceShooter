#include "GameOver.hpp"
#include "GuiFactoryElements.hpp"
#include "../system/Input.hpp"
#include "Game.hpp"
#include "Settings.hpp"
#include "GameMenu.hpp"
#include "../utils/Score.hpp"

#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

GameOver::GameOver(Application& application) : GuiFactory(application),
	m_p_tapToRestart(nullptr),
	m_p_animationClock(new sf::Clock()),
	m_p_logo(new sf::Sprite(m_p_assets->getTexture("Logo"))),
	m_p_sceneMusic(AudioManager::playMusic("MenuMusic", true))
{
	GET_WINDOW_SIZE_AND_ASPECTRATIO
	Score score = readScore();
	addText(m_p_assets->getString("gui.string.gameOver"), sf::Vector2f(windowSize.x / 2.f, windowSize.y * 40.f / 100.f),
			sf::Color(255, 255, 255, 255), 70U);
	m_p_tapToRestart = addText(m_p_assets->getString("gui.string.tapToRestart"), sf::Vector2f(windowSize.x / 2.f, windowSize.y * 65.f / 100.f),
			sf::Color(255, 255, 153, 255), 50U);
	m_p_tapToRestart->setRotation(6.f);
	addText(m_p_assets->getString("gui.string.score") + std::string(": ") + std::to_string(score.score), sf::Vector2f(windowSize.x * 25.f / 100.f, windowSize.y * 50.f / 100.f),
			sf::Color(255, 255, 255, 255), 55U, Origin::LEFT_CENTER);
	addText(m_p_assets->getString("gui.string.highScore") + std::string(": ") + std::to_string(score.hightScore), sf::Vector2f(windowSize.x * 25.f / 100.f, windowSize.y * 55.f / 100.f),
			sf::Color(255, 255, 255, 255), 55U, Origin::LEFT_CENTER);
	Widget* widget = addButton(m_p_assets->getString("gui.button.settings"), sf::Vector2f(windowSize.x * 29.f / 100.f, windowSize.y * 95.f / 100.f),
			  WIDGET_SIZE_NARROW, [&app = *m_p_application]() { app.pushState<Settings>(); });
	addButton(m_p_assets->getString("gui.button.leftToMenu"), sf::Vector2f(windowSize.x * 71.f / 100.f, windowSize.y * 95.f / 100.f),
			  WIDGET_SIZE_NARROW, [&app = *m_p_application]() { app.changeState<GameMenu>(); });
	addButton(m_p_assets->getString("gui.button.exit"), sf::Vector2f(windowSize.x * 71.f / 100.f, ABOVE_THAN(widget)),
			  WIDGET_SIZE_NARROW, []() {Window::close(); });
	m_p_logo->setScale(aspectRatio, aspectRatio);
	m_p_logo->setOrigin(m_p_logo->getLocalBounds().width / 2.f, m_p_logo->getLocalBounds().height / 2.f);
	m_p_logo->setPosition(windowSize.x / 2, windowSize.y * 30.f / 100.f);
}

GameOver::~GameOver() {
	delete m_p_animationClock;
	AudioManager::setStatus(m_p_sceneMusic, Status::STOP);
}

void GameOver::input() {
	GuiFactory::input();
	if (Input::jclicked(sf::Mouse::Left)) {
		for (const auto& it : m_widgets) {
			if (it->getState() == Widget::State::PRESSED)
				return;
		}
		m_p_application->changeState<Game>();
	}
}

void GameOver::update(const float dt) {
	GuiFactory::update(dt);
	sf::Color color = m_p_tapToRestart->getFillColor();
	m_p_tapToRestart->setFillColor(sf::Color(color.r, color.g, color.b,
								  (static_cast<int>(m_p_animationClock->getElapsedTime().asSeconds()) % 2 ? 255U : 0U)));
}

void GameOver::draw(sf::RenderWindow& window) {
	GuiFactory::draw(window);
	window.draw(*m_p_logo);
}