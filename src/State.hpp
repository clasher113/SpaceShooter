#ifndef STATE_HPP
#define STATE_HPP

class Application;
class Assets;
namespace sf {
	class RenderWindow;
}

class State
{
public:
	State(Application& application);
	virtual ~State();

	virtual void input() = 0;
	virtual void update(const float dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

protected:
	Application* m_p_application;
	Assets* m_p_assets;
};

#endif // !STATE_HPP