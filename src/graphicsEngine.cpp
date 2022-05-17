#include "graphicsEngine.hpp"
#include <iostream>

namespace graphicsEngine {

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

        return "";
    }

    void draw(sf::RenderWindow &window, size_t NUM_NODES, structures::Node nodes[], int **m) {

        window.clear(background);

        sf::RectangleShape topBar(sf::Vector2f(width, height / 10));
        topBar.setPosition(0, 0);

        window.draw(topBar);
        for (size_t i {}; i < NUM_BUTTONS; i++) {
            buttons[i].draw(window);
		}

		structures::Node::draw(window, NUM_NODES, nodes, m);
    }
}
