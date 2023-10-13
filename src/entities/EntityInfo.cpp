#include "EntityInfo.hpp"
#include "../utils/Log.hpp"

std::unordered_map<std::string, EntityInfo*> EntityInfo::entitiesInfo;

EntityInfo* EntityInfo::getEntity(const std::string& name) {
	try	{
		return entitiesInfo.at(name);
	}
	catch (const std::exception&) {
		ERR(__FUNCTION__ << "(" << name << ") : Entity doe's not exist!");
		return nullptr;
	}
}

const std::string& EntityInfo::getName(EntityInfo* entityInfo) {
	auto findResult = std::find_if(std::begin(entitiesInfo), std::end(entitiesInfo), [&](const std::pair<std::string, EntityInfo*>& pair) {
			return pair.second == entityInfo;
	});
	if (findResult != std::end(entitiesInfo)) {
		return findResult->first;
	};
	return std::string("");
}

decltype(EntityInfo::entitiesInfo)::iterator EntityInfo::getEntity(size_t index) {
	if (index > entitiesInfo.size() -1) {
		FATAL("Wrong index!", __FUNCTION__ << "(size_t " + std::to_string(index) + ") : Reached end of the container!");
		exit(1);
	}
	auto it = entitiesInfo.begin();
	std::advance(it, index);
	return it;
}

void EntityInfo::addEntity(const std::string& name, EntityInfo* entity) {
	entitiesInfo.emplace(name, entity);
}