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
	const int DFS_CODE {1};
	const int BFS_CODE {2};

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

	//Actual DFS function which does all the work
	void DFS(sf::RenderWindow &window, std::vector<size_t> &v, size_t N, structures::Node nodes[], int **m, const size_t end_node, const size_t current_node, int &total_value, unsigned int &min_value, std::vector<size_t> &current_path, bool visited[]); 

	//Helper function to call from other translation units which automatically creates all the temporary variables needed.
	void DFS(sf::RenderWindow &window, size_t N, structures::Node nodes[], int **m, const size_t end_node = 0, const size_t current_node = 0);

	//Reconstructs the path from the BFS function
	void reconstruct_path(std::vector<size_t> &v, size_t N, int nodes[], int **m, const size_t current_node = 0, const size_t start_node = 0);

	void BFS(sf::RenderWindow &window, size_t N, structures::Node nodes[], int **m, const size_t end_node = 0, const size_t start_node = 0);
}
#endif
