#include "AnimationComponent.hpp"

#include <SFML/Graphics/Sprite.hpp>

AnimationComponent::AnimationComponent(sf::Sprite* sprite, AnimationData* animationData) :
	m_p_sprite(sprite),
	m_p_animationData(animationData),
	m_currentFrame(0),
	m_frameOffset(0),
	m_cycle(0),
	m_lastFrame(0),
	m_width(sprite->getTextureRect().width / animationData->m_columns),
	m_height(sprite->getTextureRect().height / animationData->m_rows)
{
	m_p_sprite->setTextureRect(sf::IntRect(0, 0, m_width, m_height));
	m_p_sprite->setOrigin(m_width / 2.f, m_height / 2.f);
}

void AnimationComponent::update(const float dt) {
	m_currentFrame += m_p_animationData->m_speed * dt;
	if (m_lastFrame == static_cast<int>(m_currentFrame)) return;
	m_lastFrame = static_cast<int>(m_currentFrame);
	if (m_currentFrame > m_p_animationData->m_frames) {
		m_currentFrame -= m_p_animationData->m_frames;
		m_cycle++;
	}
	int currentColumn = static_cast<int>(m_currentFrame + m_frameOffset) % m_p_animationData->m_columns;
	int currentRow = static_cast<int>((m_currentFrame + m_frameOffset) / m_p_animationData->m_columns);

	m_p_sprite->setTextureRect(sf::IntRect(m_width * currentColumn, m_height * currentRow, m_width,	m_height));
}