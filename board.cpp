#include "board.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'

#define PLAYER 'P'
#define AIR ' '
#define WALL '&'
#define DIRT ':'
#define GEM '*'
#define BOULDER 'O'
#define EXIT 'E'

//  X
// Y0123
//  1
//  2
//  3

void Board::readBoard(string filename){
    ifstream file;
    try{
    file.open(filename);  
    }catch(std::invalid_argument){
        printw("File name incorrect.");
    }

    string tmp;
    getline(file, tmp); //skip top line
    
    for(int i = 0; i < hz; i++){
        getline(file, tmp);
        for(int j = 0; j < vt; j++){
            brd[i][j] = tmp[j];
            
            if(brd[i][j] == PLAYER){
                px = i; py = j;
            }
            if(brd[i][j] == GEM){
                gemCount++;
            }
        }
    }
}

void Board::printBoard(){
    move(0,0);

    for(int i = 0; i < hz; i++){
        for(int j = 0; j < vt; j++){
            printw("%c", brd[i][j]);
        }
        printw("\n");
    }

    printw("X: %d Y: %d     Gems left: %d\n", px, py, (gemCount - score));

    refresh();
}

char Board::validMove(int x, int y, char direction){
    if (brd[x][y] == WALL) return 0;
    
    if (brd[x][y] == GEM){
        score++;
    }

    if (direction == UP || direction == DOWN){
        if (brd[x][y] == BOULDER) return 0;
    }

    if (direction == LEFT && brd[x][y] == BOULDER){
        if (brd[x][y-1] == AIR){
            brd[x][y-1] = BOULDER;
        }else{
            return 0;
        }
    }

    if (direction == RIGHT && brd[x][y] == BOULDER){
        if (brd[x][y+1] == AIR){
            brd[x][y+1] = BOULDER;
        }else{
            return 0;
        }
    }

    if (brd[x][y] == EXIT && score != gemCount) return 0;
    else if(brd[x][y] == EXIT && score == gemCount) complete = 1;

    return 1;
}

void Board::checkAround(){
    bool moved = false;
    for (int x = hz-1; x >= 0; x--){
        for(int y = 0; y <= vt-1; y++){
            if(gravityLogic(x, y)) moved = 1;
        }
    }
}

bool Board::gravityLogic(int x, int y){
    if ((brd[x][y] != BOULDER) && (brd[x][y] != GEM)) return 0;

    if(brd[x+1][y] == AIR){
        brd[x+1][y] = brd[x][y];
        brd[x][y] = AIR;
        x++;
        if(brd[x+1][y] == PLAYER) alive = 0;
        return 1;
    }
    if((brd[x+1][y] == BOULDER || brd[x+1][y] == GEM) && brd[x][y-1] == AIR && brd[x+1][y-1] == AIR){
        brd[x+1][y-1] = brd[x][y];
        brd[x][y] = AIR;
        x++; y--;
        if(brd[x+1][y] == PLAYER) alive = 0;
        return 1;
    }
    if((brd[x+1][y] == BOULDER || brd[x+1][y] == GEM) && brd[x][y+1] == AIR && brd[x+1][y+1] == AIR){
        brd[x+1][y+1] = brd[x][y];
        brd[x][y] = AIR;
        x++; y++;
        if(brd[x+1][y] == PLAYER) alive = 0;
        return 1;
    }
    
    return 0;
}

void Board::explode(){
    for(int x = px-1; x <= px+1; x++){
        for(int y = py-1; y <= py+1; y++){
            if(brd[x][y] != WALL) brd[x][y] = AIR;
        }
    }
}

int Board::playerMove(){
    int npx, npy;
    char input = getch();

    if (!alive && !complete){
        explode();
        complete = 1;
    }

    if (!alive || complete) return input;
    
    switch(input){
        case LEFT:
            npx = px;
            npy = py - 1;
            break;
        case RIGHT:
            npx = px;
            npy = py + 1;
            break;
        case UP:
            npx = px - 1;
            npy = py;
            break;
        case DOWN:
            npx = px + 1;
            npy = py;
            break;
        default:
            return input;
    }

    if (!validMove(npx, npy, input)) return input;// input has to be wasd

    brd[npx][npy] = PLAYER;
    brd[px][py] = AIR;
    px = npx; py = npy;

    return input;
}

Board::Board(string filename){
    //get board length
    string tmp;
    ifstream file(filename);
    getline(file, tmp, ' ');
    vt = stoi(tmp);
    getline(file, tmp);
    hz = stoi(tmp);
    file.close();

    
    brd = (char**)malloc(sizeof(char**) * vt);
    for(int i = 0; i < hz; i++){
        brd[i] = (char*)malloc(sizeof(char*) * hz);
    }

    gemCount = 0;

    if (filename[0] != '\0'){
        readBoard(filename);
    }

    score = 0;
    alive = 1;
    complete = 0;
}