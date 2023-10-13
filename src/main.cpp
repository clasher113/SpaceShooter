#include "Application.hpp"
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

int main() {
	Application app;
	if (!app.initialize()) {
#ifdef _DEBUG
		std::cin.get();
#endif // _DEBUG
		return 1;
	}
	app.runGameLoop();

	return 0;
}