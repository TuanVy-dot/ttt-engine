#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "engine/eval.h"
#include "engine/board.h"

// Helper function to print the board state (optional for debugging)
void print_board(Board board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", board.cells[i][j]);
        }
        printf("\n");
    }
}

// Test Case 1: Check if 'M1' is detected when the current player is X and has a winning move
void test_eval_HaveM1_X_Win() {
    Board board;
    cell_Value cells[9] = {X_PLAYER, X_PLAYER, BLANK_CELL,
                            O_PLAYER, O_PLAYER, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL};
    board_set(&board, cells);
    board.player_turn = X_PLAYER;

    // X should have a potential "M1" (winning move) in the top row
    assert(eval_HaveM1(board, board.player_turn) == true); 
}

// Test Case 2: Check if 'M1' is detected when the current player is O and has a winning move
void test_eval_HaveM1_O_Win() {
    Board board;
    cell_Value cells[9] = {X_PLAYER, O_PLAYER, BLANK_CELL,
                            X_PLAYER, O_PLAYER, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL};
    board_set(&board, cells);
    board.player_turn = O_PLAYER;

    // O should have a potential "M1" (winning move) in the top row
    assert(eval_HaveM1(board, board.player_turn) == true); 
}

// Test Case 3: No "M1" scenario (no winning move possible for current player)
void test_eval_HaveM1_No_Win() {
    Board board;
    cell_Value cells[9] = {X_PLAYER, O_PLAYER, X_PLAYER,
                            O_PLAYER, X_PLAYER, O_PLAYER,
                            O_PLAYER, X_PLAYER, O_PLAYER};
    board_set(&board, cells);
    board.player_turn = X_PLAYER;

    // No winning move for X
    assert(eval_HaveM1(board, board.player_turn) == false); 
}

// Test Case 4: Check if the function works when the board is empty (no moves made yet)
void test_eval_HaveM1_Empty_Board() {
    Board board;
    cell_Value cells[9] = {BLANK_CELL, BLANK_CELL, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL,
                            BLANK_CELL, BLANK_CELL, BLANK_CELL};
    board_set(&board, cells);
    board.player_turn = X_PLAYER;

    // No moves, so no "M1" for either player
    assert(eval_HaveM1(board, board.player_turn) == false); 
}

// Test Case 5: Check if the function detects an "M1" when player turn is O
void test_eval_HaveM1_O_Turn() {
    Board board;
    cell_Value cells[9] = {O_PLAYER, X_PLAYER, X_PLAYER,
                            BLANK_CELL, O_PLAYER, O_PLAYER,
                            X_PLAYER, X_PLAYER, BLANK_CELL};
    board_set(&board, cells);
    board.player_turn = O_PLAYER;

    // O should have an "M1" (winning move)
    assert(eval_HaveM1(board, board.player_turn) == true);
}

int main() {
    test_eval_HaveM1_X_Win();
    test_eval_HaveM1_O_Win();
    test_eval_HaveM1_No_Win();
    test_eval_HaveM1_Empty_Board();
    test_eval_HaveM1_O_Turn();

    printf("All tests passed!\n");
    return 0;
}
