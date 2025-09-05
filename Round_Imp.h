#include "Die.h"
#include "Player.h"
#include "OutputFormatter.h"

#include <vector>
#include <algorithm>
#include <iostream>

using std::cout;
using std::vector;
using std::sort;
using std::cin;

struct ScoringCombo
{
    vector<Die> dice;
    int value;
    string combo_name = "Unnamed";

    string ToString()
    {
        stringstream ss;
        ss << combo_name << "(";
        for(int i = 0; i < dice.size(); i++)   
        {
            ss << dice[i].GetDieFace();
            if(i + 1 != dice.size())
                ss << ", ";
        }
        ss << ") Value: " << value;

        return ss.str();
    }
};

class Round
{
private:
    int roundScore;
    Player *activePlayer;
    vector<Die> dice;

public:
    Round(Player *player = nullptr)
    {
        activePlayer = player;
        roundScore = 0;
        ResetDice();
        
    }

    int GetRoundScore() { return roundScore; }


    void ResetDice()
    {
        //Reset the dice count.
        dice = vector<Die>();

        for(int i = 0; i < 6; i++)
            dice.push_back(Die());
    }

    void RunRound()
    {
        bool farkle = false; 
        bool player_end = false;

        do
        {
            cout << "Current Round Score: ";
            cout << roundScore;
            cout << "\n";
            //1. Roll the player dice.
            vector<ScoringCombo> result = RollDice();
            
            //2. Output the resulting dice.
            cout << "\n --> Roll Result: ";
            for(int i = 0; i < dice.size(); i++)
                cout << dice[i].GetDieFace() << ", ";
            cout << "\n";

            //3. Check for Farkle. If farkled.  The round is over.
            if(result.size() == 0)
            {   
                vector<string> output = {activePlayer->name + " Farkled!", 
                    "You lose all points this round..."};
                cout << OutputFormatter(output).GetResult() << "\n";
                roundScore = 0;
                farkle = true;
            }
            //4. If not farkled. Show scoring combinations.
            else
            {
                bool stop = false;
                do
                {
                    cout << "Scoring Combinations: \n";
                    for(int i = 0; i < result.size(); i++)
                    {
                        cout << " --> " << i+1 << ". " << result[i].ToString() << "\n"; 
                    }
                    cout << "\n";
                    int choice;

                    cout << "Enter number of combination to keep (enter -1 to stop): ";
                    cin >> choice;

                    if(choice > 0)
                    {
                        CommitScoringCombo(result, choice);
                        result = GetScoringCombos();
                    }

                    
                    if (result.size() == 0 || choice == -1)
                        stop = true;

                //5. Choose Dice to keep.  Remove dice from possible rolls as player chooses.
                }while(!stop);

                //If the user scored on all dice, reset dice.
                if(dice.size() == 0)
                    ResetDice();

                int choice_roll;
                //6. Ask the player if they want to roll again.
                cout << "You have " << dice.size() << " dice left.  ";
                cout << "Roll Again? (1=Yes, 2=No): ";
                cin >> choice_roll;
                
                //Is the user chooses to stop rolling, then raise that flag.
                if(choice_roll != 1)
                {
                    player_end = true;
                    cout << "\n\n";
                    vector<string> output {"Round Result"};
                    stringstream ss;
                    ss << activePlayer->name << " scored ";
                    ss << roundScore;
                    ss << " points this round!";
                    output.push_back(ss.str());
                    cout << OutputFormatter(output).GetResult();
                    activePlayer->score += roundScore;
                }

                if(dice.size() == 0)
                    ResetDice();
            }

            
        }while(!farkle && !player_end);
    }

