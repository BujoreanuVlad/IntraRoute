#include <fstream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "engine.hpp"
#include "structures.hpp"
#include "graphicsEngine.hpp"

//Constants for the window
const float width {1000};
const float height {800};
extern const size_t NUM_BUTTONS {6};

//Button codes
const int FIND_PATH_CODE {-1};
const int ADD_NODE_CODE {-2};
const int REMOVE_NODE_CODE {-3};
const int LOAD_PRESET_CODE {-4};
const int SAVE_PRESET_CODE {-5};
const int SET_TIME_LINK_CODE {-6};

sf::RenderWindow *window;
sf::Font font;
extern const sf::Color background(62, 66, 62);
structures::Button buttons[NUM_BUTTONS];
std::string currentFile {""};

size_t NUM_NODES {};
int **m;
structures::Node *nodes;

namespace ge = graphicsEngine;

namespace {
	
	//bool value which tracks if the next node clicked should be removed or not
	bool removeNode {};

	//Initializes main window and all of its assets
	void init() {

		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window = new sf::RenderWindow(sf::VideoMode(width, height), "Intranet path finder", sf::Style::Default, settings);
		engine::setTimeLink(1);

		if (!font.loadFromFile("Media/Fonts/Hack-Regular.ttf"))
			window->close();

		buttons[0] = structures::Button {FIND_PATH_CODE, "Find path"};
		buttons[0].setPosition(width / 100, height / 80);

		buttons[1] = structures::Button {ADD_NODE_CODE, "Add node"};

		buttons[2] = structures::Button {REMOVE_NODE_CODE, "Remove node"};

		buttons[3] = structures::Button {LOAD_PRESET_CODE, "Load preset"};

		buttons[4] = structures::Button {SAVE_PRESET_CODE, "Save preset"};

		buttons[5] = structures::Button {SET_TIME_LINK_CODE, "Set time link"};

		for (size_t i {1}; i < NUM_BUTTONS; i++)
			buttons[i].setPosition((i+1) * width/100 + i * buttons[0].getWidth(), height / 80);
	}	

	//Loads a preset from the Presets/ folder
	void loadPreset(const std::string &filename) {

		if (NUM_NODES) {

			for (size_t i {}; i < NUM_NODES; i++)
				delete[] m[i];
			delete[] m;

			m = nullptr;

			delete[] nodes;
			nodes = nullptr;

			NUM_NODES = 0;
		}

		std::ifstream in("Presets/" + filename + ".txt");

		if (!in) {
			in.close();
			currentFile = "";
			return;
		}
		
		currentFile = filename;

		in >> NUM_NODES;

		m = new int*[NUM_NODES];
		nodes = new structures::Node[NUM_NODES];
		
		for (size_t i {}; i < NUM_NODES; i++) {

			m[i] = new int[NUM_NODES];

			for (size_t j {}; j < NUM_NODES; j++)
				in >> m[i][j];
		}

		for (size_t i {}; i < NUM_NODES; i++) {

			size_t group, index;
			float posX, posY;
			
			in >> group >> index;
			in >> posX >> posY;

			nodes[i] = structures::Node {group, index};
			nodes[i].setPosition(posX, posY);
		}

		in.close();
	}

	//Checks if a certain node address already exists
	bool checkIfValid(size_t group, size_t index) {

		for (size_t i {}; i < NUM_NODES; i++)
			if (nodes[i].getGroup() == group && nodes[i].getIndex() == index)
				return false;

		return true;
	}

	int getAlgorithm() {

		sf::RenderWindow algorithmWindow(sf::VideoMode(300, 600), "Select algorithm");
		structures::Button algorithms[3];
		int code {};

		algorithms[0] = structures::Button {engine::DFS_CODE, "DFS"};
		algorithms[0].setPosition(100, 100);

		algorithms[1] = structures::Button {engine::BFS_CODE, "BFS"};
		algorithms[1].setPosition(100, 200);

		algorithms[2] = structures::Button {engine::DIJKSTRA_CODE, "Dijkstra", 250};
		algorithms[2].setPosition(25, 300);

		while (algorithmWindow.isOpen()) {
		
			sf::Event event;

			while (algorithmWindow.pollEvent(event)) {

				if (event.type == sf::Event::Closed)
					algorithmWindow.close();

				if (event.type == sf::Event::MouseButtonPressed) {

					auto position {sf::Mouse::getPosition(algorithmWindow)};

					for (size_t i {}; i < 3; i++) {
						auto buttonPosition = algorithms[i].getRect().getPosition();
						if ((position.x >= buttonPosition.x && position.x <= buttonPosition.x + algorithms[i].getWidth()) &&
							(position.y >= buttonPosition.y && position.y <= buttonPosition.y + algorithms[i].getHeight())) {
							return algorithms[i].getCode();
						}
					}
				}
			}

			algorithmWindow.clear(sf::Color::White);
	
			for (size_t i {}; i < 3; i++)
				algorithms[i].draw(algorithmWindow);

			algorithmWindow.display();
		}

		return code;
	}

