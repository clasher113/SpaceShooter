#include "EngineInitializer.hpp"

#include "Assets.hpp"
#include "audio/AudioManager.hpp"
#include "entities/EntityInfo.hpp"
#include "entities/components/Components.hpp"
#include "utils/Log.hpp"
#include "utils/Config.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <memory>

#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/Music.hpp>
#include <fparser_mpfr.hh>

template<typename T_RES>
void loadResource(Assets* assets, const std::string& name, const std::string& path) {
	T_RES* resource = new T_RES();
	if (!resource->loadFromFile(path)) {
		delete resource;
		return;
	}
	assets->store(name, resource);
	INFO("Resource \"" << name << "\" with type \"" << typeid(T_RES).name() << "\" loaded");
}

rapidjson::Document* readJSON(const std::string& filePath) {
	std::ifstream f;
	f.open(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		FATAL("Parsing error", "Failed to open file: " << filePath);
		return 0;
	}

	std::stringstream buffer;
	buffer << f.rdbuf();

	const std::string JSONString = buffer.str();
	if (JSONString.empty())
	{
		FATAL("Parsing error", "No JSON resources file!");
		return 0;
	}

	rapidjson::Document* document = new rapidjson::Document;
	rapidjson::ParseResult parseResult = document->Parse(JSONString.c_str());
	if (!parseResult)
	{
		FATAL("JSON parse error: ", rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl <<
			"In JSON file: " << filePath);
		return 0;
	}
	return document;
}

template<typename T_RES>
void parse(Assets* assets, const rapidjson::Document& document, const std::string& path, const std::string& memberName) {
	auto resourceIt = document.FindMember(memberName.c_str());
	if (resourceIt != document.MemberEnd()) {
		for (const auto& currentResource : resourceIt->value.GetArray()) {
			std::string name(currentResource.GetArray()[0].GetString());
			std::string fullPath(path + currentResource.GetArray()[1].GetString());
			loadResource<T_RES>(assets, name, fullPath);
		}
	}
}

template<typename Type>
void getMember(const rapidjson::Value& from, const char* memberName, Type& to) {
	if (from.HasMember(memberName)) to = from[memberName].Get<Type>();
};

void getMember(const rapidjson::Value& from, const char* memberName, std::string& to) {
	if (from.HasMember(memberName)) to = from[memberName].GetString();
};

std::unordered_map<EffectState, std::string> effectStates;

void animationComponent(const rapidjson::Value& currentEntity, AnimationData*& animData) {
	auto animationComponentIt = currentEntity.FindMember("animationComponent");
	if (animationComponentIt == currentEntity.MemberEnd()) return;
	animData = new AnimationData();
	const auto& currentAnimation = animationComponentIt->value.GetObj();
	getMember(currentAnimation, "frames", animData->m_frames);
	getMember(currentAnimation, "framesColumns", animData->m_columns);
	getMember(currentAnimation, "framesRows", animData->m_rows);
	getMember(currentAnimation, "animationSpeed", animData->m_speed);
}

void movementComponent(const rapidjson::Value& currentEntity, MovementData*& movementData) {
	auto movementComponentIt = currentEntity.FindMember("movementComponent");
	if (movementComponentIt == currentEntity.MemberEnd()) return;
	movementData = new MovementData();
	const auto& currentMovementData = movementComponentIt->value.GetObj();
	getMember(currentMovementData, "aimEnemy", movementData->m_aimEnemy);
	getMember(currentMovementData, "followEnemy", movementData->m_followEnemy);
	getMember(currentMovementData, "startSpeed", movementData->m_startSpeed);
	getMember(currentMovementData, "maxSpeed", movementData->m_maxSpeed);
	getMember(currentMovementData, "accelerationSpeed", movementData->m_accelerationSpeed);
	const auto& functionIt = currentMovementData.FindMember("function");
	if (functionIt != currentMovementData.MemberEnd()) {
		size_t it = 0;
		for (const auto& currentFunction : functionIt->value.GetArray()) {
			FunctionData* functionData = &movementData->m_functionData[it++];
			std::string expression, variables, variables_parsed;
			getMember(currentFunction, "expression", expression);
			getMember(currentFunction, "variables", variables_parsed);
			if (expression.empty() || variables_parsed.empty()) continue;
			std::vector<std::string> tokens;
			std::istringstream iss(variables_parsed);
			std::string s;
			while (getline(iss, s, ',')) {
				tokens.emplace_back(s);
			}
			for (const auto& it : tokens) {
				size_t pos = it.find('=');
				variables.append(it.substr(0, pos) + std::string(","));
				functionData->m_functionVariables.emplace_back(static_cast<float>(atof(it.substr(pos + 1, it.size()).data())));
			}
			functionData->m_p_function.Parse(expression, variables + " posX, posY, elapsed, dt");
			const char* errmsg = functionData->m_p_function.ErrorMsg();
			if (strlen(errmsg) > 0) {
				ERR(expression << ": " << errmsg);
				continue;
			}
			functionData->m_p_function.Optimize();
			if (it == 2) break;
		}
	}
	const auto& direction = currentMovementData.FindMember("direction");
	if (direction != currentMovementData.MemberEnd())
		movementData->m_direction = sf::Vector2f(direction->value[0].GetFloat(), direction->value[1].GetFloat());
}

