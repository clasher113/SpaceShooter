#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

class Assets;
namespace sf {
	class RectangleShape;
	class Shape;
	class Text;
	class String;
}
enum class Origin;

class Widget : public sf::Drawable {
public:
	enum class State {
		IDLE = 0,
		HOVER,
		PRESSED
	};
	Widget();
	virtual ~Widget();

	virtual void input(const sf::Vector2f& cursorPos);
	virtual void update(const float dt) = 0;

	virtual void setPosition(const sf::Vector2f& position);
	virtual void setSize(const sf::Vector2f& size);
	virtual void setOrigin(Origin origin);
	virtual void setString(const sf::String& string) = 0;

	virtual const sf::Vector2f& getSize();
	virtual const sf::Vector2f getPosition();
	virtual const sf::FloatRect getLocalBounds();

	State getState() const { return m_currentState; };

protected:
	State m_currentState, m_lastState;
	Origin m_origin;

	sf::RectangleShape* m_p_shape;
};

#endif // !WIDGET_HPP
