#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <string>
#include <math.h>
#include <set>

#include "main_data_first.h"
#include "priority_queue.h"


#if !defined(temp_h_graph)
#define temp_h_graph

static struct edge {
	int first;
	int second;
	int value;
};

inline bool compar(edge& first, edge& second)
{
	return first.value < second.value;
}

class graph {
public:
	graph() : vertices(0), edges(0) {}
	graph(const int& some_vertices);															// Class constructor 
	graph(const graph& g);                                                                      // Constructor for copying a class object
	graph(const std::string& some_name);														// add constructor
	graph& operator = (const graph& g);                                                         // Operator overloading
	~graph();                                                                                   // Class destructor 
	int  E() { return edges; }                                                                  // Method for getting the number of edges in a graph
	int  V() { return vertices; }                                                               // Method for getting the number of vertices in a graph
	bool adjacent(const int& x, const int& y, const int& value);                                // Method for getting information about an edge between vertices
	std::list<int> neighbors(const int& x, const int& value);                                   // Method for getting a list of neighbors
	int get_node_value(const int& x);                                                           // Method for obtaining information about the number of edges at a edge
	int get_edge_value(const int& x, const int& y) { return matrix_graph[x][y]; }				// Method for getting edge value
	void set_edge_value(const int& x, const int& y, const int& value);                          // Method for setting the desired edge value
	void for_testing();                                                                         // Test Method (delete) Console 
	std::list<std::pair<int, int>> prim(const int& a, const int& value);                        // add method
	std::vector<int> kruscal(const int& value);													// added additionally, but not working yet
	std::list<std::pair<int, int>> boruvka(const int& value);
	bool is_conected(const int& u, const int& v, const int& value);
	std::list<int> path(const int& first_vartices, const int& second_vertices,
		const int& value);
	void add_links(const int& hex, const std::vector<hex_gamer>& field_of_play);                      // methd to adding links for special classes Controller, mind
	void clear_edges();																			// specific algprithm for hex_game
	std::vector<int> search_border_hex_up(const hex_gamer& local_player, std::vector<hex_gamer>& field);
	std::vector<int> search_border_hex_down(const hex_gamer& local_player, std::vector<hex_gamer>& field);
	hex_gamer who_won(const hex_gamer& player, std::vector<hex_gamer>& field, bool work_brain = false);
	inline std::vector<int>& get_upper_ends() { return upper_hex; }
	inline void clear_upper_ends() { upper_hex.clear(); }
	inline std::vector<int>& get_lower_ends() { return lower_hex; }
	inline void clear_lower_ends() { lower_hex.clear(); }
	std::pair<int, int> get_last_edge(const hex_gamer& local_player);

private:

	int vertices;                                                                               // Number of vertices in the graph
	int edges;                                                                                  // The number of edges in the graph
	std::vector<std::vector<int>> matrix_graph;                                                 // array of edge values
	std::vector<edge> array_vertex;															// vector for edge( using in some algorithm for optimization)
	int ostov_cost;                                                                             // cost of ostov 'tree' 
	int path_cost;
	std::vector<int> upper_hex;
	std::vector<int> lower_hex;

	std::vector <int> tree_vertices;                                                            // data for Union-Find algorithm

	int find(int& v);                                                                           // method for Union-Find algorithm
	void make_set(int& v);
	void unite(int& v, int& u);
};

#endif