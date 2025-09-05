#include "Round_Imp.h"
#include "OutputFormatter.h"

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

        //Get Player count
        cout << "How many players? ";
        cin >> numPlayers;

        //Needed when you have a getline statement that
        //immediately follows a cin statement.
        cin.ignore();

        //Get the name of each player
        for(int i = 0; i < numPlayers; i++)
        {
            Player p;
            cout << "Player " << i + 1 << "'s name: ";
            cin >> p.name;

            p.score = 0;
            players.push_back(p);
        }
    }

    void SimulateGame()
    {
        do
        {
            for(int i = 0; i < players.size() && CheckForWinner() == false; i++)
            {
                cout << "\n\n";
                stringstream ss;
                ss << "Score: ";
                ss << players[i].score;

                cout << "\n\n";
                vector<string> output = {players[i].name + "'s Round", ss.str()};
                cout << OutputFormatter(output).GetResult() << "\n\n";
                activeRound = Round(&players[i]);
                activeRound.RunRound();
            }

        } while (CheckForWinner() == false);
        
        //Print Winning player.

        for(int i = 0; i < players.size(); i++)
        {
            if(players[i].score >= 10000)
            {
               vector<string> output;
               output.push_back("Winner: " + players[i].name);
               stringstream ss;
               ss << "Final Score: ";
               ss << players[i].score;
               output.push_back(ss.str());
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