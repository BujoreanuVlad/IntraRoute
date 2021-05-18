#include <iostream>
#include <SFML/Graphics.hpp>
#include "engine.h"

const float width {1000};
const float height {800};

sf::RenderWindow window(sf::VideoMode(width, height), "Intranet path finder");

int m[4][4] = {
			   {0, 10, 0, 0},
			   {12, 0, 20, 15},
			   {0, 3, 0, 7},
			   {0, 8, 0, 0}
			  };

int main() {

	std::vector<size_t> my_path;

	engine::DFS(my_path, m, 3, 0);
	std::cout << "\n";

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

		window.display();
	}

	return 0;
}