    vector<ScoringCombo> RollDice()
    {
        for(int i = 0; i < 6; i++)
            dice[i].RollDice();

        return GetScoringCombos();
    }

private:
    vector<ScoringCombo> GetScoringCombos()
    {
        vector<ScoringCombo> scoring_combos;
        sort(dice.begin(), dice.end());

        // First, find all the 1's and 5's
        FindOnesFives(scoring_combos);

        // Next, find all the sets of 3.
        FindSetsOfThree(scoring_combos);

        // After that, look for 4 of a kind
        Find4OfAKind(scoring_combos);

        // After that, look for 5 of a kind
        Find5OfAKind(scoring_combos);

        //After that, look for 6 of a kind
        Find6OfAKind(scoring_combos);

        //After that, look for 1 - 6 straight
        FindStraight1to6(scoring_combos);

        //After that, look for 3 pairs
        Find3Pairs(scoring_combos);

        //After that, look for 4 of a kind and a pair
        Find4OfAKindAndPair(scoring_combos);

        //Finally, look for 2 triplets
        Find2Triplets(scoring_combos);

        return scoring_combos;
    }
    int GetThreesScore(Die d)
    {
        if(d.GetDieFace() == 1)
            return 300;
        else
            return d.GetDieFace() * 100;
    }
    void FindOnesFives(vector<ScoringCombo> & combos)
    {
        for(int i = 0; i < dice.size(); i++)
        {
            if(dice[i].GetDieFace() == 1 || dice[i].GetDieFace() == 5)
            {
                ScoringCombo c;
                c.dice.push_back(dice[i]);
                
                if(dice[i].GetDieFace() == 1)
                {
                    c.combo_name = "One";
                    c.value = 100;
                }
                if(dice[i].GetDieFace() == 5)
                {    
                    c.combo_name = "Five";
                    c.value = 50;
                }
                combos.push_back(c);
            }
        }
    }
    void FindSetsOfThree(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 3)
            return;

