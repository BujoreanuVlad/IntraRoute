#if !defined ENGINE_H
#define ENGINE_HG
#include <vector>
#include "structures.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

namespace engine {

	float time_link {1};

	template <size_t N>
	void DFS(sf::RenderWindow &window, std::vector<size_t> &v, structures::Node (&nodes)[N], int (&m)[N][N], const size_t end_node = 0, const size_t current_node = 0) {

		static int total_value {0};
		static unsigned int min_value {static_cast<unsigned>(~0)};
		static std::vector<size_t> current_path;

		structures::lightUp(nodes[current_node], sf::Color::Red);
		current_path.push_back(current_node);
		structures::draw(window, nodes, m);

		{
			float t = std::clock();

			while ((std::clock() - t) / CLOCKS_PER_SEC < time_link);
		}

		if (current_node == end_node) {

			if (min_value > total_value) {
				min_value = total_value;
				v = current_path;
			}

			structures::lightUp(nodes[current_node]);
			current_path.pop_back();
			return;
		}

		for (int i {}; i < N; i++) {
		
			if (m[current_node][i] > 0) {

				total_value += m[current_node][i];

				if (total_value < min_value) {
					m[current_node][i] *= -1;
					m[i][current_node] *= -1;

					DFS(window, v, nodes, m, end_node, i);

					m[current_node][i] *= -1;
					m[i][current_node] *= -1;
				}

				total_value -= m[current_node][i];
			}
		}

		structures::lightUp(nodes[current_node]);
		current_path.pop_back();
		structures::draw(window, nodes, m);
		{
			float t = std::clock();

			while ((std::clock() - t) / CLOCKS_PER_SEC < time_link);
		}
	}

	template <size_t N>
	void reconstruct_path(std::vector<size_t> &v, int (&nodes)[N], int (&m)[N][N], const size_t current_node = 0, const size_t start_node = 0) {

		if (current_node == start_node) {
			v.push_back(current_node);
			return;
		}

		for (size_t i {}; i < N; i++) {

			if (i == current_node)
				continue;

			if (nodes[i] + m[i][current_node] == nodes[current_node]) {
				reconstruct_path(v, nodes, m, i, start_node);
				v.push_back(current_node);
				return;
			}
		}
	}

	template <size_t N>
	void BFS(std::vector<size_t> &v, int (&m)[N][N], const size_t end_node = 0, const size_t start_node = 0) {
		
		int nodes[N] {};
		size_t *buff, *queue;
		size_t buff_size {}, queue_size {1};
		size_t index {};
		queue = new size_t[1];
		queue[0] = start_node;
		buff = new size_t[N];

		while (index < queue_size) {

			size_t current_node {queue[index++]};

			for (size_t i {}; i < N; i++) {

				if (i == start_node)
					continue;

				int sum {m[current_node][i] + nodes[current_node]};
				if (sum < nodes[i] || !nodes[i]) {
					nodes[i] = sum;
					buff[buff_size++] = i;
				}
			}

			if (index == queue_size) {

				delete[] queue;
				queue = buff;
				queue_size = buff_size;
				index = 0;

				buff = new size_t[N];
				buff_size = 0;
			}
		}
		
		delete[] queue;
		delete[] buff;

		reconstruct_path(v, nodes, m, end_node, start_node);
	}
}
#endif
