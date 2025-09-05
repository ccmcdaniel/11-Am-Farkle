#include "Game.h"
#include <iostream>
#include <ctime>

using std::cout;
using std::cin;
using std::srand;

int main()
{
    srand(time(NULL));

    Game game;

    game.SimulateGame();

}
