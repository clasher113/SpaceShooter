#ifndef EVENTS_HPP
#define EVENTS_HPP

namespace sf {
	class Event;
	class RenderWindow;
}

class Events {
public:
	static void initialize();
	static void finalize();

	static void pollEvents(sf::RenderWindow& window);
	static const long long getTime();
private:
	static sf::Event* s_m_p_event;
};

#endif // !EVENTS_HPP
