#include "structures.h"

namespace {

	   sf::Color node_bg(35, 40, 35);
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
		node.rect = rect;

		return node;
	}

	void setPosition(Node &node, float x, float y) {

		node.rect.setPosition(x, y);
	}

	bool isInside(const Node &node, const sf::Vector2i &position) {

		   const auto node_position = node.rect.getPosition();

		   if (position.x < node_position.x || position.x > node_position.x + node.width)
				 return false;
		   
		   if (position.y < node_position.y || position.y > node_position.y + node.height)
				 return false;

		   return true;
	}

	sf::Text makeText(const Node &node) {

		sf::Text text;
		text.setFont(font);
		text.setFillColor(sf::Color::Green);
		text.setOutlineColor(sf::Color::Green);
		text.setString(std::to_string(node.group) + "." + std::to_string(node.index) + ".");
		auto position = node.rect.getPosition();
		text.setPosition(position.x + 10, position.y + 2);

		return text;
	}
}
