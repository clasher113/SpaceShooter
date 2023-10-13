#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Layers;
namespace sf {
	class RectangleShape;
}
enum class Origin;

class ProgressBar : public sf::Drawable {
public:
	ProgressBar(const unsigned int& value, int max, float width, float height);
	~ProgressBar();
	
	void update(const float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setPosition(const sf::Vector2f& position);
	void setSize(const sf::Vector2f& size);
	void setOrigin(Origin origin);

private:
	int m_max;
	const unsigned int& m_value;
	float m_showValue;

	sf::RectangleShape* m_p_border;
	sf::RectangleShape* m_p_level;
};

#endif // !PROGRESS_BAR_HPP