#if !defined STRUCTURES_H
#define STRUCTURES_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>

//Getting the font from the main.cpp file
extern sf::Font font;

namespace structures {

	struct Node {

		float width, height;
		size_t group, index;
		sf::RectangleShape rect;
	};

	struct Edge {

		   Node *from, *to;
		   int cost;
	};

	//Default width and height of the nodes' RectangleShape
	const float width {100};
	const float height {50};

	//A constructor for making new Node structs.
	//Don't ask me why I don't use a class instead
	Node newNode(size_t group, size_t index, float w = width, float h = height);
	//Setting the position of said node
	void setPosition(Node &node, float x, float y);
	//Checks if a certain coordinate is inside the rectangle defined by node's rect
	bool isInside(const Node &node, const sf::Vector2i &position);
	//Changes the color of the border
	void lightUp(Node &node, const sf::Color color = sf::Color::Cyan);
	//Resets the colors of all the nodes to Cyan
	template <size_t N>
	void reset(Node (&nodes)[N]) {

		for (size_t i {}; i < N; i++)
			lightUp(nodes[i]);
	}
	
	Edge newEdge(Node *from, Node *to, int cost);

	//Helper function for drawing text
	sf::Text makeText(const Node &node);
	//Template to draw all the nodes on a RenderWindow window
	template <size_t N>
	void draw(sf::RenderWindow &window, Node (&nodes)[N], int (&m)[N][N]) {
		
		for (size_t i {}; i < N; i++) {
			
			//Making the text over the rectangle for each node
			sf::Text text = makeText(nodes[i]);

			//Drawing the rectangle and over it the text we just made
			window.draw(nodes[i].rect);
			window.draw(text);
		}

		for (size_t i {}; i < N; i++) {

			//Drawing the connections between the nodes
			for (size_t j {}; j < N; j++) {

				if (m[i][j]) {
					
					auto positioni = nodes[i].rect.getPosition();
					auto positionj = nodes[j].rect.getPosition();
					sf::Vertex line[2] {sf::Vertex(sf::Vector2f(positioni.x + nodes[i].width, positioni.y + nodes[i].height), sf::Color::Red), sf::Vertex(sf::Vector2f(positionj.x, positionj.y), sf::Color::Blue)};
					window.draw(line, 2, sf::Lines);
				}
			}
		}

		window.display();
	}
}

#endif
