#include "engine/board.h"
#include "engine/eval.h"

int main() {
    Board board;
    board_init(&board);
    board_set(&board, 
        (cell_Value[9]){
            BLANK_CELL, BLANK_CELL, X_PLAYER,
            O_PLAYER, BLANK_CELL, X_PLAYER,
            X_PLAYER, BLANK_CELL, O_PLAYER
        }
    );
    board_print(board);
    board_play(&board, eval_getBestMove(board, 3));
    board_print(board);
    return 0;
}
