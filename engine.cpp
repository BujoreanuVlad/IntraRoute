#include "engine.hpp"

namespace engine {

	float time_link {1};

	void setTimeLink(const float new_time_link) {

		time_link = new_time_link;
	}

	void wait() {

		float t = std::clock();

		while ((std::clock() - t) / CLOCKS_PER_SEC < time_link);
	}

	void showPath(sf::RenderWindow &window, structures::Node nodes[], const std::vector<size_t> &path) {

		for (size_t i{}; i < path.size(); i++) {

			structures::Node &tmp {nodes[path[i]]};
			structures::lightUp(tmp, sf::Color::Green);
			sf::Text text(std::to_string(i+1), font);
			auto position = tmp.rect.getPosition();
			text.setPosition(position.x, position.y - tmp.height / 2);
			text.setFillColor(sf::Color::Green);
			text.setCharacterSize((tmp.width + tmp.height) / 150 * 22.5);
			tmp.order = text;
		}
	}
}
