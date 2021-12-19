#include <iostream>
#include <set>
#include <list>
#include "main_data_first.h"
// castile and operator overloading
#if !defined(temp_h_main_data)
#define temp_h_main_data

inline std::string operator * (std::string a, unsigned int b)
{
	std::string output = "";
	output.reserve(79);
	while (b--) {
		output += a;
	}
	return output;
}

inline std::ostream& operator<< (std::ostream& out, hex_gamer element)
{
	if (element == first)
		std::cout << "first";
	else if (element == second)
		std::cout << "second";
	return out;
}

inline std::set<int> convert_list_pair_to_set(std::list<std::pair<int, int>> some_list)
{
	std::set<int> output;
	for (auto& it : some_list) {
		output.insert(it.first);
		output.insert(it.second);
	}
	return output;
}
inline int my_mark(hex_gamer local_player, int hex, int size_ostov, int size_field)
{
	return (local_player == first) ? (hex / size_field) : (hex / size_field);
}
#endif