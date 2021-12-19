#include "search_tree.h"

search_tree::search_tree(hex_gamer who_is_computer, int size)
{
	copy_of_field.resize(size * size);
	statistice.resize(size * size);
	graph temp_graph(size * size); // посмотри сколько должно будет(должно быть 121 100 81 ...
	computer = who_is_computer;

	// create ends of winner path for first and second player 
	mind_graph = temp_graph;
	this->size = size;
}

int search_tree::search_move_u_minmax(controller& controller)
{
	int best_move = 0;
	update_data(controller);

	for (int i = 0; i < statistice.size(); ++i) {
		minmax(i, max_depth, computer);
		if (statistice[i] > statistice[best_move])
			best_move = i;
	}
	return best_move;
}

int search_tree::search_move_u_aplha_beta(controller& controller)
{
	int best_move = 0;
	update_data(controller);

	for (int i = 0; i < statistice.size(); ++i) {

		alpha_beta_algorithm(i, max_depth, INT_MIN, INT_MAX, computer);
		if (statistice[i] > statistice[best_move])
			best_move = i;
	}
	return best_move;
}

int search_tree::minmax(int node, int depth, hex_gamer local_gamer)
{
	if (depth == 0)
		return heuristic_function(local_gamer);

	hex_gamer next_player = (local_gamer == first) ? (second) : (first);
	if (local_gamer == computer) { // in future posible is mistake
		int value = INT_MIN;
		auto child = get_possible_move(node, local_gamer);
		for (auto& it : child) {
			make_move(it, local_gamer);
			value = std::max(value, minmax(it, depth - 1, next_player));
		}
		return value;
	}
	else {
		int value = INT_MAX;
		auto child = get_possible_move(node, local_gamer);
		for (auto& it : child) {
			make_move(it, local_gamer);
			value = std::min(value, minmax(it, depth - 1, next_player));
		}
		return value;
	}
}

int search_tree::alpha_beta_algorithm(int node, int depth, int alpha, int beta, hex_gamer local_gamer)
{
	if (depth == 0)
		return heuristic_function(local_gamer);

	copy_of_field[node] = local_gamer;

	const hex_gamer next_player = (local_gamer == first) ? (second) : (first);
	if (local_gamer == computer) { // in future posible is mistake
		int value = INT_MIN;
		auto child = get_possible_move(node, local_gamer);
		for (auto& it : child) {
			this->copy_of_field[it] = local_gamer;
			value = std::max(value, alpha_beta_algorithm(it, depth - 1, alpha, beta, next_player));
			if (value >= beta)
				break;
			alpha = std::max(alpha, value);
		}
		return value;
	}
	else {
		int value = INT_MAX;
		auto child = get_possible_move(node, local_gamer);
		for (auto& it : child) {
			this->copy_of_field[it] = local_gamer;
			value = std::min(value, minmax(it, depth - 1, next_player));
			if (value <= alpha)
				break;
			beta = std::min(beta, value);
		}
		return value;
	}
}

void search_tree::update_data(controller& controller)
{
	for (int i = 0; i < copy_of_field.size(); ++i) {
		copy_of_field[i] = controller.field_of_play[i];
	}
	mind_graph = controller.own_graph;
}

inline void search_tree::clear_statistice()
{
	// clear array 
	for (auto& it : statistice) {
		it = 0;
	}
}


std::set<int>& search_tree::get_possible_move(int hex, hex_gamer local_player)
{
	static std::set<int> output;
	output.clear();
	//piece_of_gold inform = find_a_piece_of_gold(local_player);
	start_position local_start = analyze_start_position(local_player);

	do
	{
		int temp_move = possible_move(hex, local_player, local_start, output);
		if (temp_move == INT_MIN)
		{
			/*std::cout << "Problem!" << std::endl;
			system("pause");*/
			break;
		}
		output.insert(temp_move);
		/*int warning_position = 0;
		if (check_neigbors(hex, temp_move, local_player, warning_position))
		{
			output.insert(warning_position);
		}*/
		
	} while (output.size() < amount_of_child);

	return output;
}

