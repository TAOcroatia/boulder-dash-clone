using namespace std;
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <ncurses.h>

#define ESC 27

//░▓■O╬
//DWPSG

//https://kirilllive.github.io/ASCII_Art_Paint/ascii_paint.html
//https://codeincomplete.com/games/boulderdash/play/

#include "board.h"

int main(){
    //Board testLvl("levels/testLevel.txt");

    cout << "Enter level location (CTRL+D (for default)): ";
    string location = "levels/lvl1.txt";
    cin >> location;
    cout << endl << location;

    Board testLvl(location);

    initscr();
    noecho();
    curs_set(0);
    timeout(50); //run game at 20 fps
    
    int i = 0;
    int j = 0;
    while(1){
        if(i == 4){
            i = 0;
            testLvl.checkAround();
        }

        if(!testLvl.alive){
            j++;
            if(j == 32) break;
        }

        if(testLvl.complete && testLvl.alive){
            j++;
            if(j == 32) break;
        }

        if (testLvl.playerMove() == ESC) break;
        testLvl.printBoard();

        i++;
    }

    curs_set(1);
    echo();
    endwin();

    if(!testLvl.alive){
        cout << endl << "Game over!" << endl;
    }
    if(testLvl.alive && testLvl.complete){
        cout << endl << "Level complete!" << endl;
    }

    return 0;
}
