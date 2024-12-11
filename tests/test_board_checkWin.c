#include "engine/board.h"
#include <assert.h>

int main() {
    Board board;
    board_init(&board);

    // Test: X wins on a row
    board_set(&board, (cell_Value[9]){X_PLAYER, X_PLAYER, X_PLAYER, BLANK_CELL, BLANK_CELL, BLANK_CELL, BLANK_CELL, BLANK_CELL, BLANK_CELL});
    assert(board_checkWin(board) == X_WIN);

    // Test: O wins on a row
    board_set(&board, (cell_Value[9]){O_PLAYER, O_PLAYER, O_PLAYER, BLANK_CELL, BLANK_CELL, BLANK_CELL, BLANK_CELL, BLANK_CELL, BLANK_CELL});
    assert(board_checkWin(board) == O_WIN);

    // Test: X wins on a column
    board_set(&board, (cell_Value[9]){X_PLAYER, BLANK_CELL, BLANK_CELL, X_PLAYER, BLANK_CELL, BLANK_CELL, X_PLAYER, BLANK_CELL, BLANK_CELL});
    assert(board_checkWin(board) == X_WIN);

    // Test: O wins on a column
    board_set(&board, (cell_Value[9]){O_PLAYER, BLANK_CELL, BLANK_CELL, O_PLAYER, BLANK_CELL, BLANK_CELL, O_PLAYER, BLANK_CELL, BLANK_CELL});
    assert(board_checkWin(board) == O_WIN);

    // Test: X wins on a diagonal
    board_set(&board, (cell_Value[9]){
            X_PLAYER, BLANK_CELL, BLANK_CELL,
            BLANK_CELL, X_PLAYER, BLANK_CELL,
            BLANK_CELL, BLANK_CELL, X_PLAYER});
    assert(board_checkWin(board) == X_WIN);

    // Test: O wins on a diagonal
    board_set(&board, (cell_Value[9]){O_PLAYER, BLANK_CELL, BLANK_CELL, BLANK_CELL, O_PLAYER, BLANK_CELL, BLANK_CELL, BLANK_CELL, O_PLAYER});
    assert(board_checkWin(board) == O_WIN);

    // Test: No winner, game is ongoing
    board_set(&board, (cell_Value[9]){
            X_PLAYER, O_PLAYER, X_PLAYER,
            O_PLAYER, BLANK_CELL, O_PLAYER,
            X_PLAYER, O_PLAYER, X_PLAYER});
    assert(board_checkWin(board) == ONGOING);

    // Test: Draw (no winner, board is full)
    board_set(&board, (cell_Value[9]){
            X_PLAYER, O_PLAYER, O_PLAYER,
            O_PLAYER, X_PLAYER, X_PLAYER,
            X_PLAYER, O_PLAYER, O_PLAYER});
    assert(board_checkWin(board) == XO_DRAW);
}
