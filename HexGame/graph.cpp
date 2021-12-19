
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>

#include "graph.h"

graph::graph(const int& some_vertices)
{
	vertices = some_vertices;
	std::vector<edge> temp(0);
	array_vertex = temp;
	matrix_graph.reserve(vertices);
	matrix_graph.resize(vertices);
	for (int i = 0; i < vertices; ++i) {
		matrix_graph[i].reserve(vertices);
		matrix_graph[i].resize(vertices);
		for (int j = 0; j < vertices; ++j) {
			matrix_graph[i][j] = 0;
		}
	}
}

graph::graph(const graph& g)
{
	vertices = g.vertices;
	matrix_graph.resize(vertices);
	std::vector<edge> temp(g.array_vertex.size());
	array_vertex = temp;
	for (int i = 0; i < vertices; ++i) {
		matrix_graph[i].resize(vertices);
		for (int j = 0; j < vertices; ++j) {
			matrix_graph[i][j] = g.matrix_graph[i][j];
		}
	}

	for (int i = 0; i < array_vertex.size(); ++i) {
		array_vertex[i] = g.array_vertex[i];
	}
}

graph::graph(const std::string& some_name)
{
	if (some_name != "") {
		std::ifstream data_file(some_name);
		std::istream_iterator<int> start(data_file), end;
		std::vector<int> data(start, end);

		vertices = *data.begin();
		matrix_graph.resize(vertices);
		std::vector<edge> temp(0);
		array_vertex = temp;

		for (int i = 0; i < vertices; ++i) {
			matrix_graph[i].resize(vertices);
		}
		data.erase(data.begin());

		for (int i = 0; i < data.size();) {
			matrix_graph[data[i]][data[i + 1]] = matrix_graph[data[i + 1]][data[i]] = data[i + 2];
			array_vertex.push_back({ data[i], data[i + 1], data[i + 2] });
			i = i + 3;
		}
	}
}

graph& graph::operator=(const graph& g)
{
	this->vertices = g.vertices;

	this->matrix_graph.resize(vertices);
	for (int i = 0; i < vertices; ++i) {
		this->matrix_graph[i].resize(vertices);
		for (int j = 0; j < vertices; ++j)
			this->matrix_graph[i][j] = g.matrix_graph[i][j];
	}

	this->array_vertex.resize(g.array_vertex.size());
	for (int i = 0; i < array_vertex.size(); ++i)
		this->array_vertex[i] = g.array_vertex[i];

	return *this;
}

graph::~graph()
{
	if (vertices > 0) {
		for (int i = 0; i < vertices; ++i)
			this->matrix_graph[i].clear();
		this->matrix_graph.clear();
		this->vertices = 0;
		this->edges = 0;
	}
}

// method for testing class in console 
void graph::for_testing()
{
	for (auto& it : array_vertex) {
		std::cout << "First vertex: " << it.first << " second: " << it.second << " value: " << it.value << std::endl;
	}
}

// Method for setting the desired edge value
void graph::set_edge_value(const int& first, const int& second, const int& value)
{
	bool empty = matrix_graph[first][second] == 0;
	matrix_graph[first][second] = matrix_graph[second][first] = value;
	if ((value != 0) && (empty))
		array_vertex.push_back({ first, second, value });
}

bool graph::adjacent(const int& x, const int& y, const int& value) {
	if (matrix_graph[x][y] == matrix_graph[y][x]) {
		if (matrix_graph[x][y] == value)
			return true;
		else
			return false;
	}
}

// Algorithm for searching in the graph of neighbors with the considered vertex
std::list<int> graph::neighbors(const int& x, const int& value) {

	std::list<int> output;
	for (int i = 0; i < vertices; ++i) {
		if ((matrix_graph[x][i] == value) & (x != i)) {
			output.push_front(i);
		}
	}
	return output;
}


int graph::get_node_value(const int& x) {
	int p = 0;
	for (int i = 0; i < vertices; ++i) {
		if (matrix_graph[x][i] > 0) {
			++p;
		}
	}
	return p;
}

std::list<std::pair<int, int>> graph::prim(const int& a, const int& value)
{
	edge elem_edge;
	std::vector<edge> char_edges(this->array_vertex.size());

	auto it = std::copy_if(this->array_vertex.begin(), this->array_vertex.end(), char_edges.begin(), [&](edge& first) {return first.value == value; });
	char_edges.erase(it, char_edges.end());
	//sort(char_edges.begin(), char_edges.end(), compar);  //(закоментил, потому что все ребра будут одинакового размера  

	std::list<std::pair<int, int>> ostov;
	std::vector<bool> added_ver(vertices, true);

	added_ver[a] = false;
	int x = 0, y = 0;
	ostov_cost = 0;
	bool main_flag = true, add_flag;

	while (main_flag) {
		add_flag = true;
		for (int i = 0; (i < char_edges.size()) && (add_flag); ++i) {
			elem_edge = char_edges[i];
			x = elem_edge.first;
			y = elem_edge.second;
			if ((added_ver[x] ^ added_ver[y])) {
				ostov.push_back({ elem_edge.first, elem_edge.second });
				ostov_cost += elem_edge.value;
				added_ver[y] = added_ver[x] = false;
				char_edges.erase(char_edges.begin() + i);
				add_flag = false;
			}
		}
		if (add_flag)
			main_flag = false;
	}
	return ostov;
}

