#ifndef LAYERS_HPP
#define LAYERS_HPP

#define GUI_LAYER INT_MAX

#include <map>

class Layer;
namespace sf {
	class Drawable;
	class RenderWindow;
}

class Layers {
public:
	Layers();
	~Layers();

	void addDrawable(int layer, sf::Drawable* drawable);

	void draw(sf::RenderWindow& window, bool clear = true);
private:
	std::map<int, Layer*> m_layers;
};

#endif // !LAYERS_HPP
