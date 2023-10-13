#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

#include "IComponent.hpp"

namespace sf {
	class Sprite;
}
class EntityInfo;

struct AnimationData {
	float m_speed = 10.f;
	int m_columns = 1; 
	int m_rows = 1;
	int m_frames = 1;
};

class AnimationComponent : public IComponent{
public:
	AnimationComponent(sf::Sprite* sprite, AnimationData* animationData);

	void input() override {};
	void update(const float dt) override;

	void setOffset(float frameOffset) { m_frameOffset = frameOffset; }

	unsigned int getCycle() { return m_cycle; }
private:
	float m_currentFrame;
	int m_lastFrame;
	float m_frameOffset;
	int m_width, m_height;
	unsigned int m_cycle;

	AnimationData* m_p_animationData;
	sf::Sprite* m_p_sprite;
};

#endif // !ANIMATION_COMPONENT_HPP