#include <iostream>

#include "method_monte.h"
#include "search_tree.h"

using namespace std;
	
int main()
{
	int size; 
	cout << "Enter field sizes" << endl;
	cout << "Size = ";
	cin >> size;
	hex_gamer first_gamer = first;
	controller Consol1(size, first_gamer);
	cout << "Do you want play with computer ?" << endl;
	cout << "It's joke" << endl;
	system("pause");                // to rate my joke
	Consol1.clear();

	// While the choice of the player is impossible (I'm too lazy to deal with this trifle)
	//method_monte computer((hex_gamer)second, size);
	search_tree computer((hex_gamer)second, size);

	bool work_mind = true;
	hex_gamer computer_player = (hex_gamer)second;

	// main cycle of game 
	do {
		int move = 0;

		// cycle for checking the stroke, the cycle can only be exited after the correct stroke
		do {

			if (work_mind && (Consol1.player_now() == computer_player)) {
				//Consol1.clear();  // убрать
				//move = computer.simulation(Consol1);
				move = computer.search_move_u_minmax(Consol1);
			}
			else {
				Consol1.print();
				cout << Consol1.player_now() << " player's turn: ";
				cin >> move;
				--move;
			}
			//Consol1.clear();

		// checking the player's move
		} while (!Consol1.check_step(move));

	// checking the end of the game
	} while (!Consol1.continue_game());
	Consol1.print();
	Consol1.print_winner();
	return 0;
}