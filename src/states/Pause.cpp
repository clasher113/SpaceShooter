#include "Pause.hpp"
#include "GuiFactoryElements.hpp"
#include "../system/Input.hpp"
#include "../audio/AudioManager.hpp"
#include "GameMenu.hpp"
#include "Settings.hpp"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

Pause::Pause(Application& application) : GuiFactory(application),
	m_p_shader(new sf::Shader()),
	m_p_texture(new sf::Texture()),
	m_p_sprite(new sf::Sprite())
{
	GET_WINDOW_SIZE_AND_ASPECTRATIO
	AudioManager::setStatus(Status::PAUSE, ApplyTo::SOUNDS);
	m_p_shader->loadFromFile("blur.frag", sf::Shader::Type::Fragment);
	m_p_shader->setUniform("blur_radius", 0.003f);
	m_p_texture->create(static_cast<unsigned int>(windowSize.x), static_cast<unsigned int>(windowSize.y));
	m_p_texture->update(Window::getWindow());
	m_p_sprite->setTexture(*m_p_texture);
	m_p_background->setScale(0.f, 0.f);
	addText(m_p_assets->getString("gui.string.pause"), sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f),
								  sf::Color::White, 50U);
	Widget* widget = nullptr;
	widget = addButton(m_p_assets->getString("gui.button.leftToMenu"), sf::Vector2f(windowSize.x * 71.f / 100.f, windowSize.y * 95.f / 100.f),
			  WIDGET_SIZE_NARROW, [&app = *m_p_application](){ AudioManager::setStatus(Status::STOP, ApplyTo::ALL); app.popState(); app.changeState<GameMenu>(); });
	addButton(m_p_assets->getString("gui.button.settings"), sf::Vector2f(windowSize.x * 29.f / 100.f, windowSize.y * 95.f / 100.f),
			  WIDGET_SIZE_NARROW, [&app = *m_p_application](){ app.pushState<Settings>(); });
	addButton(m_p_assets->getString("gui.button.resume"), sf::Vector2f(windowSize.x * 71.f / 100.f, ABOVE_THAN(widget)),
			  WIDGET_SIZE_NARROW, [&app = *m_p_application](){ app.popState(true); });
	addButton(m_p_assets->getString("gui.button.exit"), sf::Vector2f(windowSize.x * 29.f / 100.f, ABOVE_THAN(widget)),
			  WIDGET_SIZE_NARROW, [](){ Window::close(); });
#ifndef _DEBUG
	Window::getWindow().setMouseCursorVisible(true);
#endif // _DEBUG
}

Pause::~Pause() {
	delete m_p_shader;
	delete m_p_texture;
	delete m_p_sprite;
	AudioManager::setStatus(Status::RESUME, ApplyTo::SOUNDS);
#ifndef _DEBUG
	Window::getWindow().setMouseCursorVisible(false);
#endif // !_DEBUG
}

void Pause::input() {
	GuiFactory::input();
	if (Input::jpressed(sf::Keyboard::Enter)) {
		m_p_application->popState(true);
	}
}

void Pause::draw(sf::RenderWindow& window) {
	window.draw(*m_p_sprite, m_p_shader);
	GuiFactory::draw(window);
}