	//Chooses which action to perform based on the code of the button pressed
	void choice(int code) {

		switch (code) {

			case FIND_PATH_CODE:
				{
					std::string fromCode {ge::prompt()};
					std::string toCode {ge::prompt()};
					size_t from {}, to {};
					bool foundFrom {}, foundTo {};
		
					if (fromCode == "" || toCode == "")
						break;

					for (size_t i {}; !(foundFrom && foundTo) && i < NUM_NODES; i++) {
						if (nodes[i].getAddress().getString().toAnsiString() == fromCode) {
							from = i;
							foundFrom = true;
						}
						if (nodes[i].getAddress().getString().toAnsiString() == toCode) {
							to = i;
							foundTo = true;
						}
					}
					
					if (foundFrom && foundTo) {
						structures::Node::reset(NUM_NODES, nodes);
						ge::draw(*window, NUM_NODES, nodes, m);

						int algorithm_code {getAlgorithm()};
						switch (algorithm_code) {

							case engine::DFS_CODE: 
								engine::DFS(*window, NUM_NODES, nodes, m, from, to);
								break;

							case engine::BFS_CODE:
								engine::BFS(*window, NUM_NODES, nodes, m, from, to);
								break;

							case engine::DIJKSTRA_CODE:
								engine::Dijkstra(*window, NUM_NODES, nodes, m, from, to);
								break;
						}	
					}

					break;
				}

			case LOAD_PRESET_CODE:
				{
					std::string filename = ge::prompt();
					loadPreset(filename);
					break;
				}

			case SAVE_PRESET_CODE:
				{
					std::string filename = ge::prompt();
					if (filename == "")
						filename = currentFile;

					std::ofstream out("Presets/" + filename + ".txt");

					out << NUM_NODES << "\n\n";
					
					for (size_t i {}; i < NUM_NODES; i++) {
						for (size_t j {}; j < NUM_NODES; j++)
							out << m[i][j] << " ";
						out << "\n";
					}

					out << "\n";

					for (size_t i {}; i < NUM_NODES; i++) {
						auto position {nodes[i].getRect().getPosition()};
						out << nodes[i].getGroup() << " " << nodes[i].getIndex() << " " << position.x << " " << position.y << "\n";
					}

					out.close();
					break;
				}

			case ADD_NODE_CODE:
				{
					std::string node_group {ge::prompt()};
					if (node_group == "")
						break;
					std::string node_index {ge::prompt()};
					if (node_index == "")
						break;

					if (node_group.find_first_not_of("0123456789") != std::string::npos)
						break;
					if (node_index.find_first_not_of("0123456789") != std::string::npos)
						break;

					size_t group {static_cast<size_t> (std::stoi(node_group))};
					size_t index {static_cast<size_t> (std::stoi(node_index))};

					if (!checkIfValid(group, index))
						break;

					NUM_NODES++;
					structures::Node *buff = new structures::Node[NUM_NODES];
					int **m_buff = new int*[NUM_NODES];

					for (size_t i {}; i < NUM_NODES - 1; i++) {

						buff[i] = nodes[i];
						m_buff[i] = new int[NUM_NODES];

						for (size_t j {}; j < NUM_NODES - 1; j++)
							m_buff[i][j] = m[i][j];

						m_buff[i][NUM_NODES-1] = 0;

						if (m[i] != nullptr) {
							delete[] m[i];
							m[i] = nullptr;
						}
					}

					m_buff[NUM_NODES-1] = new int[NUM_NODES] {};
					if (m != nullptr) {
						delete[] m;
						m = nullptr;
					}
					m = m_buff;

					buff[NUM_NODES-1] = structures::Node {group, index};
					buff[NUM_NODES-1].setPosition(width/10, height/10);
					if (nodes != nullptr) {
						delete[] nodes;
						nodes = nullptr;
					}
					nodes = buff;
					break;
				}

			case REMOVE_NODE_CODE:
				removeNode = !removeNode;
				if (removeNode)
					buttons[2].setTextFillColor(sf::Color::Red);
				else
					buttons[2].setTextFillColor(sf::Color::White);
				break;

			case SET_TIME_LINK_CODE:
				{
					std::string newTimeLink = ge::prompt();
					if (newTimeLink == "")
						break;
					if (newTimeLink.find_first_not_of("0123456789.") == std::string::npos)
						engine::setTimeLink(std::stof(newTimeLink));
					break;
				}
		}
	}

