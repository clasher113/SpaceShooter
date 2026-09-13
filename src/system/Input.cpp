#include "Input.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vcruntime_string.h> // for memset()

bool* Input::_keys = nullptr;
uint* Input::_frames = nullptr;
uint Input::_current = 0;

void Input::initialize() {
	unsigned int keyCount = sf::Keyboard::KeyCount+ sf::Mouse::ButtonCount;
	_keys = new bool[keyCount];
	_frames = new uint[keyCount];
	memset(_keys, false, keyCount * sizeof(*_keys));
	memset(_frames, 0, keyCount * sizeof(*_frames));
}

void Input::finalize() {
	delete[] _keys;
	delete[] _frames;
}

bool Input::pressed(const sf::Keyboard::Key keycode) {
	if (keycode == sf::Keyboard::Key::Unknown || static_cast<unsigned int>(keycode) >= sf::Keyboard::KeyCount)
		return false;
	return _keys[static_cast<unsigned int>(keycode)];
}

bool Input::jpressed(const sf::Keyboard::Key keycode) {
	if (keycode == sf::Keyboard::Key::Unknown || static_cast<unsigned int>(keycode) >= sf::Keyboard::KeyCount)
		return false;
	return _keys[static_cast<unsigned int>(keycode)] && _frames[static_cast<unsigned int>(keycode)] == _current;
}

bool Input::unpressed(const sf::Keyboard::Key keycode) {
	if (keycode == sf::Keyboard::Key::Unknown || static_cast<unsigned int>(keycode) >= sf::Keyboard::KeyCount)
		return false;
	return !_keys[static_cast<unsigned int>(keycode)] && _frames[static_cast<unsigned int>(keycode)] == _current;
}

bool Input::clicked(const sf::Mouse::Button button) {
	return _keys[sf::Keyboard::KeyCount + static_cast<unsigned int>(button)];
}

bool Input::jclicked(const sf::Mouse::Button button) {
	const unsigned int index = static_cast<unsigned int>(button) + sf::Keyboard::KeyCount;
	return _keys[index] && _frames[index] == _current;
}

bool Input::unclicked(const sf::Mouse::Button button) {
	const unsigned int index = static_cast<unsigned int>(button) + sf::Keyboard::KeyCount;
	return !_keys[index] && _frames[index] == _current;
}