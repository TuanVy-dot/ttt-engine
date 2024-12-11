#include "engine/board.h"
#include "general_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
    Board board;
    board_init(&board);
    assert(! board_isFull(board));

    board.cells[0][2] = X_PLAYER;
    board.cells[1][2] = X_PLAYER;
    assert(! board_isFull(board));

    board_set(&board, (cell_Value[9]){X_PLAYER, X_PLAYER, X_PLAYER, X_PLAYER, X_PLAYER, X_PLAYER, X_PLAYER, X_PLAYER, X_PLAYER});
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", board.cells[i][j]);
        }
    }
    putchar('\n');
    assert(board_isFull(board));

    return 0;
}
