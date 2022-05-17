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

	//Default width and height of the nodes' RectangleShape
	const float width {::width / 10};
	const float height {::height / 800 * 50};

	class Node {
	
		private:
			float width, height;
			size_t group, index;
			sf::RectangleShape rect;
			//The ip (intranet protocol) address of the code (in the format x.x.)
			sf::Text address;
			//Text that shows order of the path taken
			sf::Text order;
		public:
			Node() = default;
			Node (size_t g, size_t i, float w = structures::width, float h = structures::height);
			//Setting the position of said node
			void setPosition(float x, float y);
			void setOrder(sf::Text &text);
			//Checks if a certain coordinate is inside the rectangle defined by node's rect
			float getWidth() const { return width; }
			float getHeight() const { return height; }
			size_t getGroup() const { return group; }
			size_t getIndex() const { return index; }
			const sf::Text& getAddress() const { return address; }
			const sf::RectangleShape& getRect() const { return rect; }
			bool isInside(const sf::Vector2i &position) const;
			//Changes the color of the border
			void lightUp(const sf::Color color = sf::Color::Cyan);
			//Resets the colors of all the nodes to Cyan and sets the
			//text of each node to an empty text
			static void reset(size_t N, Node nodes[]); 
			//Helper function for drawing text
			sf::Text makeText() const;
			//Function to draw all the nodes on a RenderWindow window
			static void draw(sf::RenderWindow &window, size_t N, Node nodes[], int **m);
			
	};

	class Button {
		
		private:
			float width, height;
			int code;
			sf::Text text;
			sf::RectangleShape rect;

		public:
			Button() = default;
			Button (int code_, const char c[] = "", float w = structures::width, float h = structures::height);
			float getWidth() const { return width; }
			float getHeight() const { return height; }
			int getCode() const { return code; }
			const sf::RectangleShape& getRect() const { return rect; }
			void setPosition(float x, float y);
			//Sets the text on a button
			void setText(std::string text);
			void setTextFillColor(const sf::Color &color);
			//Draws a button
			void draw(sf::RenderWindow &window);
			
	};
}

#endif
