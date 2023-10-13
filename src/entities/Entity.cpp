#include "Entity.hpp"
#include "../Assets.hpp"
#include "../graphics/Layers.hpp"
#include "EntityInfo.hpp"
#include "components/IComponent.hpp"
#include "components/Components.hpp"
#include "../system/Window.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transformable.hpp>

Entity::Entity(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem) : IEntity(),
	m_p_assets(assets),
	m_name(name),
	m_p_entitySystem(entitySystem),
	m_isDead(false),
	m_scale(Window::getAspectRatio() * ENTITIES_SCALE),
	m_p_entityInfo(EntityInfo::getEntity(name)),
	m_p_sprite(new sf::Sprite())
{
	m_p_sprite->setTexture(assets->getTexture(m_p_entityInfo->textureName));
	m_p_sprite->setOrigin(m_p_sprite->getTextureRect().width / 2.f, m_p_sprite->getTextureRect().height / 2.f);
	m_p_sprite->setPosition(x, y);
	m_p_sprite->setScale(m_scale, m_scale);
	if (m_p_entityInfo->movementData != nullptr) {
		addComponent(new MovementComponent(*m_p_entityInfo->movementData, entitySystem, this));
	}
	if (m_p_entityInfo->animationData != nullptr) {
		addComponent(new AnimationComponent(m_p_sprite, m_p_entityInfo->animationData));
	}
	for (auto& it : m_p_entityInfo->shootingData) {
		addComponent(new ShootingComponent(entitySystem, it, this));
	}
	if (!m_p_entityInfo->effectData.empty()) {
		addComponent(new EffectComponent(assets, entitySystem, &m_p_entityInfo->effectData, this));
	}
}

Entity::~Entity() {
	while (!m_components.empty()) {
		delete m_components.front();
		m_components.erase(m_components.begin());
	}
	delete m_p_sprite;
}

void Entity::input() {
	if (m_isDead) return;
	for (const auto& it : m_components) {
		it->input();
	}
}

void Entity::update(const float dt) {
	if (m_isDead) return;
	for (const auto& it : m_components) {
		it->update(dt);
	}

#ifdef DRAW_HITBOXES
	m_hitbox_shape.setFillColor(sf::Color::Transparent);
	m_hitbox_shape.setOutlineThickness(3.f * m_scale);
	m_hitbox_shape.setPosition(m_p_sprite->getPosition());
	m_hitbox_shape.setOrigin(m_p_entityInfo->hitboxHalfSize);
	m_hitbox_shape.setScale(m_scale, m_scale);
	m_hitbox_shape.setSize(m_p_entityInfo->hitboxHalfSize * 2.f);
	if (getHitbox().contains(sf::Vector2f(Window::getCursorPosition()))) {
		m_hitbox_shape.setOutlineColor(sf::Color::Red);
		if (Input::jclicked(sf::Mouse::Left))
			m_isDead = true;
	}
	else m_hitbox_shape.setOutlineColor(sf::Color::Yellow);
#endif // DRAW_HITBOXES
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_p_sprite);

#ifdef DRAW_HITBOXES
	target.draw(m_hitbox_shape);
#endif // DRAW_HITBOXES
}

void Entity::draw(Layers& layers) const {
	layers.addDrawable(m_p_entityInfo->drawLayer, m_p_sprite);
#ifdef DRAW_HITBOXES
	layers.addDrawable(GUI_LAYER, &m_hitbox_shape);
#endif // DRAW_HITBOXES
}

void Entity::addComponent(IComponent* component) {
	m_components.emplace(m_components.begin(), component);
}

void Entity::removeComponent(size_t iterator) {
	if (iterator > m_components.size()) return;
	removeComponent(m_components[iterator]);
}

void Entity::removeComponent(IComponent* component) {
	delete component;
	m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void Entity::setPosition(sf::Vector2f position) {
	m_p_sprite->setPosition(position);
}

void Entity::setRotation(float angle) {
	m_p_sprite->setRotation(angle);
}

void Entity::move(float offsetX, float offsetY) {
	m_p_sprite->move(offsetX, offsetY);
}

sf::FloatRect Entity::getHitbox() const {
	return sf::FloatRect(m_p_sprite->getPosition() - m_p_entityInfo->hitboxHalfSize * m_scale,
						 m_p_entityInfo->hitboxHalfSize * 2.f * m_scale);
}

sf::FloatRect Entity::getGlobalBounds() const {
	return m_p_sprite->getGlobalBounds();
}

const sf::Vector2f& Entity::getPosition() const {
	return m_p_sprite->getPosition();
}

EntityType Entity::getType() const {
	return m_p_entityInfo->type;
}

EntityInfo* Entity::getInfo() const {
	return m_p_entityInfo;
}

template<typename T>
inline T* Entity::getComponent(size_t offset) {
	for (const auto& it : m_components) {
		if (typeid(T) == typeid(*it)) {
			if (offset == 0)
				return static_cast<T*>(it);
			else offset--;
		}
	}
	return nullptr;
}

template AnimationComponent* Entity::getComponent<AnimationComponent>(size_t offset);
template EffectComponent* Entity::getComponent<EffectComponent>(size_t offset);