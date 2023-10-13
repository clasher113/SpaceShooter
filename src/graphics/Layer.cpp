#include "Layer.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Layer::Layer() {
}

Layer::~Layer() {
}

void Layer::addDrawable(sf::Drawable* drawable) {
	m_drawableList.emplace_back(drawable);
}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (const auto& it : m_drawableList) {
		target.draw(*it);
	}
}