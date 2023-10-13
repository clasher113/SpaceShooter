#ifndef SET_ORIGIN_HPP
#define SET_ORIGIN_HPP

#include <SFML/Graphics/Rect.hpp>

namespace sf {
	class Transformable;
}

enum class Origin {
	LEFT_TOP = 0,	CENTER_TOP,		RIGHT_TOP,
	LEFT_CENTER,	CENTER,			RIGHT_CENTER,
	LEFT_BOTTOM,	CENTER_BOTTOM,	RIGHT_BOTTOM
};
namespace SetOrigin {
	extern void setOrigin(sf::Transformable& object, const sf::FloatRect& localBounds, Origin origin);
}
#endif // !SET_ORIGIN_HPP