// method can return error(INT_MIN), if algorithm didn't find possible move.
int search_tree::possible_move(int hex, hex_gamer local_player, start_position local_start, std::set<int>& all_moves)
{
	// coordinate hex( it made for this (is to get rid of a lot of if)
	//const std::vector<std::pair<int, int>> relative_position = { {1, -2}, {2, -1}, {1, 1}, {-1, 2}, {-2, 1}, {-1, 1} };
	const std::vector<std::pair<int, int>> relative_position = { {0, -1}, { 1, -1 }, { 1, 0 }, { 0, 1 }, { -1, 1 }, { -1, 0 } };
	//{ {0, -1}, { 1, -1 }, { 1, 0 }, { 0, 1 }, { -1, 1 }, { -1, 0 } }
	// an attempt to implement an automaton
	const bool avtomat[3][4] = {
		{false, false, false, false},
		{false, false, true, false },
		{false, false, true, false }
	};

	int const x = hex % this->size, y = hex / this->size;

	int required_level_max = 0, output = 0, required_level_min = 0;
	bool visit_first = false, visit_second = false;
	for (auto& it : relative_position)
	{
		int temp_hex = (y + it.second) * size + x + it.first;
		if (position_check(x, y, this->size, it) && (copy_of_field[temp_hex] == none) && (all_moves.find(temp_hex) == all_moves.end()))
		{
			int x_temp = x + it.first;
			int y_temp = y + it.second;

			int temp_level = (local_player == first) ? (it.second) : (it.first);

			if (avtomat[local_player][local_start] && (required_level_min > temp_level))
			{
				required_level_min = temp_level;
				output = y_temp * this->size + x_temp;
				return output;
			}
			if (!avtomat[local_player][local_start] && (required_level_max < temp_level))
			{
				required_level_max = temp_level;
				output = y_temp * this->size + x_temp;
				return output;
			}
		}
	}
	return INT_MIN; // RETURN ERROR
}


inline int search_tree::heuristic_function(hex_gamer local_player)
{
	int output = 0;
	print();
	//position_border local_position = (local_player == first) ? (horisont) : (vertical);
	std::vector<int> ends;
	start_position local_position = analyze_start_position(local_player, true);
	if (local_position == middle)
	{
		auto it_first = find_a_piece_of_gold(local_player, down);
		auto it_second = find_a_piece_of_gold(local_player, up);
		int first_mark = my_mark(local_player, it_first.hex, it_first.size_ostov, this->size);
		int second_mark = my_mark(local_player, it_second.hex, it_second.size_ostov, this->size);
		output = second_mark - (this->size - first_mark);
	}
	else
	{
		auto it = find_a_piece_of_gold(local_player, local_position);
		output = my_mark(local_player, it.hex, it.size_ostov, this->size);
	}
	// нужна функция от двух переменных для оценки игры
	return output;
}

start_position search_tree::analyze_start_position(const hex_gamer& local_player, bool store_data) 
{
	// initialize (find position, meta-data about hex(position))
	start_position local_start = nowhere;
	this->meta_data = mind_graph.search_border_hex_up(local_player, copy_of_field);
	if (this->meta_data.empty())
	{
		this->meta_data = mind_graph.search_border_hex_down(local_player, copy_of_field);
		if (!this->meta_data.empty())
		{
			local_start = down;
		}
		else
		{
			this->meta_data = for_middle_var(local_player, local_start);
		}
	}
	else
	{
		local_start = up;
	}
	if (!store_data)
	{
		this->meta_data.clear();
	}
	return local_start;
}

inline std::vector<int>& search_tree::analyze_start_position(const start_position& local_position)
{
	// initialize (find position, meta-data about hex(position))
	std::vector<int> output(0);
	if (local_position == middle)
	{
		output.push_back(this->meta_data.front());
	}
	else
	{
		return this->meta_data;
	}
	this->meta_data.clear();
	return output;
}

