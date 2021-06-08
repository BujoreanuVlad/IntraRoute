#if !defined STRUCTURES_H
#define STRUCTURES_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>

//Getting the font from the main.cpp file
extern sf::Font font;
extern const float width;
extern const float height;

namespace structures {

	struct Node {

		float width, height;
		size_t group, index;
		sf::RectangleShape rect;
		//The ip (intranet protocol) address of the code (in the format x.x)
		sf::Text address;
		//Text that shows order of the path taken
		sf::Text order;
	};

	struct Button {

		float width, height;
		int code;
		sf::Text text;
		sf::RectangleShape rect;
	};

	struct Edge {

		   Node *from, *to;
		   int cost;
	};

	//Default width and height of the nodes' RectangleShape
	const float width {::width / 10};
	const float height {::height / 800 * 50};

	//A constructor for making new Node structs.
	//Don't ask me why I don't use a class instead
	Node newNode(size_t group, size_t index, float w = width, float h = height);
	//Setting the position of said node
	void setPosition(Node &node, float x, float y);
	//Checks if a certain coordinate is inside the rectangle defined by node's rect
	bool isInside(const Node &node, const sf::Vector2i &position);
	//Changes the color of the border
	void lightUp(Node &node, const sf::Color color = sf::Color::Cyan);
	//Resets the colors of all the nodes to Cyan and sets the
	//text of each node to an empty text
	void reset(size_t N, Node nodes[]); 
	
	Edge newEdge(Node *from, Node *to, int cost);

	//Constructor, you already know the drill
	Button newButton(int code, const char c[] = "", float w = width, float h = height);
	void setPosition(Button &button, float x, float y);
	//Sets the text on a button
	void setText(Button &button, std::string text);
	//Draws a button
	void draw(sf::RenderWindow &window, Button &button);

	//Helper function for drawing text
	sf::Text makeText(const Node &node);
	//Function to draw all the nodes on a RenderWindow window
	void draw(sf::RenderWindow &window, size_t N, Node nodes[], int **m);
}

#endif
