#if !defined ENGINE_H
#define ENGINE_HG
#include <vector>

namespace engine {

	template <size_t N>
	void DFS(std::vector<size_t> &v, int (&m)[N][N], const size_t end_node = 0, const size_t current_node = 0) {

		static int total_value {0};
		static unsigned int min_value {static_cast<unsigned>(~0)};
		static std::vector<size_t> current_path;

		if (current_node == end_node) {
			if (min_value > total_value) {
				min_value = total_value;
				v = current_path;
			}
			return;
		}

		for (int i {}; i < N; i++) {
		
			if (m[current_node][i] > 0) {

				total_value += m[current_node][i];

				if (total_value < min_value) {
					m[current_node][i] *= -1;
					current_path.push_back(i);

					DFS(v, m, end_node, i);

					current_path.pop_back();
					m[current_node][i] *= -1;
				}

				total_value -= m[current_node][i];
			}
		}
	}

	template <size_t N>
	void BFS(std::vector<size_t> &v, int (&m)[N][N], const size_t end_node, const size_t start_node) {
		
		int nodes[N] {};
		size_t *buff, *queue;
		size_t buff_size {}, queue_size {1};
		size_t index {};
		queue = new size_t[1];
		queue[0] = start_node;

		while (index < queue_size) {

			size_t current_node {queue[index]};

			for (size_t i {}; i < N; i++) {
				if (m[current_node][i]

	}
}
#endif
