#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <list>
#include <cassert>
#include <thread>
#include <mutex>

#include "method_monte.h"

method_monte::method_monte(const hex_gamer& some_player, const int& size)
{
	computer = some_player;
	field_of_statistic.resize(size * size);
	size_field = size;
}

method_monte::method_monte(const method_monte& monte_carlo)
{
	this->size_field = monte_carlo.size_field;
	field_of_statistic = monte_carlo.field_of_statistic;
	computer = monte_carlo.computer;
}

method_monte& method_monte::operator=(const method_monte& monte_carlo)
{
	this->size_field = monte_carlo.size_field;
	this->field_of_statistic.resize(this->size_field * size_field);
	this->field_of_statistic = monte_carlo.field_of_statistic;
	this->computer = monte_carlo.computer;
	return *this;
}

method_monte::~method_monte()
{
	if (field_of_statistic.size() > 0) {
		field_of_statistic.clear();
		size_field = 0;
	}
}

//	procedure for simulating a game of two players
void method_monte::copy_data(std::vector<std::pair<hex_gamer, int>>& history_game, std::vector<hex_gamer>& field_of_node)
{
	for (auto& it : history_game) {
		field_of_node[it.second] = it.first;
	}
}

// procedure for filling the array with first and second player hexes
void method_monte::fifty_fifty(std::vector<hex_gamer>& field_of_node)
{
	int size = field_of_node.size();
	for (int i = 0; i < round(size / 2); ++i) {
		field_of_node[i] = (hex_gamer)first;
		field_of_node[size - 1 - i] = (hex_gamer)second;
	}
	if (size % 2 > 0)
		(rand() % 2 > 0) ? (field_of_node[ceil(size / 2)] = (hex_gamer)second) : (field_of_node[ceil(size / 2)] = (hex_gamer)first);
}

int method_monte::simulation_one_thread(std::vector<hex_gamer> field_game, graph& mind_graph, std::vector<hex_gamer>& field_of_node, std::vector<std::pair<hex_gamer, int>> history, int repeating)
{
	int output = 0;
	clear_statistic();

	// copy existing data

	for (int i = 0; i < repeating; ++i) {

		// filling according to the player system
		fifty_fifty(field_of_node);

		mtx3.lock();
		// generation
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		shuffle(field_of_node.begin(), field_of_node.end(), std::default_random_engine(seed));
		mtx3.unlock();

		copy_data(history, field_of_node);
		//print();

		// convert data for graph method
		mtx2.lock();
		convert(field_game, mind_graph, field_of_node);
		mtx2.unlock();

		// cheaking who won
		hex_gamer  local_player = computer;
		local_player = mind_graph.who_won(local_player, field_of_node, true);
		int spin = (local_player != none) ? (1) : (- 1);
		if (local_player == none)
		{
			local_player = (computer == first) ? (second) : (first);
		}

		// it is determined who the winner is, if he is not, then we change to the opposite player
	
		std::list<int> way = finding_way(mind_graph, local_player);
		mtx.lock();
		for (auto it = way.begin(); it != way.end(); ++it) {
			if (field_game[*it] == none) {
				field_of_statistic[*it] += spin;
				if (field_of_statistic[output] < field_of_statistic[*it])
					output = *it;
			}
		}
		mtx.unlock();
	}
	return output;
}

