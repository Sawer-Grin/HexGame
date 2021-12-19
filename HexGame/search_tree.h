#ifndef _tree_search_
#define _tree_search_

#include <iostream>
#include <string> 
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>

#include "main_data.h"
#include "main_data_first.h"
#include "controller.h"
#include "graph.h"


enum start_position {nowhere = 0, middle = 1, down = 2, up = 3};
typedef struct piece_of_gold
{
	int hex;
	int size_ostov;
	start_position position;
};

class search_tree
{
public:
	search_tree(hex_gamer future_gamer, int size);
	int search_move_u_minmax(controller& controller);
	int search_move_u_aplha_beta(controller& controller);
	~search_tree();

protected:
	std::vector<hex_gamer> copy_of_field;
	hex_gamer computer;
	int best_move;
	std::vector<int> statistice;
	graph mind_graph;
	std::vector<int> meta_data;

private:
	int size;
	void update_data(controller& controller);
	void clear_statistice();
	std::set<int>& get_possible_move(int hex, hex_gamer local_player);
	int heuristic_function(hex_gamer local_player);
	int minmax(int node, int depth, hex_gamer local_gamer);
	int alpha_beta_algorithm(int node, int depth, int alpha, int beta, hex_gamer local_gamer);
	piece_of_gold find_a_piece_of_gold(const hex_gamer& local_player, const start_position& local_start);
	start_position analyze_start_position(const hex_gamer& local_player, bool store_data = false);
	inline std::vector<int>& analyze_start_position(const start_position& local_position);
	inline std::vector<int> for_middle_var(const hex_gamer& local_player, start_position& local_start);
	int possible_move(int hex, hex_gamer local_player, start_position local_start, std::set<int>& last_possible_position);
	bool check_neigbors(const int& first, const int& second, hex_gamer& local_player, int& warning_position);
	int generate_start_move(const hex_gamer& local_player, start_position& local_start);
	std::vector<int>& neighbors_hex(const int& first, const int& second);							// Method for getting a list of neighbors between two vertices
	void make_move(int hex, hex_gamer local_player);
	void print();
};

#endif