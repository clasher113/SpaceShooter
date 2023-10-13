#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

class IComponent {
public:
	IComponent() {};
	virtual ~IComponent() {};

	virtual void input() = 0;
	virtual void update(const float dt) = 0;
};

#endif // !ICOMPONENT_HPP