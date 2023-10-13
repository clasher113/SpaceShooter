#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "Widget.hpp"

#include <variant>

#define SLIDER_TYPES int*, unsigned int*, float*
typedef std::variant<SLIDER_TYPES> Value;

class Slider : public Widget {
public:
	Slider(Assets* assets, const sf::String& string, const Value& value, int min, int max);
	~Slider();

	void input(const sf::Vector2f& cursorPos) override;
	void update(const float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setPosition(const sf::Vector2f& position) override;
	void setSize(const sf::Vector2f& size) override;
	void setOrigin(Origin origin) override;
	void setString(const sf::String& string) override;

private:
	bool m_pressed;
	int m_min, m_max;
	Value m_value;

	sf::RectangleShape* m_p_slider;
	sf::Text* m_p_text;
	sf::String* m_p_string;

	float calculatePosition();
};

#endif // !SLIDER_HPP