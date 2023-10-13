#ifndef SETTING_HPP
#define SETTING_HPP

#include "GuiFactory.hpp"

class Settings : public GuiFactory {
public:
	Settings(Application& application);
	~Settings();
};

#endif // !SETTING_HPP