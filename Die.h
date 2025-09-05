#include <cstdlib>
#include <string>
#include <sstream>

using std::rand;
using std::string;
using std::stringstream;

class Die
{
private:
    int currentFaceUp = 1;
    bool locked = false;

public:
    int GetDieFace() { return currentFaceUp; }
    bool IsLocked() { return locked; }
    void RollDice()
    {
        if(locked == false)
            currentFaceUp = rand() % 6 + 1;
    }
    void LockDie()
    {
        locked = true;
    }
    void UnlockDie()
    {
        locked = false;
    }

    //Added after class...
    string ToString()
    {

        stringstream ss;
        if(locked == false)
            ss << currentFaceUp;
        else
            ss << currentFaceUp << " (Locked) ";

        return ss.str();
    }

    bool operator< (const Die & right)
    {
        return this->currentFaceUp < right.currentFaceUp;
    }
    bool operator> (const Die & right)
    {
        return this->currentFaceUp > right.currentFaceUp;
    }
    bool operator<= (const Die & right)
    {
        return this->currentFaceUp <= right.currentFaceUp;
    }
    bool operator>= (const Die & right)
    {
        return this->currentFaceUp >= right.currentFaceUp;
    }
    bool operator== (const Die & right)
    {
        return this->currentFaceUp == right.currentFaceUp;
    }
    bool operator!= (const Die & right)
    {
        return this->currentFaceUp != right.currentFaceUp;
    }
};