void shootingComponent(const rapidjson::Value& currentEntity, std::vector<ShootingData*>& shootingData) {
	auto shootingComponentIt = currentEntity.FindMember("shootingComponent");
	if (shootingComponentIt == currentEntity.MemberEnd()) return;
	shootingData.reserve(shootingComponentIt->value.GetArray().Size());
	for (const auto& currentShootingComponent : shootingComponentIt->value.GetArray()) {
		ShootingData* currentShootingData = shootingData.emplace_back(new ShootingData());
		getMember(currentShootingComponent, "shootInterval", currentShootingData->m_shootInterval);
		getMember(currentShootingComponent, "queueInterval", currentShootingData->m_queueInterval);
		getMember(currentShootingComponent, "queueSize", currentShootingData->m_queueSize);
		getMember(currentShootingComponent, "bulletName", currentShootingData->m_bulletName);
		getMember(currentShootingComponent, "shootPosOffsetX", currentShootingData->m_positionOffset.x);
		getMember(currentShootingComponent, "shootPosOffsetY", currentShootingData->m_positionOffset.y);
		if (currentShootingComponent.HasMember("mode")) {
			std::string mode = currentShootingComponent["mode"].GetString();
			if (mode == "auto") currentShootingData->m_shootingMode = ShootingMode::AUTOMATIC;
			else if (mode == "manual") currentShootingData->m_shootingMode = ShootingMode::MANUAL;
			else currentShootingData->m_shootingMode = ShootingMode::UNKNOWN;
			getMember(currentShootingComponent, "key", currentShootingData->m_key);
		}
		movementComponent(currentShootingComponent, currentShootingData->m_p_movementData);
	}
}

void effectComponent(const rapidjson::Value& currentEntity, std::unordered_multimap<EffectState, EffectData*>& effectData) {
	auto effectComponentIt = currentEntity.FindMember("effectComponent");
	if (effectComponentIt == currentEntity.MemberEnd()) return;
	const auto& currentEffectComponent = effectComponentIt->value.GetObj();
	for (const auto& it : effectStates) {
		const auto& currentStateIt = currentEffectComponent.FindMember(it.second.data());
		if (currentStateIt == currentEffectComponent.MemberEnd()) continue;
		const auto& currentState = currentStateIt->value.GetArray();
		for (const auto& currentEffect : currentState) {
			EffectData* currentEffectData = effectData.emplace(it.first, new EffectData())->second;
			getMember(currentEffect, "effect", currentEffectData->m_effectName);
			getMember(currentEffect, "effectPosOffsetX", currentEffectData->m_positionOffset.x);
			getMember(currentEffect, "effectPosOffsetY", currentEffectData->m_positionOffset.y);
			getMember(currentEffect, "pinToParent", currentEffectData->m_pinToParent);
			getMember(currentEffect, "loopAnimation", currentEffectData->m_loopAnimation);
			getMember(currentEffect, "loopSound", currentEffectData->m_loopSound);
			const auto& sounds = currentEffect.FindMember("sounds");
			if (sounds == currentEffect.MemberEnd()) continue;
			for (const auto& currentSound : sounds->value.GetArray()) {
				currentEffectData->m_sounds.emplace_back(currentSound.GetString());
			}
		}
	}
}

