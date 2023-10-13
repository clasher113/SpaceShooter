#ifndef INPUT_HPP
#define INPUT_HPP

typedef unsigned int uint;

class Input {
public:
	static void initialize();
	static void finalize();

	static bool pressed(int keycode);
	static bool jpressed(int keycode);
	static bool unpressed(int keycode);

	static bool clicked(int button);
	static bool jclicked(int button);
	static bool unclicked(int button);

	static bool* _keys;
	static uint* _frames;
	static uint _current;
};

#endif // !INPUT_HPP