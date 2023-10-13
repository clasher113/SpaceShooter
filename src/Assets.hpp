#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <memory>
#include <unordered_map>
#include <string>

namespace sf {
	class Texture;
	class Font;
	class SoundBuffer;
	class Music;
}

class Assets {
public:
	Assets();
	~Assets();

	const sf::Texture& getTexture(const std::string& name) const;
	void store(const std::string& name, sf::Texture* texture);

	const sf::SoundBuffer* getSoundBuffer(const std::string& name) const;
	void store(const std::string& name, sf::SoundBuffer* soundBuffer);

	const sf::Font* getFont(const std::string& name) const;
	void store(const std::string& name, sf::Font* font);

	sf::Music* getMusic(const std::string& name) const;
	void store(const std::string& name, sf::Music* music);

	const char* getString(const std::string& name) const;
	void store(const std::string& name, std::string* string);
private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
	std::unordered_map<std::string, std::shared_ptr<sf::Font>> m_fonts;
	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> m_soundBuffers;
	std::unordered_map<std::string, std::shared_ptr<sf::Music>> m_musics;
	std::unordered_map<std::string, std::shared_ptr<std::string>> m_strings;
};

#endif // !ASSETS_HPP