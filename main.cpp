#include <fstream>
#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "engine.hpp"
#include "structures.hpp"

const float width {1000};
const float height {800};
const size_t NUM_BUTTONS {4};

const size_t start_node {0};
const size_t end_node {3};

const int ADD_NODE_CODE {-1};
const int LOAD_PRESET_CODE {-2};

sf::RenderWindow *window;
sf::Font font;
extern const sf::Color background(70, 75, 70);
structures::Button buttons[NUM_BUTTONS];

size_t NUM_NODES;
int **m;
structures::Node *nodes;

namespace {

	void init() {

		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window = new sf::RenderWindow(sf::VideoMode(width, height), "Intranet path finder", sf::Style::Default, settings);
		engine::setTimeLink(0);

		if (!font.loadFromFile("Media/Fonts/Hack-Regular.ttf"))
			window->close();

		buttons[0] = structures::newButton(engine::DFS_CODE, "DFS");
		setPosition(buttons[0], width / 100, height / 80);

		buttons[1] = structures::newButton(engine::BFS_CODE, "BFS");
		//setPosition(buttons[1], 2 * width / 100 + buttons[0].width, height / 80);

		buttons[2] = structures::newButton(ADD_NODE_CODE, "Add node");

		buttons[3] = structures::newButton(LOAD_PRESET_CODE, "Load preset");

		for (size_t i {1}; i < NUM_BUTTONS; i++)
			setPosition(buttons[i], (i+1) * width/100 + i * buttons[0].width, height / 80);
	}	

	void loadPreset(const std::string &filename) {

		if (NUM_NODES) {

			for (size_t i {}; i < NUM_NODES; i++)
				delete[] m[i];
			delete[] m;

			delete[] nodes;

			NUM_NODES = 0;
		}

		std::ifstream in("Presets/" + filename + ".txt");
		if (!in) {
			in.close();
			return;
		}
		
		in >> NUM_NODES;

		m = new int*[NUM_NODES];
		nodes = new structures::Node[NUM_NODES];
		
		for (size_t i {}; i < NUM_NODES; i++) {

			m[i] = new int[NUM_NODES];

			for (size_t j {}; j < NUM_NODES; j++)
				in >> m[i][j];
		}

		for (size_t i {}; i < NUM_NODES; i++) {
			nodes[i] = structures::newNode(1, i+1);
			structures::setPosition(nodes[i], 50 + (i/5) * 120, 150 + (i%5) * 100);
		}

		in.close();
	}

	std::string prompt() {

		sf::RenderWindow promptWindow(sf::VideoMode(300, 100), "Prompt");
		std::string promptText {};
		sf::Text enteredText("", font);
		enteredText.setFillColor(sf::Color::Black);

		while (promptWindow.isOpen()) {

			sf::Event event;

			while (promptWindow.pollEvent(event)) {

				if (event.type == sf::Event::Closed)
					promptWindow.close();

				if (event.type == sf::Event::TextEntered) {

					if (event.text.unicode == static_cast<char>(13)) {
						promptWindow.close();
						return promptText;
					}

					if (event.text.unicode == '\b')
						promptText.erase(promptText.length() - 1, 1);
					else
						promptText += event.text.unicode;
					enteredText.setString(promptText.c_str());
				}
			}

			promptWindow.clear(sf::Color::White);

			promptWindow.draw(enteredText);

			promptWindow.display();
		}

		return promptText;
	}

	void choice(int code) {

		switch (code) {

			case engine::DFS_CODE: 
				structures::reset(NUM_NODES, nodes);
				engine::DFS(*window, NUM_NODES, nodes, m, end_node, start_node);
				break;
			case engine::BFS_CODE:
				structures::reset(NUM_NODES, nodes);
				engine::BFS(*window, NUM_NODES, nodes, m, end_node, start_node);
				break;

			case LOAD_PRESET_CODE:
				std::string filename = prompt();
				loadPreset(filename);
				break;
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

	loadPreset("Preset1");

	//node that is clicked
	//And initial coordinates of mouse
	structures::Node *node {nullptr};
	float mouse_x_diff, mouse_y_diff;

	while (window->isOpen()) {

		sf::Event event;

		while (window->pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window->close();

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
		structures::draw(*window, NUM_NODES, nodes, m);
		window->display();
	}

	delete node;

	return 0;
}
