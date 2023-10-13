#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SpaceShip.hpp"
#include <functional>

class Player : public SpaceShip {
public:
	Player(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem);
	~Player() override;

	void update(const float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void draw(Layers& layers) const override;

	void setOnScoreChangeCallback(const std::function<void()>& callback) { m_onScoreChangeCallback = callback; callback(); };

	void onEnemyKilled(SpaceShipInfo* enemyInfo);
	void onEnemyLost(SpaceShipInfo* enemyInfo);

	int getScore() { return m_score; }
private:
	int m_score;

	std::function<void()> m_onScoreChangeCallback;
};

#endif // !PLAYER_HPP