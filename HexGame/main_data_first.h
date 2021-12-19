#include <iostream>

#if !defined(temp_h_main_data_first)
#define temp_h_main_data_first
	
typedef enum { none = 0, first = 1, second = 2 } hex_gamer;
typedef enum { none_method = 0, monte_carlo_method = 1, minmax_method = 2, alpha_beta_method = 3 } method_of_AI;
typedef enum { none_border = 0, horisont = 1, vertical = 2 } position_border;
static int links[3] = { 0, 1, 2 };
static std::pair<int, int> border[6] = { {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {-1, 1}, {0, 1} };
static int size_border = 6;
static int const amount_of_repeat = 2000;
static int const amount_of_thread = 4;
static int const max_depth = 4;
static int const amount_of_child = 2;

// for checking the correctness of the step entered by the player
inline bool position_check(const int& x, const int& y, const int& size, const std::pair<int, int> element = { 0, 0 })				// suckiest feature, but that shit needed to be taken away
{
	return (x + element.first >= 0) && (x + element.first < size) && (y + element.second < size) && (y + element.second >= 0);
}

// drowing field ( not phisical size)
static int size_picture_x = 32, size_picture_y = 24;


// setting of game choosing in button for generate
static struct setting_of_game
{
	int size_of_bord;
	hex_gamer computer;
	hex_gamer first_player;
	method_of_AI method;
};

#endif