#include "structures.hpp"

extern const sf::Color background;

namespace {

	sf::Color node_bg(35, 40, 35);
	sf::Color Grey(175, 180, 175);
}

namespace structures {

	Node newNode(size_t group, size_t index, float width, float height) {

		Node node;

		node.group = group;
		node.index = index;
		node.width = width;
		node.height = height;
		sf::RectangleShape rect(sf::Vector2f(width, height));
		rect.setOutlineColor(sf::Color::Cyan);
		rect.setOutlineThickness(2);
		rect.setFillColor(node_bg);
		rect.setPosition(sf::Vector2f(0, ::height/9));
		node.rect = rect;

		return node;
	}

	void setPosition(Node &node, float x, float y) {

		node.rect.setPosition(x, y);
		node.order.setPosition(x, y - node.height / 2);
	}

	bool isInside(const Node &node, const sf::Vector2i &position) {

		   const auto node_position = node.rect.getPosition();

		   if (position.x < node_position.x || position.x > node_position.x + node.width)
				 return false;
		   
		   if (position.y < node_position.y || position.y > node_position.y + node.height)
				 return false;

		   return true;
	}

	void lightUp(Node &node, const sf::Color color) {
		
		node.rect.setOutlineColor(color);
	}

	void reset(size_t N, Node nodes[]) {

		for (size_t i {}; i < N; i++) {
			lightUp(nodes[i]);
			nodes[i].order = sf::Text();
		}
	}

	sf::Text makeText(const Node &node) {

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize((width + height) / 150 * 30);
		text.setFillColor(sf::Color::Green);
		text.setOutlineColor(sf::Color::Green);
		text.setString(std::to_string(node.group) + "." + std::to_string(node.index) + ".");
		auto position = node.rect.getPosition();
		text.setPosition(position.x + width / 6, position.y + height / 10);

		return text;
	}

	Edge newEdge(Node *from, Node *to, int cost) {

		   Edge edge;

		   edge.from = from;
		   edge.to = to;
		   edge.cost = cost;

		   return edge;
	}

	Button newButton(int code, const char c[], float width, float height) {

		Button button;

		button.width = width;
		button.height = height;
		button.code = code;
		button.rect = sf::RectangleShape(sf::Vector2f(width, height));
		button.rect.setFillColor(background);
		setText(button, c);

		return button;
	}

	void setPosition(Button &button, float x, float y) {

		button.rect.setPosition(x, y);
		button.text.setPosition(x + button.width / 7, y);
	}

	void setText(Button &button, std::string text) {

		size_t words {1};
		unsigned int max_word_length {};

		for (size_t i {text.find_first_of(" ")}; i < text.length();) {

			text.replace(i, 1, "\n");
			words++;
			i = text.find_first_of(" ", i);
		}

		sf::Text buttonText(text, font);
		//buttonText.setCharacterSize((30*4) / text.length());
		buttonText.setCharacterSize(40 / words);
		auto currentPosition = button.rect.getPosition();
		buttonText.setPosition(currentPosition.x - button.width / 5, currentPosition.y - button.height / 5);
		button.text = buttonText;
	}

	void draw(sf::RenderWindow &window, Button &button) {

		window.draw(button.rect);
		window.draw(button.text);
	}

	void draw(sf::RenderWindow &window, size_t N, Node nodes[], int **m) {

		for (size_t i {}; i < N; i++) {
            
            //Making the text over the rectangle for each node
            sf::Text text = makeText(nodes[i]);

            //Drawing the rectangle and over it the text we just made
            window.draw(nodes[i].rect);
            window.draw(text);
            window.draw(nodes[i].order);
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