//	procedure for simulating a game of two players
int method_monte::simulation(controller& control)
{
	hex_gamer local_winner = (hex_gamer)none;
	int best_moves[4] = { 0, 0, 0, 0 };

	graph for_first_th(control.own_graph);
	std::vector<hex_gamer> fir_field(size_field * size_field);

	std::thread first([&]()
		{
			best_moves[0] = simulation_one_thread(control.field_of_play, for_first_th, fir_field, control.history_of_game,  amount_of_repeat / amount_of_thread);
		});

	graph for_second_th(control.own_graph);
	std::vector<hex_gamer> sec_field(size_field * size_field);

	std::thread second([&]()
		{
			best_moves[1] = simulation_one_thread(control.field_of_play, for_second_th, sec_field, control.history_of_game, amount_of_repeat / amount_of_thread);
		});

	graph for_third_th(control.own_graph);
	std::vector<hex_gamer> third_field(size_field * size_field);

	std::thread third([&]()
		{
			best_moves[2] = simulation_one_thread(control.field_of_play, for_third_th, third_field, control.history_of_game, amount_of_repeat / amount_of_thread);
		});

	graph for_fourth_th(control.own_graph);
	std::vector<hex_gamer> fourth_field(size_field * size_field);

	best_moves[3] = simulation_one_thread(control.field_of_play, for_fourth_th, fourth_field, control.history_of_game, amount_of_repeat / amount_of_thread);
	first.join();
	second.join();
	third.join();


	int best_step = 0;
	for (int i = 0; i < amount_of_thread; ++i) {
		if (field_of_statistic[best_step] < field_of_statistic[best_moves[i]])
			best_step = best_moves[i];
	}

	return best_step;
}

void method_monte::convert(std::vector<hex_gamer>& field_game, graph& mind_graph, std::vector<hex_gamer>& field_of_node)
{
	mind_graph.clear_edges();
	for (int i = 0; i < field_of_node.size(); ++i) {
		if (field_game[i] == none) {
			mind_graph.add_links(i, field_of_node);
		}
	}
}


//	The algorithm described below will work correctly if 
//	the method of the graph class was called before it - who_won
std::list<int> method_monte::finding_way(graph& mind_graph, const hex_gamer& local_gamer)
{
	std::vector<int>::iterator it;

	// cleaning vector from repeating elements
	auto upper_array = mind_graph.get_upper_ends();
	mind_graph.clear_upper_ends();
	auto lower_array = mind_graph.get_lower_ends();
	mind_graph.clear_lower_ends();

	std::pair <int, int> ends;

	// add
	int size = round(sqrt(mind_graph.V()));
	int size_trail = INT_MAX;

	for (auto i = upper_array.begin(); i != upper_array.end(); ++i) {
		for (auto j = lower_array.begin(); j != lower_array.end(); ++j) {
			std::list<int> trail = mind_graph.path(*i, *j, links[local_gamer]);
			if ((trail.size() >= size) && (trail.size() < size_trail)) {
				size_trail = trail.size();
				ends.first = *i;
				ends.second = *j;
			}
		}
	}

	return mind_graph.path(ends.first, ends.second, links[computer]);
}

// algorithm for finding the element with the highest value 
// (the number of paths in which the given hex is included)
int method_monte::best_move()
{
	int hex;
	int temp = 0;

	for (int i = 0; i < field_of_statistic.size(); ++i) {
		if (field_of_statistic[i] > temp) {
			hex = i;
			temp = field_of_statistic[i];
		}

	}
	return hex;
}

// function for testing the work of the class (in the future will be turned into an image of the work of AI)
void method_monte::print(std::vector<hex_gamer>& field_of_node)
{
	int field_size = round(sqrt(field_of_statistic.size()));
	for (int i = 0; i < field_size; ++i) {

		std::string f_line(i * 2, ' ');
		for (int j = 0; j < field_size; ++j) {
			if (field_of_node[i * field_size + j] == first)
				f_line += 'X';
			else if (field_of_node[i * field_size + j] == second)
				f_line += 'Y';
			else
				f_line += '.';
			f_line += " - ";
		}
		f_line.resize(f_line.length() - 3);
		std::cout << f_line << std::endl;
		f_line.clear();

		if (i != field_size - 1) {
			std::string s_line(i * 2 + 1, ' ');

			f_line = "\\ / ";
			f_line = f_line * (field_size - 1);
			f_line = s_line + f_line;
			f_line = f_line + '\\';
			std::cout << f_line << std::endl;
		}
	}
}

//	helper function for the algorithm
void method_monte::clear_statistic()
{
	for (auto it = field_of_statistic.begin(); it != field_of_statistic.end(); ++it)
		*it = 0;
}