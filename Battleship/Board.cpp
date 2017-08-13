#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    char board [MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    m_rows = m_game.rows();
    m_cols = m_game.cols(); //grab the number of rows and cols from game for usage later
    
    clear();
}

void BoardImpl::clear()
{
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            board[i][j] = '.';      //clear the board by putting '.' every point
        }
    }
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                board[r][c] = '#'; //# sign used to denote a blocked location
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (board[r][c] == '#')
            {
                board[r][c] = '.'; //unblock by putting back the period
            }
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId < 0 || shipId>=m_game.nShips())
    {
        return false;   //if shipId is negative or bigger than the number of ships is false
    }
    if (m_game.isValid(topOrLeft) == false)
    {
        return false; //point is outside of the board
    }
    if (board[topOrLeft.r][topOrLeft.c] != '.')
    {
        return false;   //if ship overlaps already placed ship or is blocked
    }
    char idCheck = m_game.shipSymbol(shipId);
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            if (board[i][j] == idCheck)
            {
                return false;   //if the ship symbol is already on the board, return false
            }
        }
    }
    
    //check overlap ship or if ship would be partly or fully outside the board
    
    if (dir == HORIZONTAL) //for horizontal only column increases
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (board[topOrLeft.r][topOrLeft.c + i] != '.')
            {
                return false;    //if not an empty spot on the board return false
            }
            Point temp(topOrLeft.r, topOrLeft.c+i);
            if (m_game.isValid(temp) == false)
            {
                return false; //if not a point on the board return false
            }
        }
        //check all points before placing the ships with the symbol and returning true
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            board[topOrLeft.r][topOrLeft.c+i] = idCheck; //set the board to the ship symbol
        }
        return true;
    }
    else if (dir == VERTICAL) //for vertical only row increases
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (board[topOrLeft.r+i][topOrLeft.c] != '.')
            {
                return false;    //if not an empty spot on the board return false
            }
            Point temp(topOrLeft.r+i, topOrLeft.c);
            if (m_game.isValid(temp) == false)
            {
                return false; //if not a point on the board return false
            }
        }
        //check all points before placing the ships with the symbol and returning true
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            board[topOrLeft.r+i][topOrLeft.c] = idCheck; //set the board to the ship symbol
        }
        return true;
    }
    
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return false; // This compiles, but may not be correct
}

void BoardImpl::display(bool shotsOnly) const
{
    // This compiles, but may not be correct
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return false; // This compiles, but may not be correct
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            if ((board[i][j] != '.')&&(board[i][j] != 'X') && (board[i][j] != 'o'))
            {
                return false; //it is character other than empty board, hit and miss
            }
        }
    }
    return true; //only '.', 'X', and 'o' remain
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