	//Checks which button was clicked and performs its action
	void isButtonClicked(float x, float y) {

		for (size_t i {}; i < NUM_BUTTONS; i++) {

			auto position = buttons[i].getRect().getPosition();
			if ((x >= position.x && x <= position.x + buttons[i].getWidth()) &&
				(y >= position.y && y <= position.y + buttons[i].getHeight())) {

				choice(buttons[i].getCode());
				return;
			}
		}
	}
}

int main() {

	init();

	//node that is being dragged around the screen
	//And initial coordinates of mouse
	structures::Node *node {nullptr};
	float mouse_x_diff, mouse_y_diff;

	//Indices of From and To nodes for creating/deleting connections
	size_t from {}, to {};

	while (window->isOpen()) {

		sf::Event event;

		while (window->pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window->close();

			//Checks if a button was pressed
			if (event.type == sf::Event::MouseButtonPressed) {
			
				auto position {sf::Mouse::getPosition(*window)};
				isButtonClicked(position.x, position.y);
			}
	
			//Releases the node
			if (event.type == sf::Event::MouseButtonReleased)
				   node = nullptr;

			//Checks if a node was clicked and what to do with it
			if (node == nullptr && event.type == sf::Event::MouseButtonPressed) {

				//Either drags node around the screen or deletes it depending if the removeNode flag is set
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				   sf::Vector2i position {sf::Mouse::getPosition(*window)};
	
				   for (size_t i {}; i < NUM_NODES; i++) {

						if (nodes[i].isInside(position)) {

							if (removeNode) {

								structures::Node *buff = new structures::Node[NUM_NODES--];
								int **m_buff = new int*[NUM_NODES];
								for (size_t j {}; j <= NUM_NODES; j++) {
									
									if (j == i) {
										delete[] m[j];
										continue;
									}

									buff[j - static_cast<size_t>(j > i)] = nodes[j];
									m_buff[j - static_cast<size_t>(j > i)] = new int[NUM_NODES];

									for (size_t k {}; k <= NUM_NODES; k++)
										m_buff[j - static_cast<size_t>(j > i)][k - static_cast<size_t>(k > i)] = m[j][k];

									delete[] m[j];
								}

								delete[] nodes;
								delete[] m;

								nodes = buff;
								m = m_buff;
							}
							else {
								node = &(nodes[i]);
								auto node_position {node->getRect().getPosition()};
								mouse_x_diff = position.x - node_position.x;
								mouse_y_diff = position.y - node_position.y;
							}

							break;
						 }
				   }
				}

				//Creates link between nodes or deletes it if it already exists
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		
					auto position {sf::Mouse::getPosition(*window)};

					for (size_t i {}; i < NUM_NODES; i++) {

						if (nodes[i].isInside(position)) {

							if (!from) {
								from = i+1;
								nodes[i].lightUp(sf::Color::Magenta);
							}
							else {
								to = i+1;
								if (from != to) {
									//If connection already exists delete it
									if (m[from-1][to-1])
										m[from-1][to-1] = 0;
									//Otherwise create it
									else {
										std::string value {ge::prompt()};
										if (value.find_first_not_of("0123456789") == std::string::npos && value != "")
											m[from-1][to-1] = std::stoi(value);
									}
								}

								nodes[from-1].lightUp();
								from = 0;
								to = 0;
							}

							break;
						}
					}
				}
			}

			if (node != nullptr) {

				sf::Vector2i position {sf::Mouse::getPosition(*window)};

				node->setPosition(position.x - mouse_x_diff, position.y - mouse_y_diff);
			}

		}

		ge::draw(*window, NUM_NODES, nodes, m);
		window->display();
	}

	delete node;

	return 0;
}
