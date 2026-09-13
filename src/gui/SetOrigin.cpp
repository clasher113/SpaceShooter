#include "SetOrigin.hpp"

#include <SFML/Graphics/Transformable.hpp>

void SetOrigin::setOrigin(sf::Transformable& object, const sf::FloatRect& localBounds, Origin origin) {
	sf::Vector2f originVec(0.f, 0.f);
	switch (origin)
	{
	case Origin::CENTER_TOP:
		originVec.x = localBounds.size.x / 2.f;
		break;
	case Origin::RIGHT_TOP:
		originVec.x = localBounds.size.x;
		break;
	case Origin::LEFT_CENTER:
		originVec.y = localBounds.size.y / 2.f;
		break;
	case Origin::CENTER:
		originVec = localBounds.size / 2.f;
		break;
	case Origin::RIGHT_CENTER:
		originVec.x = localBounds.size.x;
		originVec.y = localBounds.size.y / 2.f;
		break;
	case Origin::LEFT_BOTTOM:
		originVec.y = localBounds.size.y;
		break;
	case Origin::CENTER_BOTTOM:
		originVec.x = localBounds.size.x / 2.f;
		originVec.y = localBounds.size.y;
		break;
	case Origin::RIGHT_BOTTOM:
		originVec = localBounds.size;
		break;
	}
	object.setOrigin(originVec);
}