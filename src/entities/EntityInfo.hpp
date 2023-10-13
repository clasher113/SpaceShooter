#ifndef ENTITY_INFO_HPP
#define ENTITY_INFO_HPP

#include <unordered_map>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

struct AnimationData;
struct ShootingData;
struct EffectData;
struct MovementData;
enum class EffectState;

enum class EntityType {
	UNDEFINED = 0,
	ENEMY,
	PLAYER,
	UPGRADE
};

class EntityInfo {
public:
	virtual ~EntityInfo() {};
	EntityType type = EntityType::UNDEFINED;
	sf::Vector2f hitboxHalfSize{ 0.f, 0.f };
	std::string textureName;
	int drawLayer = 0;
	
	AnimationData* animationData = nullptr;
	MovementData* movementData = nullptr;
	std::vector<ShootingData*> shootingData;
	std::unordered_multimap<EffectState, EffectData*> effectData;

	static EntityInfo* getEntity(const std::string& name);
	static const std::string& getName(EntityInfo* entityInfo);
	static size_t getSize() { return entitiesInfo.size(); };
	
	static void addEntity(const std::string& name, EntityInfo* entity);

private:
	static std::unordered_map<std::string, EntityInfo*> entitiesInfo;
public:
	static decltype(EntityInfo::entitiesInfo)::iterator getEntity(size_t index); // not safe
};

struct SpaceShipInfo : public EntityInfo {
	SpaceShipInfo() : EntityInfo() { drawLayer = 1; }
	virtual ~SpaceShipInfo() {};
	unsigned int maxHealth = 100;
	float spawnInterval = 10.f;
};

struct BulletInfo : public EntityInfo {
	BulletInfo() : EntityInfo() { drawLayer = 2; }
	virtual ~BulletInfo() {};
	unsigned int damageMin = 10;
	unsigned int damageMax = 10;
};

struct EffectInfo : public EntityInfo {
	EffectInfo() : EntityInfo() { drawLayer = 3; }
	virtual ~EffectInfo() {};
};

struct UpgradeInfo : public EntityInfo {
	UpgradeInfo() : EntityInfo() { drawLayer = 2; type = EntityType::UPGRADE; }
	virtual ~UpgradeInfo() {};

	std::unordered_map<std::string, std::string> command;
};

#endif // !ENTITY_INFO_HPP
