#include "Input.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vcruntime_string.h> // for memset()

bool* Input::_keys = nullptr;
uint* Input::_frames = nullptr;
uint Input::_current = 0;

void Input::initialize() {
	unsigned int keyCount = sf::Keyboard::KeyCount + sf::Mouse::ButtonCount;
	_keys = new bool[keyCount];
	_frames = new uint[keyCount];
	memset(_keys, false, keyCount * sizeof(*_keys));
	memset(_frames, 0, keyCount * sizeof(*_frames));
}

void Input::finalize() {
	delete[] _keys;
	delete[] _frames;
}

bool Input::pressed(int keycode) {
	if (keycode < 0 || keycode >= sf::Keyboard::KeyCount)
		return false;
	return _keys[keycode];
}

bool Input::jpressed(int keycode) {
	if (keycode < 0 || keycode >= sf::Keyboard::KeyCount)
		return false;
	return _keys[keycode] && _frames[keycode] == _current;
}

bool Input::unpressed(int keycode) {
	if (keycode < 0 || keycode >= sf::Keyboard::KeyCount)
		return false;
	return !_keys[keycode] && _frames[keycode] == _current;
}

bool Input::clicked(int button) {
	return _keys[sf::Keyboard::KeyCount + button];
}

bool Input::jclicked(int button) {
	button += sf::Keyboard::KeyCount;
	return _keys[button] && _frames[button] == _current;
}

bool Input::unclicked(int button) {
	button += sf::Keyboard::KeyCount;
	return !_keys[button] && _frames[button] == _current;
}