#ifndef LAYER_HPP
#define LAYER_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <vector>

class Layer : public sf::Drawable {
public:
	Layer();
	~Layer();

	void addDrawable(sf::Drawable* drawable);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::vector<sf::Drawable*> m_drawableList;
};

#endif // !LAYER_HPP
