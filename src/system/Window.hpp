#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/System/Vector2.hpp>

namespace sf {
	class RenderWindow;
}

class Window {
public:
	static void initialize();
	static void finalize();
	
	static void clear();
	static void display();
	static void close();

	static bool isOpen();
	static bool isFocused();
	static float getAspectRatio() { return s_m_aspectRatio; }
	static sf::Vector2u getSize();
	static sf::RenderWindow& getWindow() { return *s_m_p_window; }
	static sf::Vector2i getCursorPosition();
private:
	static sf::RenderWindow* s_m_p_window;
	static float s_m_aspectRatio;
};

#endif // !WINDOW_HPP