#include "structures.hpp"

extern const sf::Color background;

namespace {

	sf::Color node_bg(35, 40, 35);
	sf::Color Grey(175, 180, 175);
}

namespace structures {

	void Node::setPosition(float x, float y) {

		rect.setPosition(x, y);
		address.setPosition(x + width / 6, y + height / 10);
		order.setPosition(x, y - height / 2);
	}

	void Node::setOrder(sf::Text &text) {
		order = text;
	}

	bool Node::isInside(const sf::Vector2i &position) const {

		   const auto node_position = rect.getPosition();

		   if (position.x < node_position.x || position.x > node_position.x + width)
				 return false;
		   
		   if (position.y < node_position.y || position.y > node_position.y + height)
				 return false;

		   return true;
	}

	void Node::lightUp(const sf::Color color) {
		
		rect.setOutlineColor(color);
	}

	void Node::reset(size_t N, Node nodes[]) {

		for (size_t i {}; i < N; i++) {
			nodes[i].lightUp();
			nodes[i].order = sf::Text("", font);
		}
	}

	sf::Text Node::makeText() const {

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize((width + height) / 150 * 30);
		text.setFillColor(sf::Color::Green);
		text.setOutlineColor(sf::Color::Green);
		text.setString(std::to_string(group) + "." + std::to_string(index) + ".");
		auto position = this->rect.getPosition();
		text.setPosition(position.x + width / 6, position.y + height / 10);

		return text;
	}

	void Button::setPosition(float x, float y) {

		rect.setPosition(x, y);
		text.setPosition(x + width / 7, y);
	}

	void Button::setText(std::string text) {

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
		auto currentPosition = rect.getPosition();
		buttonText.setPosition(currentPosition.x - width / 5, currentPosition.y - height / 5);
		this->text = buttonText;
	}

	void Button::draw(sf::RenderWindow &window) {

		window.draw(rect);
		window.draw(text);
	}

	void Button::setTextFillColor(const sf::Color &color) {
		text.setFillColor(color);
	}

	void Node::draw(sf::RenderWindow &window, size_t N, Node nodes[], int **m) {

		for (size_t i {}; i < N; i++) {
            
            sf::Text text = nodes[i].address;

            //Drawing the rectangle and over it the text we just made
            window.draw(nodes[i].rect);
            window.draw(text);
            window.draw(nodes[i].order);
        }

        for (size_t i {}; i < N; i++) {

            //Drawing the connections between the nodes
            for (size_t j {}; j < i; j++) {

                if (m[i][j]) {
                    
                    auto positioni = nodes[i].rect.getPosition();
                    auto positionj = nodes[j].rect.getPosition();
                    sf::Vertex line[2] {sf::Vertex(sf::Vector2f(positioni.x, positioni.y), sf::Color::Green), sf::Vertex(sf::Vector2f(positionj.x, positionj.y + nodes[j].height), sf::Color::Red)};
					sf::Text cost(std::to_string(m[i][j]), font);
					cost.setCharacterSize(10);
					cost.setPosition((positioni.x + positionj.x) / 2,
									(positioni.y + positionj.y) / 2 + nodes[j].height / 2);
                    window.draw(line, 2, sf::Lines);
					window.draw(cost);
                }

				if (m[j][i]) {

                    auto positioni = nodes[i].rect.getPosition();
                    auto positionj = nodes[j].rect.getPosition();
                    sf::Vertex line[2] {sf::Vertex(sf::Vector2f(positioni.x + nodes[i].width, positioni.y), sf::Color::Red), sf::Vertex(sf::Vector2f(positionj.x + nodes[j].width, positionj.y + nodes[j].height), sf::Color::Green)};
					sf::Text cost(std::to_string(m[j][i]), font);
					cost.setCharacterSize(12);
					cost.setPosition((positioni.x + positionj.x) / 2 + (nodes[i].width + nodes[j].width) / 2,
									(positioni.y + positionj.y) / 2 + nodes[j].height / 2);
                    window.draw(line, 2, sf::Lines);
					window.draw(cost);
                }
            }
        }

        window.display();
    }
}
