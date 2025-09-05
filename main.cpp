#include "Game.h"
#include "OutputFormatter.h"
#include <iostream>
#include <ctime>

using std::cout;
using std::cin;
using std::srand;

int main()
{

//    cout << "<!------------------------------------------------------------------!>\n";
//    cout << "                         Welcome to the Game of Farkle!\n";
//    cout << "<!------------------------------------------------------------------!>\n\n";

    cout << OutputFormatter("Welcome to Farkle!").GetResult() << "\n";
    
    srand(time(NULL));

    Game game;

    game.SimulateGame();
}
