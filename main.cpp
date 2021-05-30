#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "engine.hpp"
#include "structures.hpp"

const float width {1000};
const float height {800};
const size_t NUM_NODES {4};

const size_t start_node {0};
const size_t end_node {3};

sf::RenderWindow *window;
sf::Font font;
const sf::Color background(155, 165, 155);
std::vector<size_t> my_path;

int m[NUM_NODES][NUM_NODES] = {
	       {0, 10, 0, 0},
	       {12, 0, 2, 1},
	       {0, 3, 0, 7},
	       {0, 8, 0, 0}
	     };

structures::Node nodes[NUM_NODES]; 

namespace {

	void reset(std::vector<size_t> &path) {

		structures::reset(nodes);
		path.clear();
	}

	void choice(int code) {

		switch (code) {

			case engine::DFS_CODE: engine::DFS(*window, my_path, nodes, m, end_node, start_node); break;
			case engine::BFS_CODE: engine::BFS(*window, my_path, nodes, m, end_node, start_node); break;
			default: std::cout << "Error, no matching code\n"; break;
		}
	}
}

int main() {

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Intranet path finder", sf::Style::Default, settings);
	engine::setTimeLink(2);

	if (!font.loadFromFile("Media/Fonts/Hack-Regular.ttf"))
		return 1;

	for (size_t i {}; i < NUM_NODES; i++) {
		nodes[i] = structures::newNode(1, i+1);
		structures::setPosition(nodes[i], 50 + (i/5) * 120, 50 + (i%5) * 100);
	}
	
	//Flag that determines wether the algorithm should be run or not
	bool once {};
	
	//node that is clicked
	//And initial coordinates of mouse
	structures::Node *node {nullptr};
	float mouse_x_diff, mouse_y_diff;

	while (window->isOpen()) {

		sf::Event event;

		while (window->pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Space)
					once = true;

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

			reset(my_path);

			//engine::BFS(*window, my_path, nodes, m, 3, 0);
			choice(engine::BFS_CODE);

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
