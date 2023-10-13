#include "AudioManager.hpp"
#include "../Assets.hpp"
#include "../utils/Config.hpp"
#ifdef _DEBUG
#include "../utils/DebugInfo.hpp"
#endif // _DEBUG

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio.hpp>

float* AudioManager::s_m_p_musicVolume = nullptr;
float* AudioManager::s_m_p_soundVolume = nullptr;
std::unordered_set<sf::SoundSource*> AudioManager::s_m_sources;
Assets* AudioManager::s_m_p_assets = nullptr;

void AudioManager::initialize(Assets* assets) {
	s_m_p_assets = assets;
	s_m_p_musicVolume = Config::getParameterPtr<float>(Config::ParameterId::MUSIC_VOLUME);
	s_m_p_soundVolume = Config::getParameterPtr<float>(Config::ParameterId::SOUND_VOLUME);
	sf::Listener::setGlobalVolume(Config::getParameter<float>(Config::ParameterId::MASTER_VOLUME));
}

void AudioManager::finalize() {
	for (const auto& it : s_m_sources) {
		if (typeid(*it) == typeid(sf::Sound)) delete it;
	}
	s_m_sources.clear();
}

sf::Sound* AudioManager::playSound(const char* soundName, bool loop) {
	const sf::SoundBuffer* soundBuffer(s_m_p_assets->getSoundBuffer(soundName));
	if (soundBuffer == nullptr) return nullptr;
	sf::Sound* sound = new sf::Sound(*soundBuffer);
	sound->setVolume(*s_m_p_soundVolume);
	sound->setLoop(loop);
	sound->play();
	s_m_sources.insert(sound);
	return sound;
}

sf::Music* AudioManager::playMusic(const char* musicName, bool loop) {
	sf::Music* music = s_m_p_assets->getMusic(musicName);
	if (music == nullptr) return nullptr;
	music->setVolume(*s_m_p_musicVolume);
	music->setLoop(loop);
	music->play();
	s_m_sources.insert(music);
	return music;
}

bool AudioManager::setStatus(sf::Sound* sound, Status status) {
	return setStatus(static_cast<sf::SoundSource*>(sound), status);
}

bool AudioManager::setStatus(sf::Music* music, Status status) {
	return setStatus(static_cast<sf::SoundSource*>(music), status);
}

bool AudioManager::setStatus(sf::SoundSource* source, Status status) {
	const auto& it = s_m_sources.find(source);
	if (it == s_m_sources.end()) return false;
	switch (status) {
		case Status::STOP:
			source->stop();
			break;
		case Status::PAUSE:
			source->pause();
			break;
		case Status::RESUME:
			source->play();
			break;
	}
	return true;
}

void AudioManager::setStatus(Status status, ApplyTo toStop) {
	for (const auto& it : s_m_sources) {
		if (toStop != ApplyTo::ALL) {
			if (toStop == ApplyTo::MUSICS && typeid(*it) != typeid(sf::Music)) continue;
			if (toStop == ApplyTo::SOUNDS && typeid(*it) != typeid(sf::Sound)) continue;
		}
		setStatus(it, status);
	}
}

void AudioManager::setVolume(ApplyTo applyTo, float volume) {
	if (applyTo == ApplyTo::ALL || applyTo == ApplyTo::MUSICS) {
		for (const auto& it : s_m_sources) {
			if (applyTo != ApplyTo::ALL) {
				if (applyTo == ApplyTo::MUSICS && typeid(*it) != typeid(sf::Music)) continue;
				if (applyTo == ApplyTo::SOUNDS && typeid(*it) != typeid(sf::Sound)) continue;
			}
			it->setVolume(volume);
		}
	}
}

void AudioManager::clear() {
	std::erase_if(s_m_sources, [](sf::SoundSource* source) {
		if (source->getStatus() == sf::SoundSource::Status::Stopped) {
			if (typeid(*source) == typeid(sf::Sound)) delete source;
			return true;
		}
		return false;
	});
#ifdef _DEBUG
	DebugInfo::sounds = DebugInfo::musics = 0;
	for (const auto& it : s_m_sources) {
		if (typeid(*it) == typeid(sf::Music)) DebugInfo::musics++;
		if (typeid(*it) == typeid(sf::Sound)) DebugInfo::sounds++;
	}
#endif // _DEBUG
}