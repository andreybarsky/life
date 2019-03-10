#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <chrono>
#include <thread>

using namespace std;
typedef unordered_map<int, bool> boardmap;

class Board{
public:
    int board_rows;
    int board_cols;
    boardmap map1;
    boardmap map2;

    boardmap *cur_map;
    boardmap *next_map;

    Board(int rows, int cols);
    void display();
    void update();
    void play(int rate);

    int neighbours(int row, int col);
    int index(int row, int col);

    bool get(int row, int col);
    void set(int row, int col, bool state);
    void transfer();
};

Board::Board(int rows, int cols) {
    board_rows = rows;
    board_cols = cols;

    cur_map = &map1;
    next_map = &map2;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(i,j,false);
        }
    }
}


int Board::index(int row, int col) {
    // return 1D index corresponding to (row, col) pair
    return (row * board_cols) + col;
}


bool Board::get(int row, int col) {
    // check if board is defined here:
    if (row >= 0 && row < board_rows) 
        if (col >= 0 && col < board_cols) 
            return (*cur_map)[index(row, col)];
    return false;
}

void Board::set(int row, int col, bool state) {
    // check if board is defined here
    assert (row >= 0 && row < board_rows);
    assert (col >= 0 && col < board_cols);
    (*next_map)[index(row, col)] = state;
    
    return;
}

int Board::neighbours(int row, int col) {
    int n = get(row,col) ? -1 : 0; // account for centre coordinate value 

    for (int r = row-1; r <= row+1; r++) {
        for (int c = col-1; c <= col+1; c++) {
            if (get(r, c)) 
                n++;        
        }
    }
    return n;
}

void Board::display() {
    for (int r = 0; r < board_rows; r++) {
        for (int c = 0; c < board_cols; c++) {
            cout << (get(r,c) ? '#' : ' ');
        }
        cout << endl;
    }
}

void Board::transfer() {
    // switch double buffer between next_map and cur_map
    boardmap *temp;
    temp = cur_map;
    cur_map = next_map;
    next_map = temp;
}

void Board::update() {
    for (int r = 0; r < board_rows; r++) {
        for (int c = 0; c < board_cols; c++) {
            int n = neighbours(r, c);
            if (get(r,c)) { // alive
                if ((n > 1) && (n < 4)) { // too few or too many neighbours
                    set(r,c, true);
                }
                else {
                    set(r,c, false);
                }
            }

            else {
                if (n == 3) { // exactly 3 neighbours
                    set(r,c, true);
                }
                else {
                    set(r,c, false);
                }
            }
            
        }
    }
    // set cur_map to next_map:
    transfer();
}

void Board::play(int rate) {
    while (true) {
        system("clear");
        display();
        this_thread::sleep_for(chrono::milliseconds(rate));
        update();
    }
}

int main() {

    int num_rows = 30;
    int num_cols = 50;
    Board board(num_rows, num_cols);

    // acorn:
    board.set(10,10,true);
    board.set(12,9,true);
    board.set(12,10,true);
    board.set(11,12,true);
    board.set(12,13,true);
    board.set(12,14,true);
    board.set(12,15,true);
    
    board.transfer();

    board.display();
    board.update();
    board.display();
    board.play(10);
}