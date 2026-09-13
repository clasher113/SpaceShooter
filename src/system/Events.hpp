#ifndef EVENTS_HPP
#define EVENTS_HPP

namespace sf {
	class RenderWindow;
}

class Events {
public:
	static void pollEvents(sf::RenderWindow& window);
	static const long long getTime();
};

#endif // !EVENTS_HPP