// Algorithm Kruscal( search ostov tree)
std::vector<int> graph::kruscal(const int& value)
{
	edge elem_edge;
	std::vector<edge> char_edges(this->array_vertex.size());

	auto it = std::copy_if(this->array_vertex.begin(), this->array_vertex.end(), char_edges.begin(), [&](edge& first) {return first.value == value; });
	char_edges.erase(it, char_edges.end());
	this->tree_vertices.resize(this->vertices);
	sort(char_edges.begin(), char_edges.end(), compar); //(закоментил, потому что все ребра будут одинакового размера  

	std::vector<int> output;

	// creating initial sets
	for (int i = 0; i < vertices; ++i)
		make_set(i);

	for (int i = 0; i < char_edges.size(); ++i) { /// если косяк, то по-любому тут
		elem_edge = char_edges[i];
		if (find(elem_edge.first) != find(elem_edge.second)) {
			output.push_back(elem_edge.first);
			output.push_back(elem_edge.second);
			unite(elem_edge.first, elem_edge.second);
			ostov_cost += elem_edge.value;
		}

	}

	this->tree_vertices.clear();
	return output;
}

/*std::list<std::pair<int, int>> graph::boruvka(const int& value)
{
	while
}*/

// depth-first search algorithm
bool graph::is_conected(const int& u, const int& v, const int& value)
{
	std::vector<bool> connect(vertices, false);
	std::vector<int> stack(vertices);
	std::list<int> nei;
	int f_ver, s_ver;
	stack.push_back(u);
	connect[u] = true;

	while (stack.size() > 0) {

		f_ver = stack[stack.size() - 1];
		stack.pop_back();
		nei = neighbors(f_ver, value);

		while (!nei.empty()) {
			s_ver = nei.front();
			nei.pop_front();
			if (!connect[s_ver]) {
				stack.push_back(s_ver);
				connect[s_ver] = true;
			}
		}
	}
	return connect[v];
}


// Dijkstra's algorithm
std::list<int> graph::path(const int& u, const int& v, const int& value)
{
	priority_queue all_ver, remain_ver;
	for (int i = 0; i < V(); ++i) {
		if (i != u) {
			remain_ver.insert(i, INT_MAX / 2, true);
			all_ver.insert(i, INT_MAX / 2, true);
		}
	}
	remain_ver.insert(u, 0, true);
	all_ver.insert(u, 0, true);
	std::list<int> nei;
	int f_ver;

	// traversal of small vertices to find the minimum distance to any vertex from u
	while (remain_ver.size() > 0) {
		f_ver = remain_ver.top();
		nei = neighbors(f_ver, value);
		while (!nei.empty()) {
			int s_ver = nei.front();
			if (remain_ver.contains(s_ver)) {
				int temp = remain_ver.get_priority(f_ver) + get_edge_value(f_ver, s_ver);
				if (remain_ver.get_priority(s_ver) > temp) {
					remain_ver.chgPriority(s_ver, temp);
					all_ver.chgPriority(s_ver, temp);
				}
			}
			nei.pop_front();
		}
		remain_ver.minPriority();
	}
	path_cost = all_ver.get_priority(v);
	int f_cost = path_cost;
	std::list<int> way;
	f_ver = v;

	// backtrack to u, finding the shortest path
	while (f_cost > 0) {
		nei = neighbors(f_ver, value);
		bool flag = true;
		while (!nei.empty() && flag) {
			int s_ver = nei.front();
			int temp = all_ver.get_priority(s_ver) + get_edge_value(s_ver, f_ver);
			if ((temp == all_ver.get_priority(f_ver)) && all_ver.contains(s_ver)) {
				way.push_front(s_ver);
				f_cost = f_cost - get_edge_value(s_ver, f_ver);
				flag = false;
				all_ver.chgPriority(f_ver, -1);
				all_ver.minPriority();
				f_ver = s_ver;
			}
			nei.pop_front();
		}
	}
	way.push_back(v);
	return way;
}
hex_gamer graph::who_won(const hex_gamer& player, std::vector<hex_gamer>& field, bool work_brain)
{
	int size = round(sqrt(field.size()));
	std::vector<int> ends;
	std::pair<int, int> element;

	for (int i = 0; i < size; ++i) {
		int j;

		(player == first) ? (j = i) : (j = i * size); // преход к свободнуму выбору игроков...............................
		if (field[j] == player)
			ends.push_back(j);
	}

	std::list<std::pair<int, int>> way;
	std::set<int> clean_point;
	for (auto it = ends.begin(); it != ends.end(); ++it) {

		// можно добавить поиск в листе по элементу 
		std::list<std::pair<int, int>> trail = prim(*it, links[player]);
		if (way.size() < trail.size()) {

			if (clean_point.size() > 0)
				clean_point.clear();

			for (auto it_second = trail.begin(); it_second != trail.end(); ++it_second) {
				element = *it_second;
				clean_point.insert(element.first);
				clean_point.insert(element.second);
			}
		}
	}

	const int
		upper_border = 0,
		lower_border = size - 1;

	// logical array for collect result
	std::vector<std::vector <bool>> border(3, std::vector<bool>(2, false));

	// check in the skeleton of hexes at the "borders"
	for (auto it = clean_point.begin(); it != clean_point.end(); ++it) {

		// navigation in matrix of field_of_play 
		int y = *it % size;
		int x = *it / size;

		if (player == first) {
			if (x == upper_border) {
				border[first][1] = true;                                        // possibly since hex_gamer is a type of constants
				this->upper_hex.push_back(*it);
			}
			else if (x == lower_border) {
				border[first][0] = true;
				this->lower_hex.push_back(*it);
			}
		}
		else if (player == second) {
			if (y == upper_border) {
				border[second][1] = true;
				this->upper_hex.push_back(*it);
			}
			else if (y == lower_border) {
				border[second][0] = true;
				this->lower_hex.push_back(*it);
			}
		}
	}

	hex_gamer winner = none;

	if (border[first][0] && border[first][1])
		winner = first;

	if (border[second][0] && border[second][1])
		winner = second;

	if ((winner == none) || (work_brain == false)) {
		upper_hex.clear();
		lower_hex.clear();
	}

	return winner;
}

