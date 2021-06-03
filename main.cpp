#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "engine.hpp"
#include "structures.hpp"

const float width {1000};
const float height {800};
const size_t NUM_NODES {4};
const size_t NUM_BUTTONS {2};

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
structures::Button buttons[NUM_BUTTONS];

namespace {

	void init() {

		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window = new sf::RenderWindow(sf::VideoMode(width, height), "Intranet path finder", sf::Style::Default, settings);
		engine::setTimeLink(1);

		if (!font.loadFromFile("Media/Fonts/Hack-Regular.ttf"))
			window->close();

		for (size_t i {}; i < NUM_NODES; i++) {
			nodes[i] = structures::newNode(1, i+1);
			structures::setPosition(nodes[i], 50 + (i/5) * 120, 150 + (i%5) * 100);
		}

		buttons[0] = structures::newButton(engine::DFS_CODE);
		setPosition(buttons[0], width / 100, height / 80);
		setText(buttons[0], "DFS"); 
		buttons[1] = structures::newButton(engine::BFS_CODE);
		setPosition(buttons[1], 2 * width / 100 + buttons[0].width, height / 80);
		setText(buttons[1], "BFS");
	}

	void reset() {
		structures::reset(nodes);
		my_path.clear();
	}

	void choice(int code) {

		switch (code) {

			case engine::DFS_CODE: 
				reset();
				engine::DFS(*window, my_path, nodes, m, end_node, start_node);
				for (auto value : my_path)
					std::cout << ++value << " ";
				break;
			case engine::BFS_CODE:
				reset();
				engine::BFS(*window, my_path, nodes, m, end_node, start_node);
				for (auto value : my_path)
					std::cout << ++value << " ";
				break;
			default: std::cout << "Error, no matching code\n"; break;
		}
	}

	void draw() {

		sf::RectangleShape topBar(sf::Vector2f(width, height / 10));
		topBar.setPosition(0, 0);

		window->draw(topBar);
		for (size_t i {}; i < NUM_BUTTONS; i++)
			draw(*window, buttons[i]);
	}

	void isButtonClicked(float x, float y) {

		for (size_t i {}; i < NUM_BUTTONS; i++) {

			auto position = buttons[i].rect.getPosition();
			if ((x >= position.x && x <= position.x + buttons[i].width) &&
				(y >= position.y && y <= position.y + buttons[i].height)) {

				choice(buttons[i].code);
			}
		}
	}
}

int main() {

	init();

	//Flag that determines wether the algorithm should be run or not
	bool run {};
	
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
					run = true;

			if (event.type == sf::Event::MouseButtonPressed) {
			
				auto position {sf::Mouse::getPosition(*window)};
				isButtonClicked(position.x, position.y);
			}

			if (event.type == sf::Event::MouseButtonReleased)
				   node = nullptr;

			if (node == nullptr && event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				   sf::Vector2i position {sf::Mouse::getPosition(*window)};
	
				   for (size_t i {}; i < NUM_NODES; i++) {

						if (structures::isInside(nodes[i], position)) {

						 	node = &(nodes[i]);
							auto node_position {node->rect.getPosition()};
							mouse_x_diff = position.x - node_position.x;
							mouse_y_diff = position.y - node_position.y;
							break;
						 }
				   }
				}
			}

			if (node != nullptr) {

				sf::Vector2i position {sf::Mouse::getPosition(*window)};

				structures::setPosition(*node, position.x - mouse_x_diff, position.y - mouse_y_diff);
			}
		}

		window->clear(background);

		draw();
		structures::draw(*window, nodes, m);
/*
		if (run) {

			reset(my_path);

			choice(engine::BFS_CODE);


			run = false;
		}
*/
		window->display();
	}

	delete node;

	return 0;
}
