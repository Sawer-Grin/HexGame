#include "controller.h" 
#include "main_data.h"
#include "main_data_first.h"

#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <shared_mutex>

class method_monte
{
public:
	method_monte(): computer(first), size_field(0){}
	method_monte(const hex_gamer& some_player, const int& size);
	method_monte(const method_monte& monte_carlo);
	method_monte& operator = (const method_monte& method);
	~method_monte();
	int simulation(controller& control);																							// main algorithm for simulation next step at computer
	int best_move();																												// return best move

protected: 
	std::vector<int> field_of_statistic;																							// array for storing win statistics
	hex_gamer computer;
	int size_field;

private:
	int next_step();																												// return best move
	void copy_data(std::vector<std::pair<hex_gamer, int>>& history_game, std::vector<hex_gamer>& field_of_node);					// copying the data of the current game information
	void fifty_fifty(std::vector<hex_gamer>& field_of_node);
	void convert(std::vector<hex_gamer>& field_game, graph& mind_graph, std::vector<hex_gamer>& field_of_node);						// transfer of data from array field_of_node to mind_graph
	std::list<int> finding_way(graph& mind_graph, const hex_gamer& local_gamer);													// algorithm for finding a track of hexes
	void print(std::vector<hex_gamer>& field_of_statistic);																			// test algorithm 
	void clear_statistic();																											// helper function
	int simulation_one_thread(std::vector<hex_gamer> field_game, graph& mind_graph,	std::vector<hex_gamer>& field_of_node, std::vector<std::pair<hex_gamer, int>> history, int repeating);

	std::shared_mutex mtx;
	std::shared_mutex mtx2;
	std::shared_mutex mtx3;
};