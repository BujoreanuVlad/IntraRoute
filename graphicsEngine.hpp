#if !defined GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H
#include <SFML/Graphics.hpp>
#include <cstring>
#include "structures.hpp"

extern sf::Font font;
extern const sf::Color background;
extern const size_t NUM_BUTTONS;
extern structures::Button buttons[];

namespace graphicsEngine {

	//Shows a prompt which asks for input and returns it when the user presses the enter key
	std::string prompt();
    //Draws all the things for the window
    void draw(sf::RenderWindow &window, size_t NUM_NODES, structures::Node nodes[], int **m);
}

#endif
