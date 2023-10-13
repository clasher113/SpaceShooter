#ifndef CONFIG_HPP
#define CONFIG_HPP

#define CONFIG_TYPES bool, int, unsigned int, float

#include <unordered_map>
#include <variant>

typedef std::variant<CONFIG_TYPES> ConfigTypes;

class Config {
public:
	enum class ParameterId {
		VERTICAL_SYNC = 0,
		MASTER_VOLUME,
		MUSIC_VOLUME,
		SOUND_VOLUME,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		FPS_LIMIT
	};
	static void applyConfig();
	static void readConfig();
	static void writeConfig();
	template<typename T>
	static T* getParameterPtr(ParameterId parameter) {
		const auto& it = s_m_config.find(parameter);
		if (it == s_m_config.end()) return nullptr;
		return reinterpret_cast<T*>(&it->second.begin()->second);
	};
	template<typename T>
	static T getParameter(ParameterId parameter) {
		const auto& it = s_m_config.find(parameter);
		if (it == s_m_config.end()) return NULL;
		return std::visit([](const auto& value) { return static_cast<T>(value); }, it->second.begin()->second);
	};
	static const type_info& getParameterType(ParameterId parameter);
	template<typename T>
	static bool setParameter(ParameterId parameter, T value) {
		if (s_m_config.find(parameter) == s_m_config.end()) return false;
		s_m_config[parameter].begin()->second = value;
		return true;
	};
private:
	static std::unordered_map<ParameterId, std::unordered_map<const char*, ConfigTypes>> s_m_config;
	static void resetConfig();
};

#endif // !CONFIG_HPP