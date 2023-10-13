#include "Assets.hpp"
#include "utils/Log.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>

Assets::Assets() {
	sf::Image image;
	image.create(64, 64, sf::Color(228, 0, 255));
	sf::Texture* texture_fail = new sf::Texture();
	texture_fail->loadFromImage(image);
	image.create(32, 32, sf::Color::Black);
	texture_fail->update(image.getPixelsPtr(), 32, 32, 32, 0);
	texture_fail->update(image.getPixelsPtr(), 32, 32, 0, 32);
	store("texture_fail", texture_fail);
}

Assets::~Assets() {
	m_textures.clear();
	m_soundBuffers.clear();
	m_musics.clear();
	m_fonts.clear();
	m_strings.clear();
}

const sf::Texture& Assets::getTexture(const std::string& name) const {
	const auto& it = m_textures.find(name);
	if (it != m_textures.end()) {
		return *it->second;
	}
	ERR("Can't find texture: \"" << name << "\"");
	return *m_textures.at("texture_fail");
}

void Assets::store(const std::string& name, sf::Texture* texture) {
	m_textures.emplace(name, texture);
}

const sf::SoundBuffer* Assets::getSoundBuffer(const std::string& name) const {
	const auto& it = m_soundBuffers.find(name);
	if (it != m_soundBuffers.end()) {
		return it->second.get();
	}
	ERR("Can't find sound: \"" << name << "\"");
	return nullptr;
}

void Assets::store(const std::string& name, sf::SoundBuffer* soundBuffer) {
	m_soundBuffers.emplace(name, soundBuffer);
}

const sf::Font* Assets::getFont(const std::string& name) const {
	const auto& it = m_fonts.find(name);
	if (it != m_fonts.end()) {
		return it->second.get();
	}
	ERR("Can't find font: \"" << name << "\"");
	return nullptr;
}

void Assets::store(const std::string& name, sf::Font* font) {
	m_fonts.emplace(name, font);
}

sf::Music* Assets::getMusic(const std::string& name) const {
	const auto& it = m_musics.find(name);
	if (it != m_musics.end()) {
		return it->second.get();
	}
	ERR("Can't find music: \"" << name << "\"");
	return nullptr;
}

void Assets::store(const std::string& name, sf::Music* music) {
	m_musics.emplace(name, music);
}

const char* Assets::getString(const std::string& name) const {
	const auto& it = m_strings.find(name);
	if (it != m_strings.end()) {
		return it->second.get()->c_str();
	}
	ERR("Can't find string: \"" << name << "\"");
	return name.c_str();
}

void Assets::store(const std::string& name, std::string* string) {
	m_strings.emplace(name, string);
}