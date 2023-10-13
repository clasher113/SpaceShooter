#include "Game.hpp"
#include "../Application.hpp"
#include "../Assets.hpp"
#include "../entities/EntitySystem.hpp"
#include "../entities/Player.hpp"
#include "../entities/Effect.hpp"
#include "../entities/Upgrade.hpp"
#include "../entities/components/ControlComponent.hpp"
#include "../entities/components/EffectComponent.hpp"
#include "../audio/AudioManager.hpp"
#include "../system/Window.hpp"
#include "../system/Input.hpp"
#include "Pause.hpp"
#include "GameOver.hpp"
#include "../gui/SetOrigin.hpp"
#include "../utils/Score.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

Game::Game(Application& application) : State(application),
	m_p_entitySystem(new EntitySystem(m_p_assets)),
	m_p_sceneMusic(AudioManager::playMusic("GameMusic", true)),
	m_p_scoreText(new sf::Text(m_p_assets->getString("gui.string.score") + std::string(": 0"), *m_p_assets->getFont("ArialBlack"), 40U))
{
	m_p_player = m_p_entitySystem->spawn<Player>("Player", Window::getSize().x / 2.f, Window::getSize().y * 85.f / 100.f);
	m_p_player->addComponent(new ControlComponent(m_p_player, sf::Vector2f(500.f, 500.f), sf::Vector2f(1200.f, 1200.f)));
	m_p_player->setOnScoreChangeCallback([scoreText = m_p_scoreText, assets = m_p_assets, player = m_p_player]() {
		scoreText->setString(assets->getString("gui.string.score") + std::string(": ") + std::to_string(player->getScore()));
		SetOrigin::setOrigin(*scoreText, scoreText->getLocalBounds(), Origin::RIGHT_TOP); } 
	);
#ifdef _DEBUG
#define TEST_OBJECT_TYPE Upgrade
	//TEST_OBJECT_TYPE* object = m_p_entitySystem->spawn<TEST_OBJECT_TYPE>("Bomb", Window::getSize().x / 2.f, Window::getSize().y / 2.f);
#endif // _DEBUG

	m_background.emplace_back(new sf::Sprite(m_p_assets->getTexture("Background_1")));
	m_background.emplace_back(new sf::Sprite(m_p_assets->getTexture("Background_2")));
	m_background.emplace_back(new sf::Sprite(m_p_assets->getTexture("Background_3")));
	m_background.emplace_back(new sf::Sprite(m_p_assets->getTexture("Background_4")));
	m_background.emplace_back(new sf::Sprite(m_p_assets->getTexture("Background_5")));

	sf::Vector2f windowSize(Window::getSize());
	float aspectRatio = Window::getAspectRatio();
	for (const auto& it : m_background) {
		it->setOrigin(it->getGlobalBounds().width / 2.f, it->getGlobalBounds().height);
		it->setScale(aspectRatio, aspectRatio);
		it->setPosition(windowSize.x / 2.f, windowSize.y);
		windowSize.y -= it->getGlobalBounds().height;
	}
	windowSize = sf::Vector2f(Window::getSize());

	m_p_scoreText->setScale(aspectRatio, aspectRatio);
	m_p_scoreText->setPosition(sf::Vector2f(windowSize.x * 99.f / 100.f, windowSize.x * 1.f / 100.f));

	Upgrade::initialize(m_p_entitySystem, m_p_player);
#ifndef _DEBUG
	Window::getWindow().setMouseCursorVisible(false);
#endif // _DEBUG
}

Game::~Game() {
	Upgrade::finalize();
	delete m_p_entitySystem;
	delete m_p_scoreText;
	for (const auto& it : m_background) {
		delete it;
	}
	AudioManager::setStatus(Status::STOP);
#ifndef _DEBUG
	Window::getWindow().setMouseCursorVisible(true);
#endif // _DEBUG
}

void Game::input() {
	m_p_player->input();
	m_p_entitySystem->input();
	if (Input::jpressed(sf::Keyboard::Enter)) {
		m_p_application->pushState<Pause>(true);
	}
#ifdef _DEBUG
	if (Input::jpressed(sf::Keyboard::BackSpace)) {
		m_p_player->kill();
	}
#endif // _DEBUG
}

void Game::update(const float dt) {
	m_p_entitySystem->update(m_p_player, dt);

	if (m_p_player->isDead()) {
		EffectComponent* explosion = m_p_player->getComponent<EffectComponent>();
		if (explosion != nullptr){
			Effect* effect = explosion->getLastEffect(EffectState::ON_DEATH);
			if ((effect == nullptr || effect->isDead()) && !m_gameOver) {
				m_gameOver = true;
				writeScore(m_p_player->getScore());
				m_p_application->changeState<GameOver>();
			}
		}
	}

	sf::Sprite* lastSprite = m_background.back();
	for (const auto& it : m_background) {
		it->move(0, 50.f * dt * Window::getAspectRatio());
		if (lastSprite != nullptr && it->getGlobalBounds().top >= Window::getSize().y)
			it->setPosition(it->getPosition().x, lastSprite->getGlobalBounds().top);
		lastSprite = it;
	}
}

void Game::draw(sf::RenderWindow& window) {
	for (const auto& it : m_background) {
		if (it->getPosition().y > 0) window.draw(*it);
	}
	m_p_entitySystem->draw(window);
	window.draw(*m_p_scoreText);
}
