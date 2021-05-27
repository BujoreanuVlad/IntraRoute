#include "engine.hpp"

float engine::time_link {1};

void engine::wait() {

	float t = std::clock();

	while ((std::clock() - t) / CLOCKS_PER_SEC < time_link);
}
