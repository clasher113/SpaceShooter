#ifndef GAME_HPP
#define GAME_HPP

#include "../State.hpp"

#include <vector>

class EntitySystem;
class Player;
namespace sf {
	class Text;
	class Sprite;
	class Music;
}

class Game : public State
{
public:
	Game(Application& application);
	~Game();

	void input() override;
	void update(const float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	bool m_gameOver = false;

	EntitySystem* m_p_entitySystem;
	Player* m_p_player;
	sf::Music* m_p_sceneMusic;
	sf::Text* m_p_scoreText;

	std::vector<sf::Sprite*> m_background;
};

#endif // !GAME_HPP