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
}
