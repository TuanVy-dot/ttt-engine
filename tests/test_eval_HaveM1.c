#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "engine/eval.h"
#include "engine/board.h"

void test_eval_HaveM1_X_Win() {
    Board board;
    cell_Value cells[9] = {X_PLAYER, X_PLAYER, BLANK_CELL,
                            O_PLAYER, O_PLAYER, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL};
    board_set(&board, cells);
    board.player_turn = X_PLAYER;

    assert(eval_HaveM1(board, board.player_turn) == true); 
}

void test_eval_HaveM1_O_Win() {
    Board board;
    cell_Value cells[9] = {X_PLAYER, O_PLAYER, BLANK_CELL,
                            X_PLAYER, O_PLAYER, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL};
    board_set(&board, cells);
    board.player_turn = O_PLAYER;

    assert(eval_HaveM1(board, board.player_turn) == true); 
}

void test_eval_HaveM1_No_Win() {
    Board board;
    cell_Value cells[9] = {X_PLAYER, O_PLAYER, X_PLAYER,
                            O_PLAYER, X_PLAYER, O_PLAYER,
                            O_PLAYER, X_PLAYER, O_PLAYER};
    board_set(&board, cells);
    board.player_turn = X_PLAYER;

    assert(eval_HaveM1(board, board.player_turn) == false); 
}

void test_eval_HaveM1_Empty_Board() {
    Board board;
    cell_Value cells[9] = {BLANK_CELL, BLANK_CELL, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL};
    board_set(&board, cells);
    board.player_turn = X_PLAYER;

    assert(eval_HaveM1(board, board.player_turn) == false); 
}

int main() {
    test_eval_HaveM1_X_Win();
    test_eval_HaveM1_O_Win();
    test_eval_HaveM1_No_Win();
    test_eval_HaveM1_Empty_Board();
    return 0;
}
