#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int m_rows;
    int m_cols;
    
    struct Ship {
    public:
        Ship(int length, char symbol, string name) {
            m_length = length;
            m_symbol = symbol;
            m_name = name;
        }
        
        int m_ID;
        int m_length;
        char m_symbol;
        string m_name;
    };
    vector <Ship> m_ships;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship temp(length, symbol, name);
    temp.m_ID = m_ships.size(); //id fgoes from 0 to nships-1
    m_ships.push_back(temp);
    
    return true;  //successfully added so return true
}

int GameImpl::nShips() const
{
    return m_ships.size();
}

int GameImpl::shipLength(int shipId) const
{
    return m_ships[shipId].m_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_ships[shipId].m_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return m_ships[shipId].m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(!p1->placeShips(b1))
    {
        return nullptr; //if player's place ship fails return nullptr
    }
    if (!p2->placeShips(b2))
    {
        return nullptr; //if player's place ships fails return nullptr
    }
    while(!b1.allShipsDestroyed() && !b2.allShipsDestroyed())    //while still ships to be destroyed run the loop
    {
        bool shotHit;
        bool shipDestroyed;
        int shipId;
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        b2.display(p1->isHuman()); //display second player's board, if first player human show shots only
        Point temp1 = p1->recommendAttack();
        if(!b2.attack(temp1, shotHit, shipDestroyed, shipId)) //if attack failed
        {
            cout << p1->name() << " wasted a shot at ("<< temp1.r <<"," << temp1.c << ")." << endl;
            //if attack missed or was unnecessary, say they wasted a shot at that point
        }
        else    //shot was valid so determine if they hit or misssed, if ship sank, etc.
        {
            p1->recordAttackResult(temp1, true,shotHit, shipDestroyed,shipId);
            if (shipDestroyed) //ship sunk
            {
                cout << p1->name() << " attacked (" << temp1.r <<"," << temp1.c <<") and destroyed the " << m_ships[shipId].m_name <<", resulting in:" << endl;
            }
            else if (shotHit) //hit
            {   //Shuman the Human attacked (3,6) and hit something, resulting in:
                cout << p1->name() << " attacked (" << temp1.r <<"," << temp1.c <<") and hit something, resulting in:" << endl;
            }
            else if (!shotHit)   //miss
            {
                cout << p1->name() << " attacked (" << temp1.r <<"," << temp1.c <<") and missed, resulting in:" << endl;
            }
        }
        b2.display(p1->isHuman()); //show the result of the attack
        if (shouldPause && !b1.allShipsDestroyed() && !b2.allShipsDestroyed())
        {
            waitForEnter(); //pause game if shouldPause is true
        }
        
        if (b2.allShipsDestroyed()) //player 2 lost
        {
            if(p2->isHuman())
            {
                b2.display(false);
            }
            cout << p1->name() << " wins!" << endl;
            return p1;
        }
        
        //do player 2 now attacking player 1
        bool shotHit2;
        bool shipDestroyed2;
        int shipId2;
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        b1.display(p2->isHuman());
        Point temp2 = p2->recommendAttack();
        if(!b1.attack(temp2, shotHit2, shipDestroyed2, shipId2)) //if attack failed
        {
            cout << p2->name() << " wasted a shot at ("<< temp2.r <<"," << temp2.c << ")." << endl;
            //if attack missed or was unnecessary, say they wasted a shot at that point
        }
        else    //shot was valid so determine if they hit or misssed, if ship sank, etc.
        {
             p2->recordAttackResult(temp2, true,shotHit2, shipDestroyed2,shipId2);
            if (shipDestroyed2) //ship sunk
            {
                cout << p2->name() << " attacked (" << temp2.r <<"," << temp2.c <<") and destroyed the " << m_ships[shipId2].m_name <<", resulting in:" << endl;
            }
            else if (shotHit2) //only hit not sunk
            {
                cout << p2->name() << " attacked (" << temp2.r <<"," << temp2.c <<") and hit something, resulting in:" << endl;
            }
            else if (!shotHit2)   //miss
            {
                cout << p2->name() << " attacked (" << temp2.r <<"," << temp2.c <<") and missed, resulting in:" << endl;
            }
        }
        b1.display(p2->isHuman()); //show the result of the attack
         if (shouldPause && !b1.allShipsDestroyed() && !b2.allShipsDestroyed())
        {
            waitForEnter(); //pause game if shouldPause is true
        }
    
        if (b1.allShipsDestroyed()) //player 1 lost
        {
            if(p1->isHuman())
            {
                b1.display(false);
            }
            cout << p2->name() << " wins!" << endl;
            return p2;
        }
    }
    
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

