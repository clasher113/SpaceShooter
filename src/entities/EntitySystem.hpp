#ifndef ENTITY_SYSTEM_HPP
#define ENTITY_SYSTEM_HPP

#include "EntityInfo.hpp"

#include <vector>
#include <string>

#include <SFML/Graphics/Rect.hpp>

class Assets;
class IEntity;
class Player;
namespace sf {
	class RenderWindow;
}

class EntitySpawner;

class EntitySystem {
public:
	EntitySystem(Assets* assets);
	~EntitySystem();

	void input();
	void update(Player* player, const float dt);
	void draw(sf::RenderWindow& window);

	template<typename Type>
	Type* spawn(const std::string& name, float x, float y);

	IEntity* getClosestEntity(sf::Vector2f position, EntityType type = EntityType::UNDEFINED, const std::string& name = std::string("")) const;
	IEntity* getEntity(size_t index) const;
	size_t getSize() const { return m_entities.size(); };

	void remove(size_t index);
	void remove(IEntity* entity);
private:
	bool m_cleaningIsRequired;

	sf::FloatRect m_windowBounds;

	Assets* m_p_assets;

	std::vector<IEntity*> m_entities;
	std::vector<IEntity*> m_addBuffer;
	std::vector<EntitySpawner> m_entitySpawner;
};

class EntitySpawner {
public:
	EntitySpawner(Assets* assets, const std::string& name, EntitySystem* entitySystem);

	void update(const float dt);
	static void increasDifficulty(const float dt);

	static float s_spawnSpeed;
private:
	float m_spawnInterval, m_spawnTimer;
	static float s_maxSpawnSpeed;
	static float s_increasDifficultyInterval, s_increasDifficultyTimer, s_increasDifficultyFactor;
	sf::Vector2f m_offset;
	std::string m_entityName;
	EntityType m_entityType;
	Assets* m_p_assets;
	EntitySystem* m_p_entitySystem;
};

#endif // !ENTITY_SYSTEM_HPP