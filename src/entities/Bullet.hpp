#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"

#include <string>

namespace sf {
	class RenderWindow;
}
class EntitySystem;

class Bullet : public Entity {
public:
	Bullet(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem);
	~Bullet() override;

	void giveDamage(int damage) override;
	void kill() override { giveDamage(0); }
	unsigned int getDamage() const override;
private:
	BulletInfo* m_p_bulletInfo;
};

#endif // !BULLET_HPP
