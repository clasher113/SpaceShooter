#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

typedef unsigned int uint;

class Input {
public:
	static void initialize();
	static void finalize();

	static bool pressed(const sf::Keyboard::Key keycode);
	static bool jpressed(const sf::Keyboard::Key keycode);
	static bool unpressed(const sf::Keyboard::Key keycode);

	static bool clicked(const sf::Mouse::Button button);
	static bool jclicked(const sf::Mouse::Button button);
	static bool unclicked(const sf::Mouse::Button button);

	static bool* _keys;
	static uint* _frames;
	static uint _current;
};

#endif // !INPUT_HPP