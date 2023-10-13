#include "Settings.hpp"
#include "GuiFactoryElements.hpp"
#include "../utils/Config.hpp"

Settings::Settings(Application& application) : GuiFactory(application) {
	GET_WINDOW_SIZE_AND_ASPECTRATIO
	Widget* widget = nullptr;
	addText(m_p_assets->getString("gui.string.gameSettings"), sf::Vector2f(windowSize.x / 2.f, windowSize.y * 4.f / 100.f));
	addButton(m_p_assets->getString("gui.button.leftToMenu"), sf::Vector2f(windowSize.x * 71.f / 100.f, windowSize.y * 95.f / 100.f),
			  WIDGET_SIZE_NARROW, [&app = *m_p_application]() { app.popState(); });
	widget = addButton(m_p_assets->getString("gui.button.save"), sf::Vector2f(windowSize.x * 29.f / 100.f, windowSize.y * 95.f / 100.f),
			  WIDGET_SIZE_NARROW, []() { Config::writeConfig(); });
	addButton(m_p_assets->getString("gui.button.apply"),
			  sf::Vector2f(windowSize.x * 29.f / 100.f, ABOVE_THAN(widget)),
			  WIDGET_SIZE_NARROW, [](){Config::applyConfig(); });
	widget = addSlider(m_p_assets->getString("gui.slider.masterVolume") + COLON, sf::Vector2f(windowSize.x / 2.f, windowSize.y * 10.f / 100.f),
			 WIDGET_SIZE_WIDE, Config::getParameterPtr<float>(Config::ParameterId::MASTER_VOLUME), 0, 100);
	addSlider(m_p_assets->getString("gui.slider.effectVolume") + COLON, sf::Vector2f(windowSize.x * 29.f / 100.f, BELOW_THAN(widget)),
			  WIDGET_SIZE_NARROW, Config::getParameterPtr<float>(Config::ParameterId::SOUND_VOLUME), 0, 100);
	widget = addSlider(m_p_assets->getString("gui.slider.musicVolume") + COLON, sf::Vector2f(windowSize.x * 71.f / 100.f, BELOW_THAN(widget)),
			  WIDGET_SIZE_NARROW, Config::getParameterPtr<float>(Config::ParameterId::MUSIC_VOLUME), 0, 100);
	widget = addSlider(m_p_assets->getString("gui.slider.fps") + COLON, sf::Vector2f(windowSize.x / 2.f, BELOW_THAN(widget)),
			  WIDGET_SIZE_WIDE,
			  Config::getParameterPtr<unsigned int>(Config::ParameterId::FPS_LIMIT), 10, 500);
	size_t size = m_widgets.size();
	const char* verticalSync = m_p_assets->getString("gui.button.verticalSync");
	const char* enabled = m_p_assets->getString("gui.button.enabled");
	const char* disabled = m_p_assets->getString("gui.button.disabled");
	bool* vSync = Config::getParameterPtr<bool>(Config::ParameterId::VERTICAL_SYNC);
	addButton(verticalSync + COLON + (*vSync ? std::string(enabled) : std::string(disabled)), 
			 sf::Vector2f(windowSize.x / 2.f, BELOW_THAN(widget)), WIDGET_SIZE_WIDE,
			 [this, size, verticalSync, enabled, disabled, vSync]() { 
				Button* buttom = static_cast<Button*>(m_widgets[size]);
				if (*vSync) {
					buttom->setString(verticalSync + COLON + std::string(disabled));
					*vSync = false;
				}
				else {
					buttom->setString(verticalSync + COLON + std::string(enabled));
					*vSync = true;
				} 
			 });
}

Settings::~Settings() {
}