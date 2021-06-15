#include "engine.hpp"
#include <iostream>

namespace ge = graphicsEngine;

namespace engine {

	float time_link {1};

	void setTimeLink(const float new_time_link) {

		time_link = new_time_link;
	}

	void wait() {

		float t = std::clock();

		while ((std::clock() - t) / CLOCKS_PER_SEC < time_link);
	}

	namespace {
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

		void failPath(sf::RenderWindow &window, structures::Node &start, structures::Node &end) {

			structures::lightUp(start, sf::Color::Red);
			structures::lightUp(end, sf::Color::Red);
		}
	}

	void DFS(sf::RenderWindow &window, std::vector<size_t> &v, size_t N, structures::Node nodes[], int **m, const size_t current_node, const size_t end_node, int &total_value, unsigned int &min_value, std::vector<size_t> &current_path, bool visited[]) {

        structures::lightUp(nodes[current_node], orange);
        current_path.push_back(current_node);
        ge::draw(window, N, nodes, m);

        wait();

        if (current_node == end_node) {

            if (min_value > total_value) {
                min_value = total_value;
                v = current_path;
            }

            structures::lightUp(nodes[current_node]);
            current_path.pop_back();
            ge::draw(window, N, nodes, m);
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

                    DFS(window, v, N, nodes, m, i, end_node, total_value, min_value, current_path, visited);

                    structures::lightUp(nodes[current_node], orange);
                    m[current_node][i] *= -1;
                    m[i][current_node] *= -1;

                    ge::draw(window, N, nodes, m);
                    wait();
                }

                total_value -= m[current_node][i];
            }

        }

        visited[current_node] = false;
        structures::lightUp(nodes[current_node]);
        current_path.pop_back();
        ge::draw(window, N, nodes, m);
    }

	void DFS(sf::RenderWindow &window, size_t N, structures::Node nodes[], int **m, const size_t start_node, const size_t end_node) {

        int total_value {0};
        unsigned int min_value {static_cast<unsigned>(~0)};
        std::vector<size_t> current_path;
		std::vector<size_t> v;
        bool *visited = new bool[N] {};

        DFS(window, v, N, nodes, m, start_node, end_node, total_value, min_value, current_path, visited);
        structures::reset(N, nodes);
		if (v.empty())
			failPath(window, nodes[start_node], nodes[end_node]);
		else 
	        showPath(window, nodes, v);
    }
	namespace {
		void reconstruct_path(std::vector<size_t> &v, size_t N, int nodes[], int **m, const size_t current_node, const size_t start_node) {

			if (current_node == start_node) {
				v.push_back(current_node);
				return;
			}

			for (size_t i {}; i < N; i++) {

				if (i == current_node)
					continue;

				if (nodes[i] + m[i][current_node] == nodes[current_node]) {
					reconstruct_path(v, N, nodes, m, i, start_node);
					v.push_back(current_node);
					return;
				}
			}
		}

		void reconstruct_path(std::vector<size_t> &v, size_t previous[], size_t current_node) {
			
			if (!previous[current_node]) {
				v.push_back(current_node);
				return;
			}

			reconstruct_path(v, previous, previous[current_node]-1);
			v.push_back(current_node);
		}
	}

	void BFS(sf::RenderWindow &window, size_t N, structures::Node nodes[], int **m, const size_t start_node, const size_t end_node) {

		unsigned int total_sum {static_cast<unsigned> (~(1 << 31))};
		int *node_values = new int[N] {};
		std::vector<size_t> v;
        size_t *buff, *queue;
        size_t buff_size {}, queue_size {1};
        size_t index {};
		bool pathExists {};
        queue = new size_t[1];
        queue[0] = start_node;
        buff = new size_t[N];

        while (index < queue_size) {

            size_t current_node {queue[index++]};

            if (current_node != end_node) {

                structures::lightUp(nodes[current_node], orange);
                graphicsEngine::draw(window, N, nodes, m);
                wait();

                for (size_t i {}; i < N && current_node != end_node; i++) {

                    if (i == start_node)
                        continue;

                    //If there is path to a node check if it's a good option
                    if (m[current_node][i]) {

                        //The cost of going to node i from current_node
                        int sum {m[current_node][i] + node_values[current_node]};

                        //If conditions apply add node to queue
                        if ((sum < node_values[i] || !node_values[i]) && sum < total_sum) {
                            node_values[i] = sum;
                            buff[buff_size++] = i;
                        }
                    }
                }
            }

			else {
				pathExists = true;
				total_sum = node_values[current_node];
			}

            structures::lightUp(nodes[current_node], sf::Color::Red);
            graphicsEngine::draw(window, N, nodes, m);
            wait();

            if (index == queue_size) {

                //Coloring all the nodes to be checked
                for (size_t i {}; i < buff_size; i++) {
                    structures::lightUp(nodes[buff[i]], sf::Color::Yellow);
                    graphicsEngine::draw(window, N, nodes, m);
                    wait();
                }

                //Reseting the color
                for (size_t i {}; i < buff_size; i++)
                    structures::lightUp(nodes[buff[i]]);

				graphicsEngine::draw(window, N, nodes, m);
				wait();

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

		structures::reset(N, nodes);
		if (pathExists) {
			reconstruct_path(v, N, node_values, m, end_node, start_node);
			showPath(window, nodes, v);
		}
		else
			failPath(window, nodes[start_node], nodes[end_node]);
    }	

	namespace {
		struct pair {
			size_t index;
			int distance;
		};

		pair poll(std::vector<pair> &v) {
			
			pair min {v[0]};
			size_t index {0};

			for (size_t i {1}; i < v.size(); i++) {
				if (v[i].distance < min.distance) {
					min = v[i];
					index = i;
				}
			}

			v.erase(v.begin() + index);

			return min;
		}
	}

	void Dijkstra(sf::RenderWindow &window, size_t N, structures::Node nodes[], int **m, const size_t start_node, const size_t end_node) {

		const int MAX {~(1 << 31)};

		int *nodeValues = new int [N];
		bool *visited = new bool[N] {};
		size_t *previous = new size_t[N] {};
		std::vector<pair> pq;
		{
			pair p;
			p.index = start_node;
			p.distance = 0;
			pq.push_back(p);
		}

		for (size_t i {}; i < N; i++)
			nodeValues[i] = MAX;
		nodeValues[start_node] = 0;

		while (!pq.empty()) {

			pair buff = poll(pq);			
			visited[buff.index] = true;
			structures::lightUp(nodes[buff.index], orange);
			ge::draw(window, N, nodes, m);
			wait();
			if (buff.distance > nodeValues[buff.index])
				continue; 

			for (size_t i {}; i < N; i++) {

				if (visited[i] || !m[buff.index][i])
					continue;

				int newDist {m[buff.index][i] + nodeValues[buff.index]};
				if (newDist < nodeValues[i]) {
					nodeValues[i] = newDist;
					previous[i] = buff.index+1;
					pair p;
					p.index = i;
					p.distance = newDist;
					pq.push_back(p);
					structures::lightUp(nodes[i], sf::Color::Yellow);
					ge::draw(window, N, nodes, m);
					wait();
				}
			}

			structures::lightUp(nodes[buff.index], sf::Color::Red);
			ge::draw(window, N, nodes, m);
			wait();

			if (buff.index == end_node)
				break;
		}

		delete[] nodeValues;

		std::vector<size_t> path;
		structures::reset(N, nodes);

		if (visited[end_node])
			reconstruct_path(path, previous, end_node);
		else
			failPath(window, nodes[start_node], nodes[end_node]);

		delete[] visited;
		delete[] previous;
		showPath(window, nodes, path);
	}
}
