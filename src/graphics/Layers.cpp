#include "Layers.hpp"
#include "Layer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Layers::Layers() {
}

Layers::~Layers() {
	for (const auto& it : m_layers) {
		delete it.second;
	}
}

void Layers::addDrawable(int layer, sf::Drawable* drawable) {
	Layer* layerPtr = nullptr;
	if (m_layers.find(layer) == m_layers.end())
		m_layers.emplace(layer, layerPtr = new Layer());
	else layerPtr = m_layers[layer];
	layerPtr->addDrawable(drawable);
}

void Layers::draw(sf::RenderWindow& window, bool clear) {
	for (const auto& it : m_layers) {
		window.draw(*it.second);
		if (clear) delete it.second;
	}
	if (clear) m_layers.clear();
}