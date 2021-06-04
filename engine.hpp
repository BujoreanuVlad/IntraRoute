#if !defined ENGINE_H
#define ENGINE_HG
#include <vector>
#include <cstring>
#include "structures.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>

extern sf::Font font;

namespace engine {

	/*
	Time link variable which maps 1 real world second to a virtual time.
	For example a time_link variable of 1 would mean that the program waits 1
	real-life second for each action (when said action calls the wait() function).
	A time_link of 2 would wait 2 real-life seconds for each action, as if it 
	would take us 2 seconds for 1 virtual second to pass.
	*/
	extern float time_link;
	const sf::Color orange(252, 147, 0);
	
	//Macro codes for the algorithms declared in this file
	const int DFS_CODE {0};
	const int BFS_CODE {1};

	//Waits for the duration of the time_link variable which is measured in real-life seconds
	void wait();
	//Changes the time_link variable
	void setTimeLink(const float new_time_link = 1);
	//Shows the path graphically
	void showPath(sf::RenderWindow &window, structures::Node nodes[], const std::vector<size_t> &path);

	/*
	  I really shouldn't try to use things I don't understand

	//Template which takes as argument an algorithm (such as DFS or BFS) and uses it on a set of parameters
	template <typename Function, size_t N>
	void algorithm(Function func, sf::RenderWindow &window, std::vector<size_t> &v, structures::Node (&nodes)[N], int (&m)[N][N], const size_t end_node, const size_t start_node) {

		func(window, v, nodes, m, end_node, start_node);
	}
	*/

	//Actual DFS template which does all the work
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

	//Helper template to call from other translation units which automatically creates all the temporary variables needed.
	template <size_t N>
	void DFS(sf::RenderWindow &window, std::vector<size_t> &v, structures::Node (&nodes)[N], int (&m)[N][N], const size_t end_node = 0, const size_t current_node = 0) {

		int total_value {0};
		unsigned int min_value {static_cast<unsigned>(~0)};
		std::vector<size_t> current_path;
		bool visited[N] {};
		
		DFS(window, v, nodes, m, end_node, current_node, total_value, min_value, current_path, visited);
		structures::reset(nodes);
		showPath(window, nodes, v);
	}

	//Reconstructs the path from the BFS function
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

			if (current_node != end_node) {

				structures::lightUp(nodes[current_node], orange);
				structures::draw(window, nodes, m);
				wait();

				for (size_t i {}; i < N && current_node != end_node; i++) {

					if (i == start_node)
						continue;

					//If there is path to a node check if it's a good option
					if (m[current_node][i]) {

						//The cost of going to node i from current_node
						int sum {m[current_node][i] + node_values[current_node]};

						//If conditions apply add node to queue
						if (sum < node_values[i] || !node_values[i]) {
							node_values[i] = sum;
							buff[buff_size++] = i;
						}
					}
				}
			}

			if (index == queue_size) {

				//Coloring all the nodes to be checked
				for (size_t i {}; i < buff_size; i++) {
					structures::lightUp(nodes[buff[i]], sf::Color::Yellow);
					structures::draw(window, nodes, m);
					wait();
				}

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
		structures::reset(nodes);
		showPath(window, nodes, v);
	}
}
#endif
