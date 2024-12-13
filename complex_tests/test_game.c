#include "engine/board.h"
#include "engine/eval.h"
#include <stdio.h>

/* THIS IS FOR TEST PURPOSE
 * Don't complain why it uses scanf */

int main() {
    Board board;
    board_init(&board);

    board_Status won;
    printf("You are %d\n", O_PLAYER);
    while (1) {
        board_print(board);
        fflush(stdin);
        if (board.player_turn == BLANK_CELL || (won = board_checkWin(board)) != ONGOING) {
            if (won != XO_DRAW) {
                printf("Player %d won\n", won);
            } else {
                printf("Draw\n");
            }
            break;
        }
        if (board.player_turn == X_PLAYER) {
            struct board_Pos best_move = eval_getBestMove(board, 9);
            board_play(&board, best_move);
        } else {
            int i, j;
            printf("type in fmt: row col (it use scanf, please don't try to break it)\n");
            scanf("%d %d", &i, &j);
            board_play(&board, POS(i, j));
        }
    }
}