        for(int i = 0; i < dice.size() - 2; i++)
        {
            vector<Die> possible_match = {dice[i], dice[i+1], dice[i+2] };
            if(CheckEquals(possible_match, dice[i].GetDieFace()))
            {
                ScoringCombo combo;
                combo.dice.push_back(dice[i]); 
                combo.dice.push_back(dice[i + 1]); 
                combo.dice.push_back(dice[i + 2]);
                combo.value = GetThreesScore(dice[i]);
                combo.combo_name = "Set of 3";
                combos.push_back(combo);
            }
        }
    }
    void Find4OfAKind(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 4)
            return;
        
        for(int i = 0; i < dice.size() - 3; i++)
        {
            vector<Die> set = { dice[i], dice[i+1], dice[i+2], dice[i+3]};
            if(CheckEquals(set, dice[i].GetDieFace()))
            {
                ScoringCombo combo;
                combo.dice.push_back(dice[i]); 
                combo.dice.push_back(dice[i + 1]); 
                combo.dice.push_back(dice[i + 2]);
                combo.dice.push_back(dice[i + 3]);
                combo.value = 1000;
                combo.combo_name = "4 of a Kind";
                combos.push_back(combo);
            }
        }

    }
    void Find5OfAKind(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 5)
            return;
        
            for(int i = 0; i < dice.size() - 4; i++)
        {
            vector<Die> set = { dice[i], dice[i+1], dice[i+2], dice[i+3], dice[i+4] };
            if(CheckEquals(set, dice[i].GetDieFace()))
            {
                ScoringCombo combo;
                combo.dice.push_back(dice[i]); 
                combo.dice.push_back(dice[i + 1]); 
                combo.dice.push_back(dice[i + 2]);
                combo.dice.push_back(dice[i + 3]);
                combo.dice.push_back(dice[i + 4]);
                combo.value = 2000;
                combo.combo_name = "5 of a Kind";
                combos.push_back(combo);
            }
        }
    } 
    void Find6OfAKind(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 6)
            return;

        vector<Die> set = { dice[0], dice[1], dice[2], dice[3], dice[4], dice[5] };
        if(CheckEquals(set, dice[0].GetDieFace()))
        {
            ScoringCombo combo;
            combo.dice.push_back(dice[0]); 
            combo.dice.push_back(dice[1]); 
            combo.dice.push_back(dice[2]);
            combo.dice.push_back(dice[3]);
            combo.dice.push_back(dice[4]);
            combo.dice.push_back(dice[5]);
            combo.value = 3000;
            combo.combo_name = "6 of a Kind";
            combos.push_back(combo);
        }
    }
    void FindStraight1to6(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 6)
            return;
        
            if(dice[0].GetDieFace() == 1 && dice[1].GetDieFace() == 2 && dice[2].GetDieFace() == 3 &&
           dice[3].GetDieFace() == 4 && dice[4].GetDieFace() == 5 && dice[5].GetDieFace() == 6)
        {
            ScoringCombo combo;
            combo.dice.push_back(dice[0]); 
            combo.dice.push_back(dice[1]); 
            combo.dice.push_back(dice[2]);
            combo.dice.push_back(dice[3]);
            combo.dice.push_back(dice[4]);
            combo.dice.push_back(dice[5]);
            combo.value = 3000;
            combo.combo_name = "1 - 6 Straight";
            combos.push_back(combo);
        }
    }
    void Find3Pairs(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 6)
            return;

        if(dice[0].GetDieFace() == dice[1].GetDieFace() && 
           dice[2].GetDieFace() == dice[3].GetDieFace() &&
           dice[4].GetDieFace() == dice[5].GetDieFace())
        {
            ScoringCombo combo;
            combo.dice.push_back(dice[0]); 
            combo.dice.push_back(dice[1]); 
            combo.dice.push_back(dice[2]);
            combo.dice.push_back(dice[3]);
            combo.dice.push_back(dice[4]);
            combo.dice.push_back(dice[5]);
            combo.value = 1500;
            combo.combo_name = "3 Pairs of 2";
            combos.push_back(combo);
        }
    }
    void Find4OfAKindAndPair(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 6)
            return;

        //First, look for the dice that are 4 of a kind.
        vector<ScoringCombo> temp_combo;
        Find4OfAKind(temp_combo);

        //Next, see if it was found
        if(temp_combo.size() > 0)
        {
            ScoringCombo possible_4kindPair = temp_combo[0];

            //If it was, look for the remaining two dice.
            //Store the face up value of one of the 4 of a kind dice.
            int face_4kind = possible_4kindPair.dice[0].GetDieFace();

            //Next, look at the rolled dice and look for 2 pairs.
            for(int i = 0; i < dice.size() - 1; i++)
            {
                //If this dice is not part of the 4 of a kind set
                //and the next dice matches it, then it is 2 pairs.
                if(dice[i].GetDieFace() != face_4kind &&
                 dice[i].GetDieFace() == dice[i + 1].GetDieFace())
                {
                    possible_4kindPair.dice.push_back(dice[i]);
                    possible_4kindPair.dice.push_back(dice[i + 1]);
                    possible_4kindPair.combo_name = "4 of a Kind and a Pair";
                    combos.push_back(possible_4kindPair);
                    return;
                }   
            }
        }
    }
    void Find2Triplets(vector<ScoringCombo> & combos)
    {
        if(dice.size() < 6)
            return;

        vector<Die> set1 = {dice[0], dice[1], dice[2]};
        vector<Die> set2 = {dice[3], dice[4], dice[5]};
        if(CheckEquals(set1, dice[0].GetDieFace()) && CheckEquals(set2, dice[3].GetDieFace()))
        {
            ScoringCombo combo;
            combo.dice.push_back(dice[0]); 
            combo.dice.push_back(dice[1]); 
            combo.dice.push_back(dice[2]);
            combo.dice.push_back(dice[3]);
            combo.dice.push_back(dice[4]);
            combo.dice.push_back(dice[5]);
            combo.value = 2500;
            combo.combo_name = "2 Triplets";
            combos.push_back(combo);
        }
    }
    bool CheckEquals(vector<Die> dice_check, int value)
    {
        for(int i = 0; i < dice_check.size(); i++)
        {
            if(dice_check[i].GetDieFace() != value)
                return false;
        }

        return true;
    }
    void CommitScoringCombo(vector<ScoringCombo> & result, int choice)
    {
        choice = choice - 1;
        //1. Remove combo dice from dice roll set.
        for(int i = 0; i < result[choice].dice.size(); i++)
        {
            for (auto j = dice.begin(); j != dice.end(); j++) 
            {
                int jFace = j->GetDieFace();
                int resultFace = result[choice].dice[i].GetDieFace();

                if(jFace == resultFace)
                {
                    dice.erase(j);
                    break;
                }
            }
        }

        //2. Add score to player's total
        roundScore += result[choice].value;

        //3. Remove combination from list.
        result.erase(result.begin() + choice);
    }

};