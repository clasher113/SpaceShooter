#ifndef GUI_FACTORY_HPP
#define GUI_FACTORY_HPP

#define GET_WINDOW_SIZE sf::Vector2f windowSize(Window::getSize());
#define GET_WINDOW_ASPECTRATIO float aspectRatio = Window::getAspectRatio();
#define GET_WINDOW_SIZE_AND_ASPECTRATIO GET_WINDOW_SIZE GET_WINDOW_ASPECTRATIO
#define WIDGET_SIZE_WIDE sf::Vector2f((windowSize.x * 82.f / 100.f) / aspectRatio, 55.f)
#define WIDGET_SIZE_NARROW sf::Vector2f((windowSize.x * 40.f / 100.f) / aspectRatio, 55.f)
#define COLON std::string(": ")
#define INTERVAL 15.f
#define DISTANCE 10.f
#define BELOW_THAN(WIDGET) WIDGET->getPosition().y + ((WIDGET->getLocalBounds().height * aspectRatio) + DISTANCE * aspectRatio)
#define ABOVE_THAN(WIDGET) WIDGET->getPosition().y - ((WIDGET->getLocalBounds().height * aspectRatio) + DISTANCE * aspectRatio)

#include "../State.hpp"
#include "../gui/SetOrigin.hpp"

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <functional>
#include <variant>

class Button;
class Slider;
class Widget;
namespace sf {
	class Drawable;
	class Texture;
	class Sprite;
	class Shader;
	class Text;
	class SoundSource;
	class Sound;
	class Music;
	class RenderWindow;
	class Clock;
	class String;
}

class GuiFactory : public State {
public:
	GuiFactory(Application& application);
	~GuiFactory();

	void input() override;
	void update(const float dt) override;
	void draw(sf::RenderWindow& window) override;
protected:
	Button* addButton(const sf::String& string, const sf::Vector2f& position, const sf::Vector2f& size, const std::function<void()>& callback, Origin origin = Origin::CENTER);
	Slider* addSlider(const sf::String& string, const sf::Vector2f& position, const sf::Vector2f& size, const std::variant<int*, unsigned int*, float*>& value, int min, int max);
	sf::Text* addText(const sf::String& string, const sf::Vector2f& position, const sf::Color& color = sf::Color::White,
					  unsigned int fontSize = 40U, Origin origin = Origin::CENTER);

	sf::Sprite* m_p_background;
	std::vector<Widget*> m_widgets;
	std::vector<sf::Drawable*> m_drawable;
private:
};

#endif // GUI_FACTORY_HPP