// first - hex, second - start position
piece_of_gold search_tree::find_a_piece_of_gold(const hex_gamer& local_player, const start_position& local_start) {

	// initialize (find position, meta-data about hex(position))
	piece_of_gold output = { 0, 0 , nowhere };
	std::vector<int> ends = analyze_start_position(local_start);
	int level_step = 0;
	constexpr bool avtomat[3][3] = {
		{false, false, false },
		{false, true, false },
		{false, true, false },
	};

	for (auto it = ends.begin(); it != ends.end(); ++it) {
		auto trail = mind_graph.prim(*it, links[local_player]);       ////// переделай 
		std::set<int> temp_set;
		temp_set = convert_list_pair_to_set(trail);
		bool visit = false;
		//trail.assign(temp_set.begin(), temp_set.end());
		for (auto second_it : temp_set)
		{
			int const temp_deep = (local_player == first) ? (second_it/ this->size) : (second_it % size);
			
			if ((avtomat[local_player][local_start]) && (level_step < temp_deep))
			{

				output.hex = second_it;
				level_step = temp_deep;
				visit = true;
			}
			else if (!avtomat[local_player][local_start] && (level_step > temp_deep)) 
			{
				output.hex = second_it;
				level_step = temp_deep;
				visit = true;
			}
		}
		if ((temp_set.size() < output.size_ostov) && visit)
		{
			output.size_ostov = temp_set.size();
		}
	}
	return output;
}
inline std::vector<int> search_tree::for_middle_var(const hex_gamer& local_player, start_position& local_start)
{
	//auto last_hex = mind_graph.get_last_edge(local_player); // return two vertices, but i will use only one 
	auto octov_tree = mind_graph.kruscal(local_player);
	if (!octov_tree.empty())
	{
		local_start = middle;
	}
	return octov_tree;
}

bool search_tree::check_neigbors(const int& first, const int& second, hex_gamer& local_player, int& warning_position)
{
	auto temp_list = neighbors_hex(first, second);
	std::vector<int> temp_result(0);
	bool output = false;
	for (auto it = temp_list.begin(); it != temp_list.end(); ++it)
	{
		output = (this->copy_of_field[*it] == local_player);
		if (output)
		{
			temp_result.push_back(*it);
		}
	}

	if (temp_result.size() == 1)
	{
		warning_position = temp_result.front();
	}
	return !temp_result.empty();
}

void search_tree::print()
{
	int field_size = round(sqrt(copy_of_field.size()));
	for (int i = 0; i < field_size; ++i) {

		std::string f_line(i * 2, ' ');
		for (int j = 0; j < field_size; ++j) {
			if (copy_of_field[i * field_size + j] == first)
				f_line += 'X';
			else if (copy_of_field[i * field_size + j] == second)
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

int search_tree::generate_start_move(const hex_gamer& local_player, start_position& local_start)
{
	int output = 0;
	int i = rand() % this->size;
	if (local_player == first)
	{
		if (rand() % 2)
		{
			output = i;
			local_start = up;
		}
		else
		{
			output = this->size * (this->size - 1) + i;
			local_start = down;
		}
		//output = (rand() % 2 == 1) ? (i) : (this->size * (this->size - 1) + i);
	}
	else if (local_player == second)
	{
		if (rand() % 2)
		{
			output = i * this->size;
			local_start = up;
		}
		else
		{
			output = this->size * (i + 1) - 1;
			local_start = down;
		}
		//output = (rand() % 2 == 1) ? (i * this->size) : (this->size * (i + 1) - 1);
	}
	return output;
}

std::vector<int>& search_tree::neighbors_hex(const int& first, const int& second)
{
	int const x_first = first % this->size;
	int const y_first = first / this->size;
	int const x_second = second % this->size;
	int const y_second = second / this->size;
	std::vector<int> first_result(0);
	std::vector<int> second_result(0);

	for (int i = 0; i < size_border; ++i) {
		auto element = border[i];
		if (position_check(x_first, y_first, size, element))
		{
			int add_hex = (x_first + element.first) * size + (y_first + element.second);
			first_result.push_back(add_hex);
		}
		if (position_check(x_second, y_second, this->size, element))
		{
			int add_hex = (x_second + element.first) * size + (y_second + element.second);
			second_result.push_back(add_hex);
		}
	}
	static std::vector<int> output(first_result.size() + second_result.size());

	std::sort(first_result.begin(), first_result.end());
	std::sort(second_result.begin(), second_result.end());
	auto last_position = std::set_intersection(first_result.begin(), first_result.end(), second_result.begin(), second_result.end(), output.begin());
	output.resize(last_position - output.begin());
	return output;
}

void search_tree::make_move(int hex, hex_gamer local_player)
{
	this->copy_of_field[hex] = local_player;
	this->mind_graph.add_links(hex, this->copy_of_field);
}


search_tree::~search_tree()
{
	copy_of_field.clear();
	statistice.clear();
	computer = none;
	best_move = 0;
	size = 0;
}
