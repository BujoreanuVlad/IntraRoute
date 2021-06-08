#if !defined GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H
#include <SFML/Graphics.hpp>
#include <cstring>

extern sf::Font font;

namespace graphicsEngine {

	//Shows a prompt which asks for input and returns it when the user presses the enter key
	std::string prompt();
}

#endif
