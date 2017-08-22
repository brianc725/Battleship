#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
//typedef AwfulPlayer HumanPlayer;

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player {
public:
    HumanPlayer(string nm, const Game &g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board &b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer::HumanPlayer(string nm, const Game &g)
:Player(nm, g)
{
    //constructor using initialization list
}

bool HumanPlayer::isHuman() const
{
    return true;
}

bool HumanPlayer::placeShips(Board &b)
{
    cout << name() << " must place " << game().nShips() << " ships." << endl;
    for (int i = 0; i < game().nShips(); i++)
    {
        bool validDir = false;
        b.display(false); //show the board each time with ships on it
        Direction dir;
        while (!validDir)
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            char direction;
            cin >> direction;
            if (direction == 'h')
            {
                validDir = true;
                dir = HORIZONTAL;
            }
            else if (direction == 'v')
            {
                dir = VERTICAL;
                validDir = true;
            }
            else
            {
                cout << "Direction must be h or v." << endl;
            }
        }
        bool validPt = false;
        while (!validPt)    //check if valid pt
        {
            bool validNums = false;
            int r = 0;
            int c = 0;
            while (!validNums)  //check if valid ints
            {
                if (dir == HORIZONTAL)
                {
                    cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
                }
                else if (dir == VERTICAL)
                {
                    cout << "Enter row and column of topmost cell (e.g. 3 5): ";
                }
                if (getLineWithTwoIntegers(r, c))
                {
                    validNums = true; //got the two integers successfully
                }
                else
                {
                    cout << "You must enter two integers." << endl;
                }
            }
            Point temp(r,c);
            if (b.placeShip(temp, i, dir) == true)  //if we can place a ship at that location, end while loop
            {
                validPt = true;
            }
            else
            {
                cout << "The ship can not be placed there." << endl;
            }
        }
    } //end of for loop
    
    return true;
}

Point HumanPlayer::recommendAttack()
{
    int r = 0;
    int c = 0;
    bool run = false;
    while(run == false)
    {
        cout << "Enter the row and column to attack (e.g, 3 5): ";
        run = getLineWithTwoIntegers(r, c);
    }
    Point temp(r, c);
    return temp;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    //does nothing for a human player
}

void HumanPlayer::recordAttackByOpponent(Point p)
{
    //does nothing for a human player
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

class MediocrePlayer: public Player
{
public:
    MediocrePlayer(string nm, const Game &g);
    virtual bool placeShips(Board &b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool placeRecursive(Board &b, int shipId, Point p1);
    
    int m_state; //mediocre is initially in state 1
    char m_local [MAXROWS][MAXCOLS];
    Point previous;
};

MediocrePlayer::MediocrePlayer(string nm, const Game &g)
:Player(nm, g)
{
    m_state = 1;
    //constructor using initialization list
    for(int i = 0; i < g.rows(); i++)
    {
        for (int j = 0; j < g.cols(); j++)
        {
            m_local[i][j] = 'o'; //set local board to empty
        }
    }
}

bool MediocrePlayer::placeShips(Board &b)
{
    bool placed = false;
    for (int i = 0; i < 50; i++)
    {
        b.block(); //block half of the positions on the board
        int startShip = 0;
        Point start(0,0);
        placed = placeRecursive(b, startShip, start); //start in upper left corner with first ship and move from there
        if (placed)
        {
            b.unblock(); //if successfully placed, unblock board and return true immediately
            return true;
        }
        b.unblock();
    }
    return false; //if it got to this point then it could not place the ships
}

bool MediocrePlayer::placeRecursive(Board &b, int shipId, Point p1)
{

    if (shipId >= game().nShips())
    {
        return true; //we went through all the ships
    }
    if (!(game().isValid(p1)))
    {
        return false;    //point is outside of range so go back
    }
    if (b.placeShip(p1, shipId, HORIZONTAL))
    {
        if(placeRecursive(b, shipId+1, p1))
        {
            return true; //was able to place ship properly so place the next ship
        }
        else {
            b.unplaceShip(p1, shipId, HORIZONTAL); //ship could not be placed later so remove them
        }
    }
    if (b.placeShip(p1, shipId, VERTICAL)) //if horizontal doesnt work try vertical
    {
        if(placeRecursive(b, shipId+1, p1))
        {
            return true; //was able to place ship properly so place the next ship
        }
        else {
            b.unplaceShip(p1, shipId, VERTICAL); //ship could not be placed later so remove them
        }
    }
    //move one point at a time if point is taken
    if(p1.r == game().rows()-1)
    {
        Point p2(0, p1.c+1); //if can move down move down
        if(placeRecursive(b, shipId, p2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else {
        Point p2(p1.r+1, p1.c); //otherwise move to the right
        if(placeRecursive(b, shipId, p2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
     
}

Point MediocrePlayer::recommendAttack()
{
    if (m_state == 1)
    {
        bool validShot = false;
        while (!validShot)
        {
            Point temp = game().randomPoint();
            validShot = game().isValid(temp); //keep getting a random point if it is not valid
            if (m_local[temp.r][temp.c] == 'X')
            {
                validShot = false; //if already fired there pick again
            }
            else
            {
                m_local[temp.r][temp.c] = 'X';
                return temp;
            }
        }

    }
    else if (m_state == 2)
    {
        bool invalid = true;
        int x = 4;
        int y = 4;
        while (invalid)//handle row 4 to -4
        {
            Point temp = Point(previous.r+x, previous.c);
            if ((m_local[temp.r][temp.c] == 'o') && (game().isValid(temp)))
            {
                invalid = false;
                m_local[temp.r][temp.c] = 'X'; //mark as used now
                return temp;
            }
            else if (x >= -4)
            {
                x--;
            }
            else
            {
                invalid = false;
            }
        }
        bool invalidCol = true;
        while (invalidCol)
        {
            Point temp = Point(previous.r, previous.c+y);
            if ((m_local[temp.r][temp.c] == 'o') && (game().isValid(temp)))
            {
                invalidCol = false;
                m_local[temp.r][temp.c] = 'X';
                return temp;
            }
            else if (y >= -4)
            {
                y--;
            }
            else
            {
                m_state = 1;
                return game().randomPoint(); //if really nothing just do a random point and go back to state 1
            }

        }
    }
    return Point(0,0);
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if (m_state == 1)
    {
        if (shotHit)
        {
            previous = p; //save where to go +/- 4 from
            m_state = 2;
        }
    }
    else if (m_state == 2)
    {
        if (shipDestroyed)
        {
            m_state = 1;
        }
        
    }
}

void MediocrePlayer::recordAttackByOpponent(Point p)
{
    //does nothing for a mediocre player
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef MediocrePlayer GoodPlayer;

/*
class GoodPlayer: public Player
{
public:
    virtual bool placeShips(Board &b);
    virtual Point reccomendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

bool GoodPlayer::placeShips(Board &b)
{
    
    return true; //THIS IS NOT CORRECT MUST DELETE THIS
}

Point GoodPlayer::reccomendAttack()
{
    Point temp;
    return temp; //THIS IS NOT CORRECT MUST DELETE THIS
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    
}

void GoodPlayer::recordAttackByOpponent(Point p)
{
    //does nothing for a mediocre player
}
*/
 
//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
