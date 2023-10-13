#include "Application.hpp"
#include "EngineInitializer.hpp"
#include "Assets.hpp"
#include "system/Window.hpp"
#include "system/Events.hpp"
#include "system/Input.hpp"
#ifdef _DEBUG
#include "states/Game.hpp"
#include "states/GameOver.hpp"
#include "states/Settings.hpp"
#ifdef DEBUG_OVERLAY
#include <SFML/Graphics/Text.hpp>
#include <unordered_map>
#include <variant>
#include "utils/DebugInfo.hpp"
#include "entities/EntitySystem.hpp"
void countAvarageFPS();
#endif // DEBUG_OVERLAY
#endif // _DEBUG
#include "states/GameMenu.hpp"
#include "audio/AudioManager.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

Application::Application() :
	m_p_assets(new Assets()),
	m_isAdding(false),
	m_inAnimation(false),
	m_skipAnimation(false),
	m_isRemoving(0),
	m_fadeAnimationSpeed(610.f),
	m_fadeScreenOpacity(255.f),
	m_p_fadeScreen(nullptr)
{
}

Application::~Application() {
	Window::finalize();
	Events::finalize();
	Input::finalize();
	AudioManager::finalize();
	while (!m_states.empty()) {
		m_states.pop();
	}
	delete m_p_fadeScreen;
	delete m_p_assets;
}

bool Application::initialize() {
	if (!initializeEngine(m_p_assets)) {
		return 0;
	}
	Window::initialize();
	Events::initialize();
	Input::initialize();
	AudioManager::initialize(m_p_assets);
#ifdef _DEBUG
	m_states.push(std::make_unique<Game>(*this));
#else 
	m_states.push(std::make_unique<GameMenu>(*this));
#endif // _DEBUG
	m_p_fadeScreen = new sf::RectangleShape(sf::Vector2f(Window::getSize()));
	m_p_fadeScreen->setFillColor(sf::Color::Black);
	return 1;
}

void Application::runGameLoop() {
	sf::Clock deltaClock;

	while (Window::isOpen() && !m_states.empty()) {
		sf::Time delta = deltaClock.restart();
		Events::pollEvents(Window::getWindow());
		if (Window::isFocused()) m_states.top()->input();
		m_states.top()->update(delta.asSeconds());

		Window::clear();
		m_states.top()->draw(Window::getWindow());
		Window::getWindow().draw(*m_p_fadeScreen);
#ifdef DEBUG_OVERLAY
		countAvarageFPS();
		drawDebugOverlay(Window::getWindow(), m_p_assets);
#endif // DEBUG_OVERLAY

		Window::display();

		processStateChanges(delta.asSeconds());
		AudioManager::clear();
	}
}

bool Application::popState(bool skipAnimation) {
	if (m_inAnimation) return false;
	m_isRemoving++;
	m_skipAnimation = skipAnimation;
	return true;
}

void Application::processStateChanges(const float dt) {
	if (m_isRemoving > 0 || m_isAdding) {
		m_inAnimation = true;
		if (m_fadeScreenOpacity < 255.f) {
			m_fadeScreenOpacity += m_fadeAnimationSpeed * dt;
			if (m_fadeScreenOpacity > 255.f || m_skipAnimation) {
				if (!m_skipAnimation) m_fadeScreenOpacity = 255.f;
				else {
					m_inAnimation = false;
					m_skipAnimation = false;
				}
				while (m_isRemoving > 0) {
					if (!m_states.empty()) m_states.pop();
					m_isRemoving--;
				}
				if (m_isAdding) {
					m_newState();
					m_isAdding = false;
				}
			}
			sf::Color color = m_p_fadeScreen->getFillColor();
			m_p_fadeScreen->setFillColor(sf::Color(color.r, color.g, color.b, static_cast<unsigned int>(m_fadeScreenOpacity)));
		}
	}
	else if (!m_isAdding && m_isRemoving == 0){
		if (m_fadeScreenOpacity > 0.f) {
			m_fadeScreenOpacity -= m_fadeAnimationSpeed * dt;
			if (m_skipAnimation) m_fadeScreenOpacity = 0.f;
			if (m_fadeScreenOpacity < 0.f) {
				m_fadeScreenOpacity = 0.f;
				m_inAnimation = false;
			}
			sf::Color color = m_p_fadeScreen->getFillColor();
			m_p_fadeScreen->setFillColor(sf::Color(color.r, color.g, color.b, static_cast<unsigned int>(m_fadeScreenOpacity)));
		}
	}
}

#ifdef DEBUG_OVERLAY
void countAvarageFPS() {
	static long long updateSpeed = 1000;
	static long long lastUpdate = 0;
	static float currentFrame = 0;

	currentFrame++;
	
	long long ms = Events::getTime();

	if (ms - lastUpdate >= updateSpeed) {
		lastUpdate = ms;
		DebugInfo::FPS = currentFrame / updateSpeed * 1000;
		currentFrame = 0;
	}
}

void Application::drawDebugOverlay(sf::RenderWindow& window, Assets* assets) {
	std::unordered_map<std::string, std::variant<size_t, unsigned int, int, float>> variables;
#define ADD(NAME, VARIABLE) variables[NAME] = VARIABLE;
	using namespace DebugInfo;
	ADD("FPS", DebugInfo::FPS);
	ADD("Entities", entities);
	ADD("Musics", musics);
	ADD("Sounds", sounds);
	ADD("Spawn speed", EntitySpawner::s_spawnSpeed);
	//ADD("Upgrades spawn speed", UpgradeSpawner::s_spawnSpeed);
	sf::Text text;
	text.setFont(*assets->getFont("ArialBlack"));
	text.setScale(0.5f, 0.5f);
	text.setLineSpacing(3.f);
	float posY = 50.f;
	for (const auto& it : variables) {
		text.setString(it.first + std::string(": ") + std::visit([](auto& value) { return  std::to_string(value); }, it.second));
		text.setPosition(text.getPosition().x, posY += text.getGlobalBounds().height + text.getLineSpacing());
		window.draw(text);
	}
#undef ADD
}
#endif // DEBUG_OVERLAY