#include "Round_Imp.h"

class Game
{
private:
    Round activeRound;
    vector<Player> players;
    int numPlayers = 2;

public:
    Game()
    {
        StartNewGame();
    }

    void StartNewGame()
    {
        //Reset Players
        players = vector<Player>();

        //1. Setup Players
        Player p1, p2;
        p1 = {"Player 1", 0};
        p2 = {"Player 2", 0};

        players.push_back(p1);
        players.push_back(p2);
    }

    void SimulateGame()
    {
        do
        {
            for(int i = 0; i < players.size() && CheckForWinner() == false; i++)
            {
                cout << "******************************************************\n";
                cout << "            ";
                cout << players[i].name << "'s Round    Current Score: ";
                cout << players[i].score << "\n";
                cout << "******************************************************\n";

                activeRound = Round(&players[i]);
                activeRound.RunRound();
            }

        } while (CheckForWinner() == false);
        
        //Print Winning player.

        for(int i = 0; i < players.size(); i++)
        {
            if(players[i].score >= 10000)
            {
                cout << " Winner: " << players[i].name << "\n";
                cout << " Final Score: " << players[i].score << "\n";
            }
        }

    }


private:
//Supporting/Utility Methods
    bool CheckForWinner()
    {
        for(int i = 0; i < players.size(); i++)
        {
            if(players[i].score >= 10000)
                return true;
        }

        return false;
    }

};