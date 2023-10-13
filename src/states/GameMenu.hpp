#ifndef GAME_MENU_HPP
#define GAME_MENU_HPP

#include "GuiFactory.hpp"

class GameMenu : public GuiFactory {
public:
	GameMenu(Application& application);
	~GameMenu();

	void input() override;
	void update(const float dt) override;
	void draw(sf::RenderWindow& window) override; 

private:
	sf::Music* m_p_sceneMusic;
	sf::Clock* m_p_animationClock;
	sf::Text* m_p_tapToBegin;
	sf::Sprite* m_p_logo;
};

#endif // !GAME_MENU_HPP