void parseEntity(const rapidjson::Value& currentEntity, EntityInfo* entity) {
	auto hitboxSize = currentEntity.FindMember("hitboxHalfSize");
	if (hitboxSize != currentEntity.MemberEnd()) {
		entity->hitboxHalfSize.x = hitboxSize->value[0].GetFloat();
		entity->hitboxHalfSize.y = hitboxSize->value[1].GetFloat();
	}
	if (currentEntity.HasMember("isPlayer"))
		entity->type = (currentEntity["isPlayer"].GetBool() ? EntityType::PLAYER : EntityType::ENEMY);
	getMember(currentEntity, "textureName", entity->textureName);
	getMember(currentEntity, "drawLayer", entity->drawLayer);

	animationComponent(currentEntity, entity->animationData);
	movementComponent(currentEntity, entity->movementData);
	shootingComponent(currentEntity, entity->shootingData);
	effectComponent(currentEntity, entity->effectData);

	if (currentEntity.HasMember("name"))
		EntityInfo::addEntity(currentEntity["name"].GetString(), entity);
	else delete entity;
}

bool initializeEngine(Assets* assets) {
	Config::readConfig();

	INFO("Loading assets...");
	std::unique_ptr<rapidjson::Document> document(readJSON("resources/resources.json"));

	if (!document->IsObject()) {
		return 0;
	}

	INFO("Loading textures...");
	parse<sf::Texture>(assets, *document, "resources/textures/", "textures");
	INFO("Loading sounds...");
	parse<sf::SoundBuffer>(assets, *document, "resources/sounds/", "sounds");

	INFO("Loading musics...");
	auto musicIt = document->FindMember("musics");
	if (musicIt != document->MemberEnd()) {
		for (const auto& currentMusic : musicIt->value.GetArray()) {
			sf::Music* music = new sf::Music();
			std::string name(currentMusic.GetArray()[0].GetString());
			if (music->openFromFile(std::string("resources/musics/") + currentMusic.GetArray()[1].GetString())) {
				assets->store(name, music);
				INFO("Resource \"" << name << "\" with type \"" << typeid(sf::Music).name() << "\" opened");
			}
			else delete music;
		}
	}
	INFO("Loading fonts...");
	parse<sf::Font>(assets, *document, "resources/fonts/", "fonts");

	INFO("Loading strings...");
	auto stringIt = document->FindMember("strings");
	if (stringIt != document->MemberEnd()) {
		for (const auto& currentString : stringIt->value.GetArray()) {
			std::string fullString(currentString.GetString());
			std::string name(std::string(fullString).erase(fullString.find('=')));
			assets->store(name, new std::string(fullString.erase(0, name.size() + 1)));
		}
	}

	INFO("Loading assets success!");

	effectStates.emplace(EffectState::ON_DEPLOY, "onDeploy");
	effectStates.emplace(EffectState::ON_DEATH, "onDeath");
	effectStates.emplace(EffectState::ON_SHOOT, "onShoot");

	INFO("Parsing entities...");

	document.reset(readJSON("resources/entities.json"));

	const auto& spaceShipIt = document->FindMember("spaceShips");
	if (spaceShipIt != document->MemberEnd()) {
		for (const auto& currentSpaceShip : spaceShipIt->value.GetArray()) {
			SpaceShipInfo* spaceShip = new SpaceShipInfo();
			getMember(currentSpaceShip, "maxHealth", spaceShip->maxHealth);
			getMember(currentSpaceShip, "spawnInterval", spaceShip->spawnInterval);
			parseEntity(currentSpaceShip, spaceShip);
		}
	}
	const auto& bulletIt = document->FindMember("bullets");
	if (bulletIt != document->MemberEnd()) {
		for (const auto& currentBullet : bulletIt->value.GetArray()) {
			BulletInfo* bullet = new BulletInfo();
			getMember(currentBullet, "damageMin", bullet->damageMin);
			getMember(currentBullet, "damageMax", bullet->damageMax);
			parseEntity(currentBullet, bullet);
		}
	}
	const auto& effectIt = document->FindMember("effects");
	if (effectIt != document->MemberEnd()) {
		for (const auto& currentEffect : effectIt->value.GetArray()) {
			EffectInfo* effect = new EffectInfo();
			parseEntity(currentEffect, effect);
		}
	}
	const auto& upgradeIt = document->FindMember("upgrades");
	if (upgradeIt != document->MemberEnd()) {
		for (const auto& currentUpgradde : upgradeIt->value.GetArray()) {
			UpgradeInfo* upgrade = new UpgradeInfo();
			std::string command, args;
			getMember(currentUpgradde, "command", command);
			getMember(currentUpgradde, "arguments", args);
			upgrade->command.emplace(command, args);
			getMember(currentUpgradde, "spawnInterval", reinterpret_cast<SpaceShipInfo*>(upgrade)->spawnInterval);
			parseEntity(currentUpgradde, upgrade);
		}
	}

	effectStates.clear();
	INFO("Loaded " << EntityInfo::getSize() << " entities!");

	return 1;
}
