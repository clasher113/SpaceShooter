#ifndef EFFECT_HPP
#define EFFECT_HPP

#include "Entity.hpp"

#include <string>

namespace sf {
	class RenderWindow;
	class Vector;
	typedef Vector2<float>Vector2f;
}
struct EffectComponentData;
struct EffectInfo;
class AnimationComponent;;

class Effect : public Entity {
public:
	Effect(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem);
	~Effect() override;

	void update(const float dt) override;

	void pinTo(const sf::Vector2f& position);
	void setOffset(const sf::Vector2f& offset);
	void setLoop(bool loop);
private:
	bool m_loopAnimation;
	sf::Vector2f* m_p_position;
	sf::Vector2f m_offset;

	EffectInfo* m_p_effectInfo;
};

#endif // !EFFECT_HPP