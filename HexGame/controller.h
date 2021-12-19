#ifndef _controller_
#define _controller_

#include <iostream>
#include <vector>

#include "graph.h"
#include "main_data.h"
#include "main_data_first.h"
#include "main_data.h"

// class for controlling the game process

class controller
{
public:
	controller() : player(first), field_size(0) {}
	controller(const int& size, const hex_gamer& first_player );										// class constructor
	controller(const controller& cons);																	// copy constructor
	controller& operator = (const controller& cons);
	~controller();																						// class destructor
	bool check_step(const int& titl);																	// method for checking the correct move
	bool continue_game();																				// method for checking at the end of the game
	hex_gamer get_winner() { return winner; };															// method for displaying the winner
	hex_gamer player_now() { return player; }															// method for the player's output		

protected:
	void change_player();

	hex_gamer player;																					// player now
	hex_gamer winner;																					// variable storing the winner at the moment
	graph own_graph;
	std::vector <hex_gamer> field_of_play;																// array of field tiles не использовать для отрисовки, останется до переделки на gui
	int field_size;
	std::vector<std::pair<hex_gamer, int>> history_of_game;												// first - player, second - your step 
	position_border first_border;


	friend class method_monte;
	friend class search_tree;
};

#endif