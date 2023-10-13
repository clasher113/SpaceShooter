#include "Window.hpp"
#include "../utils/Config.hpp"
#include "../utils/Log.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include "../resource.h"
#ifdef _DEBUG
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

sf::RectangleShape windowBorder;
#endif // _DEBUG

sf::RenderWindow* Window::s_m_p_window = nullptr;
float Window::s_m_aspectRatio = 1.f;

void Window::initialize() {
	const char* TITLE = "Space Shooter";
	s_m_p_window = new sf::RenderWindow(sf::VideoMode(Config::getParameter<unsigned int>(Config::ParameterId::WINDOW_WIDTH),
													  Config::getParameter<unsigned int>(Config::ParameterId::WINDOW_HEIGHT)), TITLE, sf::Style::Close);
	s_m_p_window->setFramerateLimit(Config::getParameter<unsigned int>(Config::ParameterId::FPS_LIMIT));
	s_m_p_window->setVerticalSyncEnabled(Config::getParameter<bool>(Config::ParameterId::VERTICAL_SYNC));
	s_m_p_window->setKeyRepeatEnabled(false);
	//sf::View view(s_m_p_window->getView().getCenter(), s_m_p_window->getView().getSize() * 2.f);
	//s_m_p_window->setView(view);
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	if (hIcon) {
		SendMessage(s_m_p_window->getSystemHandle(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}
	s_m_aspectRatio = s_m_p_window->getSize().x / 1024.f;
#ifdef _DEBUG
	windowBorder.setFillColor(sf::Color::Transparent);
	windowBorder.setOutlineColor(sf::Color::Yellow);
	windowBorder.setSize(sf::Vector2f(Window::getSize()));
	windowBorder.setOutlineThickness(3.f);
	INFO("Renderer: " << glGetString(GL_RENDERER));
	INFO("OpenGL version: " << glGetString(GL_VERSION));
#endif // _DEBUG
}

void Window::finalize() {
	delete s_m_p_window;
}

void Window::clear() {
	s_m_p_window->clear();
}

void Window::display() {
#ifdef _DEBUG
	s_m_p_window->draw(windowBorder);
#endif // !_DEBUG
	s_m_p_window->display();
}

void Window::close() {
	s_m_p_window->close();
}

bool Window::isOpen() {
	return s_m_p_window->isOpen();
}

bool Window::isFocused() {
	return s_m_p_window->hasFocus();
}

sf::Vector2u Window::getSize() {
	return s_m_p_window->getSize();
}

sf::Vector2i Window::getCursorPosition() {
	return sf::Mouse::getPosition(*s_m_p_window);
}
