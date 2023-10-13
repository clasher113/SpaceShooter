#include "Bullet.hpp"
#include "EntityInfo.hpp"
#include "EntitySystem.hpp"
#include "components/EffectComponent.hpp"
#include "../utils/Random.hpp"

Bullet::Bullet(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem) : Entity(assets, name, x, y, entitySystem),
	m_p_bulletInfo(static_cast<BulletInfo*>(m_p_entityInfo))
{
}

Bullet::~Bullet() {
}

void Bullet::giveDamage(int damage) {
	EffectComponent* effect = static_cast<EffectComponent*>(getComponent<EffectComponent>());
	if (effect != nullptr) {
		effect->onDeathEffect();
		removeComponent(effect);
	}
	m_p_entitySystem->remove(this);
}

unsigned int Bullet::getDamage() const {
	return effolkronium::random_static::get<unsigned int>(m_p_bulletInfo->damageMin, m_p_bulletInfo->damageMax);
}
