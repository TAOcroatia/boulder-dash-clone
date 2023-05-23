using namespace std;
#include <iostream>

class Board{
    int vt, hz;
    int px, py;
    int score;
    int gemCount;
    char** brd;

    void readBoard(string filename);
    char validMove(int x, int y, char direction);
    bool gravityLogic(int x, int y);
    void explode();

    public:
    bool alive;
    bool complete;
    
    void printBoard();
    int playerMove();
    void checkAround();

    Board(string filename = "");
    //~Board();
};