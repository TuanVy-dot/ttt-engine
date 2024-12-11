#include "engine/board.h"
#include "general_utils.h"
#include <assert.h>

int main() {
    assert(board_isInRange(POS(2, 1)));
    assert(! board_isInRange(POS(2, 3)));
    Board board;
    board_init(&board);
    assert(board_isValidMove(board, POS(1, 1)));
    board.cells[1][1] = X_PLAYER;
    assert(! board_isValidMove(board, POS(1, 1)));
    assert(! board_isValidMove(board, POS(5, 1)));
    return 0;
}
