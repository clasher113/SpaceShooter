#ifndef EFFECT_COMPONENT_HPP
#define EFFECT_COMPONENT_HPP

#include "IComponent.hpp"

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include <unordered_map>

struct ShootingData;
class Assets;
class EntitySystem;
class Entity;
class Effect;
class IEntity;
namespace sf {
	class Sound;
}

enum class EffectState {
	UNDEFINED = 0,
	ON_DEPLOY,
	ON_DEATH,
	ON_SHOOT,
};

struct EffectData {
	bool m_loopSound = false;
	bool m_loopAnimation = false;
	bool m_pinToParent = false;
	std::vector<std::string> m_sounds;
	sf::Sound* lastSound = nullptr;
	Effect* lastEffect = nullptr;
	std::string m_effectName;
	sf::Vector2f m_positionOffset{ 0.f, 0.f };
};

class EffectComponent : public IComponent {
public:
	EffectComponent(Assets* assets, EntitySystem* entitySystem, std::unordered_multimap<EffectState, EffectData*>* effectData, Entity* entity);
	~EffectComponent() override;

	void input() override {};
	void update(const float dt) override {};

	Effect* getLastEffect() const { return m_p_lastEffect; }
	Effect* getLastEffect(EffectState effectState) const;

	void onDeathEffect();
	void onShootEffect(ShootingData* shootingData);
private:
	Assets* m_p_assets;
	Entity* m_p_entity;
	EntitySystem* m_p_entitySystem;
	Effect* m_p_lastEffect;
	sf::Sound* m_p_lastSound;

	std::unordered_multimap<EffectState, EffectData*>* m_p_effectData;

	void processEffect(EffectState effectState, int index = -1);
};

#endif // !EFFECT_COMPONENT_HPP