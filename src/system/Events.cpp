#include "Events.hpp"
#include "Input.hpp"
#include "../utils/Log.hpp"

#include <chrono>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

sf::Event* Events::s_m_p_event = nullptr;

void Events::initialize() {
	s_m_p_event = new sf::Event;
}

void Events::finalize() {
	delete s_m_p_event;
}

void Events::pollEvents(sf::RenderWindow& window) {
	Input::_current++;
	if (Input::_current >= UINT_MAX) Input::_current = 0;
	sf::Event& e = *s_m_p_event;
	while (window.pollEvent(e)) {
		if (e.type == sf::Event::Closed)
			window.close();

		if (e.type == sf::Event::KeyPressed) {
			if (e.key.code == sf::Keyboard::Unknown) {
				ERR("Unknown keyboard button pressed!")
			}
			else {
				Input::_keys[e.key.code] = true;
				Input::_frames[e.key.code] = Input::_current;
			}
		}
		else if (e.type == sf::Event::KeyReleased && e.key.code != sf::Keyboard::Unknown) {
			Input::_keys[e.key.code] = false;
			Input::_frames[e.key.code] = Input::_current;
		}
		if (e.type == sf::Event::MouseButtonPressed) {
			Input::_keys[sf::Keyboard::KeyCount + e.mouseButton.button] = true;
			Input::_frames[sf::Keyboard::KeyCount + e.mouseButton.button] = Input::_current;
		}
		else if (e.type == sf::Event::MouseButtonReleased) {
			Input::_keys[sf::Keyboard::KeyCount + e.mouseButton.button] = false;
			Input::_frames[sf::Keyboard::KeyCount + e.mouseButton.button] = Input::_current;
		}
	}
	if (Input::jpressed(sf::Keyboard::Escape)) window.close();
}

const long long Events::getTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
