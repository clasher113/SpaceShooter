#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

struct SpaceShipInfo;
class ProgressBar;

class SpaceShip : public Entity {
public:
	SpaceShip(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem);
	~SpaceShip() override;

	void update(const float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void draw(Layers& layers) const override;

	void giveDamage(int damage) override;
	void giveHitPoints(int hitPoints) override;
	void kill() override { giveDamage(m_currentHealth); };

	unsigned int getDamage() const override;
protected:
	unsigned int m_currentHealth;
	ProgressBar* m_p_healthBar;

private:
	SpaceShipInfo* m_p_spaceShipInfo;
};

#endif // !ENEMY_HPP