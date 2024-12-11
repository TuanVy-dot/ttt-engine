#include "engine/board.h"
#include "general_utils.h"
#include <assert.h>
#include <stdio.h>

int main() {
    Board board1;
    board_init(&board1);
    assert(board_isValid(board1));

    Board board2;
    board_init(&board2);
    board2.cells[0][0] = 5; // not in ternary (0, 1, 2) range
    assert(!board_isValid(board2));

    return 0;
}
