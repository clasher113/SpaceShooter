#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <unordered_set>

class Assets;
namespace sf {
	class SoundSource;
	class Sound;
	class Music;
}

enum class ApplyTo {
	ALL = 0,
	MUSICS,
	SOUNDS
};

enum class Status {
	STOP = 0,
	PAUSE,
	RESUME
};

class AudioManager {
public:
	static void initialize(Assets* assets);
	static void finalize();
	static sf::Sound* playSound(const char* soundName, bool loop = false);
	static sf::Music* playMusic(const char* musicName, bool loop = false);
	static bool setStatus(sf::Sound* sound, Status status);
	static bool setStatus(sf::Music* music, Status status);
	static bool setStatus(sf::SoundSource* source, Status status);
	static void setStatus(Status status, ApplyTo toStop = ApplyTo::ALL);
	static void setVolume(ApplyTo applyTo, float volume);
	static void clear();
private:
	static float* s_m_p_musicVolume;
	static float* s_m_p_soundVolume;
	static std::unordered_set<sf::SoundSource*> s_m_sources;
	static Assets* s_m_p_assets;
};

#endif // !AUDIO_MANAGER_HPP