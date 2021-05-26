#include <iostream>
#include <SFML/Graphics.hpp>
#include "engine.h"
#include "structures.h"

const float width {1000};
const float height {800};
const size_t NUM_NODES {4};

sf::RenderWindow *window;
sf::Font font;
const sf::Color background(155, 165, 155);

int m[NUM_NODES][NUM_NODES] = {
	       {0, 10, 0, 0},
	       {12, 0, 2, 1},
	       {0, 3, 0, 7},
	       {0, 8, 0, 0}
	     };

structures::Node nodes[NUM_NODES]; 

void generateGraph() {

	structures::Edge edge1 = structures::newEdge(&nodes[0], &nodes[1], 10);
}

int main() {

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Intranet path finder", sf::Style::Default, settings);

	if (!font.loadFromFile("Media/Fonts/Hack-Regular.ttf"))
		return 1;

	std::vector<size_t> my_path;
	generateGraph();

	for (size_t i {}; i < NUM_NODES; i++) {
		nodes[i] = structures::newNode(1, i+1);
		structures::setPosition(nodes[i], 50 + (i/5) * 120, 50 + (i%5) * 100);
	}
	
	bool once {true};
	
	//node that is clicked
	//And initial coordinates of mouse
	structures::Node *node {nullptr};
	float mouse_x_diff, mouse_y_diff;

	while (window->isOpen()) {

		sf::Event event;

		while (window->pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::MouseButtonReleased)
				   node = nullptr;

			if (node == nullptr && event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				   sf::Vector2i position = sf::Mouse::getPosition(*window);
	
				   for (size_t i {}; i < NUM_NODES; i++) {

						if (structures::isInside(nodes[i], position)) {

						 	node = &(nodes[i]);
							auto node_position = node->rect.getPosition();
							mouse_x_diff = position.x - node_position.x;
							mouse_y_diff = position.y - node_position.y;
							break;
						 }
				   }
				}
			}

			if (node != nullptr) {

				sf::Vector2i position = sf::Mouse::getPosition(*window);

				structures::setPosition(*node, position.x - mouse_x_diff, position.y - mouse_y_diff);
			}
		}

		window->clear(background);

		structures::draw(*window, nodes, m);

		if (once) {

			engine::DFS(*window, my_path, nodes, m, 3, 0);

			for (auto value : my_path) {

				std::cout << ++value << " ";
			}

			once = false;
		}

		window->display();
	}

	delete node;

	return 0;
}
