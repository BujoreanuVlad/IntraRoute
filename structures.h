#if !defined STRUCTURES_H
#define STRUCTURES_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>

extern sf::Font font;

namespace structures {

	struct Node {

		float width, height;
		size_t group, index;
		sf::RectangleShape rect;
	};

	const float width {100};
	const float height {50};

	Node newNode(size_t group, size_t index, float w = width, float h = height);
	void setPosition(Node &node, float x, float y);
	template <size_t N>
	void draw(sf::RenderWindow &window, Node (&nodes)[N]) {
		
		for (size_t i {}; i < N; i++) {
			
			sf::Text text;
			text.setFont(font);
			text.setString(std::to_string(nodes[i].group) + "." + std::to_string(nodes[i].index) + ".");
			auto position = nodes[i].rect.getPosition();
			text.setPosition(position.x + 10, position.y + 2);

			window.draw(nodes[i].rect);
			window.draw(text);
		}
	}
}

#endif
