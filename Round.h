#include "Die.h"
#include "Player.h"
#include <vector>


using std::vector;

class Round
{
private:
    int roundScore;
    Player activePlayer;
    Die dice[6];

public:
    int GetRoundScore() { return roundScore; }
    void ResetDice()
    {
        for(int i = 0; i < 6; i++)
            dice[i].UnlockDie();
    }

    void RollDice()
    {
        for(int i = 0; i < 6; i++)
            dice[i].RollDice();
    }

private:
    vector<vector<Die>> GetScoringCombos()
    {
        vector<vector<Die>> scoring_combos;

        // First, find all the 1's and 5's
        for(int i = 0; i < 6; i++)
        {
            if(dice[i].GetDieFace() == 1 || dice[i].GetDieFace() == 5)
            {
                vector<Die> combo;
                combo.push_back(dice[i]);
                scoring_combos.push_back(combo);
            }
        }
    }

};