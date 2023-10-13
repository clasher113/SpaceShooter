#ifndef UPGRADE_HPP
#define UPGRADE_HPP

#include "Entity.hpp"

#include <functional>

class Player;
struct UpgradeInfo;

class Upgrade : public Entity {
public:
	Upgrade(Assets* assets, const std::string& name, float x, float y, EntitySystem* entitySystem);
	~Upgrade() override;

	void giveDamage(int damage) override;
	void kill() override { giveDamage(0); };

	static void initialize(EntitySystem* entitySystem, Player* player);
	static void runCommand(const std::string& command, const std::string& args, Upgrade* upgrade);
	static void finalize();

private:
	UpgradeInfo* m_p_upgradeInfo;

	static std::unordered_map<std::string, std::function<void(const std::string& args, Upgrade* upgrade)>> s_m_commands;
};

#endif // !UPGRADE_HPP
