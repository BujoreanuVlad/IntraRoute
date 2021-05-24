#include <iostream>
#include <SFML/Graphics.hpp>
#include "engine.h"
#include "structures.h"

const float width {1000};
const float height {800};

sf::RenderWindow window(sf::VideoMode(width, height), "Intranet path finder");
sf::Font font;

int m[4][4] = {
	       {0, 10, 0, 0},
	       {12, 0, 2, 1},
	       {0, 3, 0, 7},
	       {0, 8, 0, 0}
	     };
structures::Node nodes[4]; 

int main() {

	if (!font.loadFromFile("Media/Fonts/Hack-Regular.ttf"))
		return 1;

	std::vector<size_t> my_path;

	for (size_t i {}; i < 4; i++) {
		nodes[i] = structures::newNode(1, i+1);
		structures::setPosition(nodes[i], 10 + (i/5) * 110, 10 + (i%5) * 70);
	}
	
	engine::BFS(my_path, m, 3, 0);

	for (auto value : my_path) {

		std::cout << ++value << " ";
	}
	
	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		structures::draw(window, nodes);

		window.display();
	}

	return 0;
}
