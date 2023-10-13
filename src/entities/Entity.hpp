#ifndef ENTITY_HPP
#define ENTITY_HPP

#define ENTITIES_SCALE 1.5f
//#define DRAW_HITBOX

#include "IEntity.hpp"

#ifdef _DEBUG
#include "../utils/DebugInfo.hpp"
#ifdef DRAW_HITBOXES
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../system/Input.hpp"
#endif // DRAW_HITBOXES
#endif // _DEBUG

#include <vector>

class IComponent;
class EntityInfo;
class EntitySystem;
struct SpaceShipInfo;
struct BulletInfo;

class Entity : public IEntity{
public:
	Entity(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem);
	~Entity() override;

	void input() override;
	void update(const float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void draw(Layers& layers) const override;

	void addComponent(IComponent* component);
	void removeComponent(size_t iterator);
	void removeComponent(IComponent* component);

	void setIsDead(bool isDead) override { m_isDead = isDead; };
	void setPosition(sf::Vector2f position) override;
	void setRotation(float angle) override;
	void giveDamage(int damage) override {};
	void giveHitPoints(int hitPoints) override {};
	void move(float offsetX, float offsetY) override;
	void kill() override {};

	bool isDead() const override { return m_isDead; };
	unsigned int getDamage() const override { return 0; };
	float* getScale() { return &m_scale; };
	const sf::Vector2f& getPosition() const override;
	sf::FloatRect getHitbox() const override;
	sf::FloatRect getGlobalBounds() const override;
	EntityType getType() const override;
	EntityInfo* getInfo() const override;
	const std::string& getName() const override { return m_name; };
	template<typename T>
	T* getComponent(size_t offset = 0);
protected:
	bool m_isDead;

	float m_scale;

	EntityInfo* m_p_entityInfo;
	EntitySystem* m_p_entitySystem;
	Assets* m_p_assets;

	std::string m_name;

	sf::Sprite* m_p_sprite;
private:
#ifdef DRAW_HITBOXES
	mutable sf::RectangleShape m_hitbox_shape;
#endif // DRAW_HITBOXES
	std::vector<IComponent*> m_components;
};

#endif // !ENTITY_HPP