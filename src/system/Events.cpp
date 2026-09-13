#include "Events.hpp"
#include "Input.hpp"
#include "../utils/Log.hpp"

#include <chrono>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

void Events::pollEvents(sf::RenderWindow& window) {
	Input::_current++;
	if (Input::_current >= UINT_MAX) Input::_current = 0;

	while (auto event = window.pollEvent()) {
		if (event->getIf<sf::Event::Closed>()) {
			window.close();
			break;
		}
		else if (const sf::Event::KeyPressed* key = event->getIf<sf::Event::KeyPressed>()) {
			if (key->code == sf::Keyboard::Key::Unknown) {
				ERR("Unknown keyboard button pressed!")
			}
			else {
				Input::_keys[static_cast<unsigned int>(key->code)] = true;
				Input::_frames[static_cast<unsigned int>(key->code)] = Input::_current;
			}
		}
		else if (const sf::Event::KeyReleased* key = event->getIf<sf::Event::KeyReleased>()) {
			if (key->code != sf::Keyboard::Key::Unknown) {
				Input::_keys[static_cast<unsigned int>(key->code)] = false;
				Input::_frames[static_cast<unsigned int>(key->code)] = Input::_current;
			}
		}
		else if (const sf::Event::MouseButtonPressed* button = event->getIf<sf::Event::MouseButtonPressed>()) {
			Input::_keys[sf::Keyboard::KeyCount + static_cast<int>(button->button)] = true;
			Input::_frames[sf::Keyboard::KeyCount + static_cast<int>(button->button)] = Input::_current;
		}
		else if (const sf::Event::MouseButtonReleased* button = event->getIf<sf::Event::MouseButtonReleased>()) {
			Input::_keys[sf::Keyboard::KeyCount + static_cast<int>(button->button)] = false;
			Input::_frames[sf::Keyboard::KeyCount + static_cast<int>(button->button)] = Input::_current;
		}
	}
	if (Input::jpressed(sf::Keyboard::Key::Escape)) window.close();
}

const long long Events::getTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
