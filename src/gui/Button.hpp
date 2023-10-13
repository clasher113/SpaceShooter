#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Widget.hpp"

#include <functional>

class Button : public Widget {
public:
	Button(Assets* assets, const std::function<void()>& callback);
	~Button();

	void input(const sf::Vector2f& cursorPos) override;
	void update(const float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setPosition(const sf::Vector2f& position);
	void setOrigin(Origin origin);
	void setString(const sf::String& string);

private:
	sf::Text* m_p_text;
	std::function<void()> m_callback;
};

#endif // !BUTTON_HPP