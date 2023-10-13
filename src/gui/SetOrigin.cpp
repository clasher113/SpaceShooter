#include "SetOrigin.hpp"

#include <SFML/Graphics/Transformable.hpp>

void SetOrigin::setOrigin(sf::Transformable& object, const sf::FloatRect& localBounds, Origin origin) {
	switch (origin)
	{
	case Origin::LEFT_TOP:
		object.setOrigin(0.f, 0.f);
		break;
	case Origin::CENTER_TOP:
		object.setOrigin(localBounds.width / 2.f, 0.f);
		break;
	case Origin::RIGHT_TOP:
		object.setOrigin(localBounds.width, 0.f);
		break;
	case Origin::LEFT_CENTER:
		object.setOrigin(0.f, localBounds.height / 2.f);
		break;
	case Origin::CENTER:
		object.setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);
		break;
	case Origin::RIGHT_CENTER:
		object.setOrigin(localBounds.width, localBounds.height / 2.f);
		break;
	case Origin::LEFT_BOTTOM:
		object.setOrigin(0.f, localBounds.height);
		break;
	case Origin::CENTER_BOTTOM:
		object.setOrigin(localBounds.width / 2.f, localBounds.height);
		break;
	case Origin::RIGHT_BOTTOM:
		object.setOrigin(localBounds.width, localBounds.height);
		break;
	}
}