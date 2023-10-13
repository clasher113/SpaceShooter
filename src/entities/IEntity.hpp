#ifndef IENTITY_HPP
#define IENTITY_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <string>

class Assets;
class Layers;
class EntityInfo;
namespace sf {
	class RenderWindow;
	class Sprite;
}
enum class EntityType;

class IEntity : public sf::Drawable {
public:
	IEntity() {};
	virtual ~IEntity() {};

	virtual void input() = 0;
	virtual void update(const float dt) = 0;
	virtual void draw(Layers& layers) const = 0;

	virtual void setIsDead(bool isDead) = 0;
	virtual void setPosition(sf::Vector2f position) = 0;
	virtual void setRotation(float angle) = 0;
	virtual void giveDamage(int damage) = 0;
	virtual void giveHitPoints(int hitPoints) = 0;
	virtual void move(float offsetX, float offsetY) = 0;
	virtual void kill() = 0;

	virtual bool isDead() const = 0;
	virtual unsigned int getDamage() const = 0;
	virtual sf::FloatRect getHitbox() const = 0;
	virtual sf::FloatRect getGlobalBounds() const = 0;
	virtual const sf::Vector2f& getPosition() const = 0;
	virtual EntityType getType() const = 0;
	virtual EntityInfo* getInfo() const = 0;
	virtual const std::string& getName() const = 0;
};

#endif // !IENTITY_HPP
