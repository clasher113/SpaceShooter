#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#ifdef _DEBUG
#include "utils/DebugInfo.hpp"
#endif // _DEBUG

#include <memory>
#include <stack>
#include <functional>
#include "State.hpp"

class Assets;
class Game;
namespace sf {
	class RectangleShape;
}

class Application
{
public:
	Application();
	~Application();

	bool initialize();

	void runGameLoop();
	template<typename T>
	inline bool pushState(bool skipAnimation = false);
	bool popState(bool skipAnimation = false);
	template<typename T>
	inline bool changeState(bool skipAnimation = false);

	Assets* const& getAssets() const { return m_p_assets; }

private:
	bool m_isAdding;
	bool m_inAnimation;
	bool m_skipAnimation;
	unsigned int m_isRemoving;
	float m_fadeAnimationSpeed;
	float m_fadeScreenOpacity;

	Assets* m_p_assets;

	sf::RectangleShape* m_p_fadeScreen;
	
	std::function<void()> m_newState;
	std::stack<std::unique_ptr<State>> m_states;

	void processStateChanges(const float dt);
#ifdef DEBUG_OVERLAY
	void drawDebugOverlay(sf::RenderWindow& window, Assets* assets);
#endif // DEBUG_OVERLAY
};

template<typename T>
inline bool Application::pushState(bool skipAnimation) {
	if (m_inAnimation) return false;
	m_newState = [this]() { m_states.push(std::make_unique<T>(*this)); };
	m_skipAnimation = skipAnimation;
	m_isAdding = true;
	return true;
}

template<typename T>
inline bool Application::changeState(bool skipAnimation) {
	popState();
	return pushState<T>(skipAnimation);
}

#endif // !APPLICATION_HPP