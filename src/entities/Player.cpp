#include "Player.hpp"
#include "../graphics/Layers.hpp"
#include "../gui/ProgressBar.hpp"
#include "../gui/SetOrigin.hpp"
#include "../system/Window.hpp"
#include "EntityInfo.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Player::Player(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem) : SpaceShip(assets, name, x, y, entitySystem),
	m_score(0)
{
	sf::Vector2f windowSize(Window::getSize());
	m_p_healthBar->setPosition(sf::Vector2f(windowSize.x * 2.f / 100.f, windowSize.x * 2.f / 100.f));
	m_p_healthBar->setSize(sf::Vector2f(windowSize.x / 3.5f, 10.f));
	m_p_healthBar->setOrigin(Origin::LEFT_TOP);
}

Player::~Player() {
}

void Player::update(const float dt) {
	Entity::update(dt);
	m_p_healthBar->update(dt);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Entity::draw(target, states);
	target.draw(*m_p_healthBar);
}

void Player::draw(Layers& layers) const {
	Entity::draw(layers);
	layers.addDrawable(GUI_LAYER, m_p_healthBar);
}

void Player::onEnemyKilled(SpaceShipInfo* enemyInfo) {
	m_score += enemyInfo->maxHealth;
	m_onScoreChangeCallback();
}

void Player::onEnemyLost(SpaceShipInfo* enemyInfo) {
	m_score -= (enemyInfo->maxHealth << 1);
	m_onScoreChangeCallback();
}