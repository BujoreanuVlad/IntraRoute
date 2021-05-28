#if !defined ENGINE_H
#define ENGINE_HG
#include <vector>
#include "structures.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

namespace engine {

	extern float time_link;
	const sf::Color orange(252, 147, 0);
	
	//Waits for the duration of the time_link variable which is measured in real-life seconds
	void wait();
	//Resets all the static variables from the DFS template
	void reset();

	//Helper template for the other DFS template which is the one which will be called by the other translation units 
	template <size_t N>
	void DFS(sf::RenderWindow &window, std::vector<size_t> &v, structures::Node (&nodes)[N], int (&m)[N][N], const size_t end_node, const size_t current_node, int &total_value, unsigned int &min_value, std::vector<size_t> &current_path, bool (&visited)[N]) {

		structures::lightUp(nodes[current_node], orange);
		current_path.push_back(current_node);
		structures::draw(window, nodes, m);

		wait();

		if (current_node == end_node) {

			if (min_value > total_value) {
				min_value = total_value;
				v = current_path;
			}

			structures::lightUp(nodes[current_node]);
			current_path.pop_back();
			structures::draw(window, nodes, m);
			return;
		}

		visited[current_node] = true;

		for (int i {}; i < N; i++) {
		
			if (m[current_node][i] > 0) {

				total_value += m[current_node][i];

				if (!visited[i] && total_value < min_value) {
					m[current_node][i] *= -1;
					m[i][current_node] *= -1;
					structures::lightUp(nodes[current_node], sf::Color::Red);

					DFS(window, v, nodes, m, end_node, i, total_value, min_value, current_path, visited);

					structures::lightUp(nodes[current_node], orange);					
					m[current_node][i] *= -1;
					m[i][current_node] *= -1;

					structures::draw(window, nodes, m);
					wait();
				}

				total_value -= m[current_node][i];
			}

			visited[current_node] = false;
		}

		structures::lightUp(nodes[current_node]);
		current_path.pop_back();
		structures::draw(window, nodes, m);
	}

	template <size_t N>
	void DFS(sf::RenderWindow &window, std::vector<size_t> &v, structures::Node (&nodes)[N], int (&m)[N][N], const size_t end_node = 0, const size_t current_node = 0) {

		int total_value {0};
		unsigned int min_value {static_cast<unsigned>(~0)};
		std::vector<size_t> current_path;
		bool visited[N] {};
		
		DFS(window, v, nodes, m, end_node, current_node, total_value, min_value, current_path, visited);
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
	void BFS(sf::RenderWindow &window, std::vector<size_t> &v, structures::Node (&nodes)[N], int (&m)[N][N], const size_t end_node = 0, const size_t start_node = 0) {
		
		int node_values[N] {};
		size_t *buff, *queue;
		size_t buff_size {}, queue_size {1};
		size_t index {};
		queue = new size_t[1];
		queue[0] = start_node;
		buff = new size_t[N];

		while (index < queue_size) {

			size_t current_node {queue[index++]};

			structures::lightUp(nodes[current_node], orange);
			structures::draw(window, nodes, m);
			wait();

			for (size_t i {}; i < N && current_node != end_node; i++) {

				if (i == start_node)
					continue;

				int sum {m[current_node][i] + node_values[current_node]};
				if (sum < node_values[i] || !node_values[i]) {
					node_values[i] = sum;
					buff[buff_size++] = i;
					structures::lightUp(nodes[i], sf::Color::Yellow);
					structures::draw(window, nodes, m);
					wait();
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

			structures::lightUp(nodes[current_node], sf::Color::Red);
			structures::draw(window, nodes, m);
			wait();
		}
		
		delete[] queue;
		delete[] buff;

		reconstruct_path(v, node_values, m, end_node, start_node);
	}
}
#endif
