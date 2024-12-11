#include "engine/board.h"
#include <stdbool.h>
#include "engine/eval.h"

/*
 * Aspects of evaluation:
 * - 2 in line, blockable: no point
 * - 2 in line, no block and is in turn(return win immediately)
 * - more then 1 two-in-line: immediate win
 * - The board is full (return Draw immediately if no win)
 * - potential 2 in line (each contribute to some points)
 *   Potential for multi-threats (immediate win)
 * The scale of [EVAL_MIN, EVAL_MAX] (in include/eval.h)
 * X is gonna be the max player
 */
double eval_EvaluateStatic(Board board) {
    double evaluation = 0.0;

    if (board.player_turn == BLANK_CELL) {
        // How am I supposed to evaluate without turn infomation?
        return 0.0;
    }

    /* IMMEDIATE SCENATIOS */
    board_Status status = board_checkWin(board);
    if (status == X_WIN) return EVAL_MAX;
    if (status == O_WIN) return EVAL_MIN;
    if (status == XO_DRAW) return 0.0;

    if (eval_HaveM1(board, board.player_turn)) {
        return (board.player_turn == X_PLAYER) ? EVAL_MAX : EVAL_MIN;
    }

    /* EVALUATION */


    return 0.0; // pass for now
}

/* Can win in 1 move (in player_turn's turn)
 * It undefined if the game already over
 * Say one has won, it might not detect it
 * It does what it does, have M1, not M0 or something 
 */
bool eval_HaveM1(Board board, cell_Value player_turn) {
    if (!player_turn) {
        return false; // is blank, we don't want that
    }
    for (int i = 0; i < 3; i++) {
        if (eval_isPotentialWin(
                (board.cells)[i][0],
                (board.cells)[i][1],
                (board.cells)[i][2],
                player_turn
            )
        ) {
            return true;
        }
    }

    for (int j = 0; j < 3; j++) {
        if (eval_isPotentialWin(
                (board.cells)[0][j],
                (board.cells)[1][j],
                (board.cells)[2][j],
                player_turn
            )
        ) {
            return 1;
        }
    }

    if (eval_isPotentialWin(
            (board.cells)[0][0],
            (board.cells)[1][1],
            (board.cells)[2][2],
            player_turn
        ) || 
        eval_isPotentialWin(
            (board.cells)[0][2],
            (board.cells)[1][1],
            (board.cells)[2][0],
            player_turn
        )
    ) {
        return true;
    }

    return false;
}

/* Check if player_turn is giving the opponent multi-threats
 * It's a win immediately with M2 if the opponent don't have M1 */
bool eval_HaveM2(Board board, cell_Value player_turn) {
    uint32_t count_potential_win = 0;
    for (int i = 0; i < 3; i++) {
        count_potential_win +=  eval_isPotentialWin(
                                    (board.cells)[i][0],
                                    (board.cells)[i][1],
                                    (board.cells)[i][2], 
                                    player_turn
        );
    }

    for (int j = 0; j < 3; j++) {
        count_potential_win +=  eval_isPotentialWin(
                                    (board.cells)[0][j],
                                    (board.cells)[1][j],
                                    (board.cells)[2][j], 
                                    player_turn
        );
    }

    count_potential_win +=  eval_isPotentialWin(
                                (board.cells)[0][0],
                                (board.cells)[1][1],
                                (board.cells)[2][2], 
                                player_turn
    );
    count_potential_win +=  eval_isPotentialWin(
                                (board.cells)[0][2],
                                (board.cells)[1][1],
                                (board.cells)[2][0], 
                                player_turn
    );

    return count_potential_win >= 2;
}

/* Check if the three cell values are a in lined two
 * player is set to the winner value */
bool eval_isPotentialWin(cell_Value c0, cell_Value c1, cell_Value c2,
                         cell_Value player_turn) {
    if ((c0 == player_turn) && ((c0 == c1 && c2 == BLANK_CELL) ||
                                (c0 == c2 && c1 == BLANK_CELL))) {
        return true;
    }
    if (c1 == player_turn && c1 == c2 && c0 == BLANK_CELL) {
        return true;
    }
    return false;
}