// special algorithm for data structure DSU
int graph::find(int& v)
{
	return (tree_vertices[v] == v) ? v : (tree_vertices[v] = find(tree_vertices[v]));
}

// special algorithm for data structure DSU
void graph::unite(int& v, int& u)
{
	v = tree_vertices[v];
	u = tree_vertices[u];
	if (rand() & 1)
		std::swap(v, u);
	if (v != u)
		tree_vertices[v] = u;
}

// special algorithm for data structure DSU
inline void graph::make_set(int& v)
{
	tree_vertices[v] = v;
};

std::vector<int> graph::search_border_hex_up(const  hex_gamer& local_player, std::vector<hex_gamer>& field)
{
	const int size = round(sqrt(V()));
	std::vector<int> ends(0);

	for (int i = 0; i < size; ++i) {
		int up_index = 0, down_index = 0;
		(local_player == first) ? (up_index = i) : (up_index = i * size);
		if (field[up_index] == local_player)
			ends.push_back(up_index);
	}

	int temp = 0;
	for (int i = 1; i < ends.size(); ++i)
	{
		if (get_edge_value(ends[temp], ends[i]))
		{
			ends.erase(ends.begin() + temp);
		}
		temp = i;
	}
	return ends;
}

std::vector<int> graph::search_border_hex_down(const hex_gamer& local_player, std::vector<hex_gamer>& field)
{
	const int size = static_cast<int>(round(sqrt(V())));
	std::vector<int> ends(0);

	for (int i = 0; i < size; ++i) {
		int up_index = 0, down_index = 0;
		(local_player == first) ? (down_index = size * (size - 1) + i) : (down_index = size * (i + 1) - 1);
		if (field[up_index] == local_player)
			ends.push_back(up_index);
	}

	int temp = 0;
	for (int i = 1; i < ends.size(); ++i)
	{
		if (get_edge_value(ends[temp], ends[i]))
		{
			ends.erase(ends.begin() + temp);
		}
		temp = i;
	}
	return ends;
}

// special algorithm for adding links in the graph (use only when working with hex game)
// you should add information in field and them call this method 
void graph::add_links( const int& hex, const std::vector<hex_gamer>& field)
{
	std::pair<int, int> element;
	hex_gamer some_player = field[hex];
	int size = round(sqrt(vertices));

	int y = hex % size;
	int x = hex / size;
	int link = links[some_player];
	int null_link = links[none];
	const int size_border = 6;

	for (int i = 0; i < size_border; ++i) {
		element = border[i];
		if (position_check(x, y, size, element)) {
			int add_hex = (x + element.first) * size + (y + element.second);

			(field[add_hex] == some_player) ? set_edge_value(hex, add_hex, link) : set_edge_value(hex, add_hex, null_link);
		}
	}
}

void graph::clear_edges()
{
	array_vertex.clear();
}

std::pair<int, int> graph::get_last_edge(const hex_gamer& local_player) 
{
	edge output;
	int local_value = static_cast<int>(local_player);
	for (auto& it : array_vertex) {
		if (it.value == local_value) {
			output = it;
			break;
		}
	}
	return { output.first, output.second };
}