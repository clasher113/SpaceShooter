#include "Components.hpp"
#include "../../Assets.hpp"
#include "../Effect.hpp"
#include "../EntitySystem.hpp"
#include "../EntityInfo.hpp"
#include "../../audio/AudioManager.hpp"
#include "../../utils/Random.hpp"
#include <SFML/Audio/SoundSource.hpp>

EffectComponent::EffectComponent(Assets* assets, EntitySystem* entitySystem, std::unordered_multimap<EffectState, EffectData*>* effectData, Entity* entity) :
	m_p_assets(assets),
	m_p_entitySystem(entitySystem),
	m_p_effectData(effectData),
	m_p_entity(entity),
	m_p_lastEffect(nullptr),
	m_p_lastSound(nullptr)
{
	processEffect(EffectState::ON_DEPLOY);
}

EffectComponent::~EffectComponent() {
	const auto& deploySound = m_p_effectData->find(EffectState::ON_DEPLOY);
	if (deploySound != m_p_effectData->end()) {
		AudioManager::setStatus(deploySound->second->lastSound, Status::STOP);
	}
}

Effect* EffectComponent::getLastEffect(EffectState effectState) const {
	const auto& deployEffect = m_p_effectData->find(effectState);
	if (deployEffect != m_p_effectData->end()) {
		return deployEffect->second->lastEffect;
	}
	return nullptr;
}

void EffectComponent::onDeathEffect() {
	processEffect(EffectState::ON_DEATH);
}

void EffectComponent::onShootEffect(ShootingData* shootingData) {
	EntityInfo* entityInfo = m_p_entity->getInfo();
	for (const auto& it : entityInfo->shootingData) {
		if (it == shootingData) {
			processEffect(EffectState::ON_SHOOT, static_cast<int>(&it - &entityInfo->shootingData.front()));
		}
	}
}

void EffectComponent::processEffect(EffectState effectState, int index) {
	bool skip = index != -1;
	float scale = *m_p_entity->getScale();
	const auto& range = m_p_effectData->equal_range(effectState);
	for (auto current = range.first; current != range.second; current++) {
		if (index > 0) {
			index--;
			continue;
		}
		if (current != m_p_effectData->end()) {
			if (!current->second->m_effectName.empty()) {
				m_p_lastEffect = m_p_entitySystem->spawn<Effect>(current->second->m_effectName, m_p_entity->getPosition().x + current->second->m_positionOffset.x * scale,
					m_p_entity->getPosition().y + current->second->m_positionOffset.y * scale);
				m_p_lastEffect->setLoop(current->second->m_loopAnimation);
				if (current->second->m_pinToParent) {
					m_p_lastEffect->pinTo(m_p_entity->getPosition());
					m_p_lastEffect->setOffset(current->second->m_positionOffset * scale);
				}
				current->second->lastEffect = m_p_lastEffect;
			}
			if (!current->second->m_sounds.empty()) {
				size_t randomSound = effolkronium::random_static::get<size_t>(0, current->second->m_sounds.size() - 1);
				if (effectState == EffectState::ON_SHOOT) {
					AudioManager::setStatus(current->second->lastSound, Status::STOP);
				}
				m_p_lastSound = AudioManager::playSound(current->second->m_sounds[randomSound].c_str(), current->second->m_loopSound);
				current->second->lastSound = m_p_lastSound;
			}
		}
		if (skip) break;
	}
}