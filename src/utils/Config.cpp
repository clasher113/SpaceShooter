#include "Config.hpp"
#include "../system/Window.hpp"
#include "../audio/AudioManager.hpp"
#include "../utils/Log.hpp"

#include <SFML/Audio/Listener.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

std::unordered_map<Config::ParameterId, std::unordered_map<const char*, ConfigTypes>> Config::s_m_config;

const std::string configFilePath = "config.ini";
constexpr char delimiter[] = ": ";

template<typename T>
bool parse(const std::string& from, T& to) {
	unsigned int errors = 0;
	std::ifstream file(from);
	if (file.is_open()) {
		std::string name;
		while (file >> name) {
			size_t pos = name.find(delimiter[0]);
			if (name.size() + strlen(delimiter) < pos) {
				ERR("Error parsing value: " << name << ". Ignoring");
				errors++;
				file.ignore();
				continue;
			}
			name.erase(pos);
			std::string value;
			std::getline(file, value);
			for (auto& it : to) {
				if (it.second.begin()->first == name) {
					const auto& map = it.second.begin();
					std::stringstream ss;
					ss << value.substr(value.find(delimiter) + strlen(delimiter));
					try {
						int a = std::stoi(ss.str());
						std::visit([&ss](auto& value) { ss >> value; }, map->second);
					}
					catch (const std::exception&) {
						ERR("Error converting value: " << value << ". Ignoring");
						errors++;
					}
					break;
				}
			}
		}
		if (errors == 0) {
			INFO(from << " parsing success");
		}
		else ERR("parsing " << from << " " << std::to_string(errors) << " errors!");
		file.close();
		return true;
	}
	file.close();
	return false;
}

template<typename T>
bool write(T& from, const std::string& to) {
	std::ofstream configFile;
	configFile.open(to, std::ofstream::binary);
	if (configFile.is_open()) {
		for (const auto& it : from) {
			const auto& map = it.second.begin();
			std::visit([&configFile, map](const auto& value) { configFile << map->first << delimiter << value << std::endl; }, map->second);
		}
		configFile.close();
		return true;
	}
	return false;
}

void Config::applyConfig() {
	Window::getWindow().setFramerateLimit(getParameter<unsigned int>(ParameterId::FPS_LIMIT));
	Window::getWindow().setSize(sf::Vector2u(getParameter<unsigned int>(ParameterId::WINDOW_WIDTH), 
											 getParameter<unsigned int>(ParameterId::WINDOW_HEIGHT)));
	Window::getWindow().setVerticalSyncEnabled(getParameter<bool>(ParameterId::VERTICAL_SYNC));
	sf::Listener::setGlobalVolume(getParameter<float>(ParameterId::MASTER_VOLUME));
	AudioManager::setVolume(ApplyTo::MUSICS, getParameter<float>(ParameterId::MUSIC_VOLUME));
	AudioManager::setVolume(ApplyTo::SOUNDS, getParameter<float>(ParameterId::SOUND_VOLUME));
	INFO("Config Applied");
}

void Config::readConfig() {
	resetConfig();
	if (!parse(configFilePath, s_m_config)) {
		ERR("Error reading config file! Writing default config.");
		writeConfig();
	}
}

void Config::writeConfig() {
	write(s_m_config, configFilePath);
	INFO("Config Written");
}

const type_info& Config::getParameterType(ParameterId parameter) {
	const auto& it = s_m_config.find(parameter);
	if (it == s_m_config.end()) return typeid(void);
	return std::visit([](auto&& x)->decltype(auto) { return typeid(x); }, it->second.begin()->second);
}

void Config::resetConfig() {
	s_m_config.clear();
#define ADD(ID, NAME, VALUE) s_m_config.emplace(ParameterId::ID, std::unordered_map<const char*, ConfigTypes>({std::make_pair(NAME, VALUE)}))
	ADD(VERTICAL_SYNC, "VerticalSync", true);
	ADD(MASTER_VOLUME, "MasterVolume", 50.f);
	ADD(MUSIC_VOLUME, "MusicVolume", 50.f);
	ADD(SOUND_VOLUME, "SoundVolume", 50.f);
	ADD(WINDOW_WIDTH, "WindowWidth", 600U);
	ADD(WINDOW_HEIGHT, "WindowHeight", 800U);
	ADD(FPS_LIMIT, "FPS", 60U);
#undef ADD
}