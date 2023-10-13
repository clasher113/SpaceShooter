#ifndef PAUSE_HPP
#define PAUSE_HPP

#include "GuiFactory.hpp"

class Pause : public GuiFactory {
public:
	Pause(Application& application);
	~Pause();

	void input() override;
	void draw(sf::RenderWindow& window) override;
private:
	sf::Shader* m_p_shader;
	sf::Texture* m_p_texture;
	sf::Sprite* m_p_sprite;
};

#endif // !PAUSE_HPP