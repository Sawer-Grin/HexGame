#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

#include "controller.h"

controller::controller(const int& size, const hex_gamer& first_player)
{
	field_size = size;
	field_of_play.resize(field_size * field_size, (hex_gamer)none);

	player = static_cast<hex_gamer>(first_player);
	graph some(field_size * field_size);
	own_graph = some;
	first_border = horisont;
	system("cls");
}

controller::controller(const controller& cons)
{
	this->field_size = cons.field_size;

	for (int i = 0; i < field_size * field_size; ++i)
		this->field_of_play[i] = cons.field_of_play[i];

	this->history_of_game.resize(cons.history_of_game.size());
	for (int i = 0; i < cons.history_of_game.size(); ++i)
		this->history_of_game[i] = cons.history_of_game[i];
	graph temp(cons.own_graph);
	this->own_graph = temp;
}

controller& controller::operator=(const controller& cons)
{
	this->field_size = cons.field_size;

	this->field_of_play.resize(cons.field_of_play.size());
	for (int i = 0; i < cons.field_of_play.size(); ++i)
		this->field_of_play[i] = cons.field_of_play[i];

	this->history_of_game.resize(cons.history_of_game.size());
	for (int i = 0; i < cons.history_of_game.size(); ++i)
		this->history_of_game[i] = cons.history_of_game[i];

	graph temp(cons.own_graph);
	this->own_graph = temp;
	return *this;
}

controller::~controller()
{
	if (field_size > 0) {
		field_of_play.clear();
		field_size = 0;
		history_of_game.clear();
	}
}

// check if such a move is possible
bool controller::check_step(const int& tile)
{
	bool output = false;
	// adjustment for the perception of the correct between the programmer and the player
	int y = tile % field_size;
	int x = tile / field_size;

	// player step check
	if (position_check(x, y, field_size) && !field_of_play[x * field_size + y]) {
		std::pair<int, int> element;
		field_of_play[tile] = player;
		history_of_game.push_back({ player, tile });
		own_graph.add_links(tile, field_of_play);
		output = true;
	}
	return output;
}

// end game check
bool controller::continue_game()
{
	const hex_gamer local_gamer = own_graph.who_won(player, field_of_play);
	bool game_is_over = (local_gamer == player);

	if (game_is_over)
		winner = player;
	change_player();
	return game_is_over;
}

// change turn queue
void controller::change_player()
{
	(player == first) ? player = (hex_gamer)second : player = (hex_gamer)first;
}