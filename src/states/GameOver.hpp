#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "GuiFactory.hpp"

class GameOver : public GuiFactory
{
public:
	GameOver(Application& application);
	~GameOver();

	void input() override;
	void update(const float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	sf::Music* m_p_sceneMusic;
	sf::Text* m_p_tapToRestart;
	sf::Clock* m_p_animationClock;
	sf::Sprite* m_p_logo;
};

#endif // !GAME_OVER_H