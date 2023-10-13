#ifndef SHOOTING_COMPONENT_HPP
#define SHOOTING_COMPONENT_HPP

#include "IComponent.hpp"

#include <string>
#include <SFML/System/Vector2.hpp>

class EntitySystem;
class Entity;
struct MovementData;

enum class ShootingMode {
	UNKNOWN = 0,
	AUTOMATIC,
	MANUAL
};

struct ShootingData {
	bool m_targetPlayer = false;
	float m_shootInterval = 1.f;
	float m_queueInterval = 0.f;
	int m_key = -1;
	unsigned int m_queueSize = 1;
	ShootingMode m_shootingMode = ShootingMode::UNKNOWN;
	sf::Vector2f m_positionOffset {0.f, 0.f};
	std::string m_bulletName;
	MovementData* m_p_movementData = nullptr;
};

class ShootingComponent : public IComponent {
public:
	ShootingComponent(EntitySystem* entitySystem, ShootingData* shootingData, Entity* parent);
	~ShootingComponent() override;

	void input() override;
	void update(const float dt) override;

private:
	bool m_fire;
	float m_shootTimer;
	float m_queueTimer;
	unsigned int m_queueCounter;

	ShootingData* m_p_shootingData;
	Entity* m_p_parent;
	EntitySystem* m_p_entitySystem;
};

#endif // !SHOOTING_COMPONENT_HPP
