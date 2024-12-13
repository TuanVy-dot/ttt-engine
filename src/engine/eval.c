#include "engine/board.h"
#include <stdbool.h>
#include <float.h>
#include "engine/eval.h"
#include "general_utils.h"

struct board_Pos eval_getBestMove(Board position, uint32_t depth) {
    struct board_Pos blanks[9];
    uint32_t n_blanks = board_getBlankPos(position, blanks);
    if (position.player_turn == X_PLAYER) {
        // maximizing
        struct board_Pos best;
        double best_eval = -INFINITY;
        for (uint32_t i = 0; i < n_blanks; i++) {
            Board child = board_playC(position, blanks[i]);
            double eval = eval_EvaluateDepth(child, depth);
            if (best_eval < eval) {
                best = blanks[i];
                best_eval = eval;
            }
        }
        return best;
    } else {
        // minimizing
        struct board_Pos best;
        double best_eval = INFINITY;
        for (uint32_t i = 0; i < n_blanks; i++) {
            Board child = board_playC(position, blanks[i]);
            double eval = eval_EvaluateDepth(child, depth);
            if (best_eval > eval) {
                best = blanks[i];
                best_eval = eval;
            }
        }
        return best;
    }
}

double eval_EvaluateDepth(Board position, uint32_t depth) {
    if (depth == 0 || board_checkWin(position) != ONGOING) {
        return eval_EvaluateStatic(position);
    }

    if (position.player_turn == X_PLAYER) {
        double maxEval;
        // MaximizingPlayer
        maxEval = -INFINITY;
        struct board_Pos blanks[9];
        uint32_t n_blanks = board_getBlankPos(position, blanks);
        for (uint32_t i = 0; i < n_blanks; i++) {
            Board child = board_playC(position, blanks[i]);
            double eval = eval_EvaluateDepth(child, depth - 1);
            eval += depth; // add the depth to show how long it take ro achive that
            maxEval = max(maxEval, eval);
        }
        return maxEval;
    } else {
        // MinimizingPlayer
        double minEval;
        minEval = INFINITY;
        struct board_Pos blanks[9];
        uint32_t n_blanks = board_getBlankPos(position, blanks);
        for (uint32_t i = 0; i < n_blanks; i++) {
            Board child = board_playC(position, blanks[i]);
            double eval = eval_EvaluateDepth(child, depth - 1);
            eval -= depth;
            minEval = min(minEval, eval);
        }
        return minEval;
    }
}

/*
 * Aspects of evaluation:
 * - 2 in line, blockable: no point
 * - Blockable threats are no points
 * - 2 in line, no block and is in turn(return win immediately)
 * - more then 1 two-in-line: immediate win
 *   Number of winning paths without opponents
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

    // If your opponent is giving you multi-threats,
    // you are lost (since you don't have M1 as checked above)
    if (eval_HaveM2(board, opponent_of(board.player_turn))) {
        // give opposite max magnitude
        return (board.player_turn == X_PLAYER) ? EVAL_MIN : EVAL_MAX;
    }

    /* EVALUATION */
    
    // Controlled open paths
    evaluation += eval_countOpenWinningPath(board, X_PLAYER) * CONTROLLED_OPEN_PATH_WEIGHT;
    evaluation -= eval_countOpenWinningPath(board, O_PLAYER) * CONTROLLED_OPEN_PATH_WEIGHT;

    // Center control
    evaluation += eval_MaxOrMin((board.cells)[1][1]) * CONTROLLED_CENTER_WEIGHT;

    // Corner control
    evaluation += eval_MaxOrMin((board.cells)[0][0]) * CONTROLLED_CORNER_WEIGHT;
    evaluation += eval_MaxOrMin((board.cells)[0][2]) * CONTROLLED_CORNER_WEIGHT;
    evaluation += eval_MaxOrMin((board.cells)[2][0]) * CONTROLLED_CORNER_WEIGHT;
    evaluation += eval_MaxOrMin((board.cells)[2][2]) * CONTROLLED_CORNER_WEIGHT;
    
    return evaluation; // pass for now
}

/* Can win in 1 move (in player_turn's turn)
 * It undefined if the game already over
 * Say one has won, it might not detect it
 * It does what it does, have M1, not M0 or something 
 */
bool eval_HaveM1(Board board, cell_Value player_turn) {
    if (player_turn == BLANK_CELL) {
        return false; // is blank, we don't want that
    }
    for (int i = 0; i < 3; i++) {
        if (eval_isPotentialWin(
                (board.cells)[i][0],
                (board.cells)[i][1],
                (board.cells)[i][2],
                player_turn
            ) != -1
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
            ) != -1
        ) {
            return 1;
        }
    }

    if (eval_isPotentialWin(
            (board.cells)[0][0],
            (board.cells)[1][1],
            (board.cells)[2][2],
            player_turn
        ) != -1 || 
        eval_isPotentialWin(
            (board.cells)[0][2],
            (board.cells)[1][1],
            (board.cells)[2][0],
            player_turn
        ) != -1
    ) {
        return true;
    }

    return false;
}

/* Check if player_turn is giving the opponent multi-threats
 * It's a win immediately with M2 if the opponent don't have M1 */
bool eval_HaveM2(Board board, cell_Value player_turn) {
    if (player_turn == BLANK_CELL) {
        return false;
    }
    bool threat_cells[3][3] = {0};
    uint32_t count_potential_win = 0;
    int32_t threat_index;
    for (int i = 0; i < 3; i++) {
        threat_index = eval_isPotentialWin(
            (board.cells)[i][0],
            (board.cells)[i][1],
            (board.cells)[i][2], 
            player_turn
        );
        if (threat_index != -1 && ! threat_cells[i][threat_index]) {
            (threat_cells)[i][threat_index] = true;
            if (++count_potential_win >= 2) return true;
        }
    }

    for (int j = 0; j < 3; j++) {
        threat_index = eval_isPotentialWin(
                                    (board.cells)[0][j],
                                    (board.cells)[1][j],
                                    (board.cells)[2][j], 
                                    player_turn
        );
        if (threat_index != -1 && ! threat_cells[threat_index][j]) {
            (threat_cells)[threat_index][j] = true;
            if (++count_potential_win >= 2) return true;
        }
    }

    threat_index = eval_isPotentialWin(
                                (board.cells)[0][0],
                                (board.cells)[1][1],
                                (board.cells)[2][2], 
                                player_turn
    );
    if (threat_index != -1 && ! threat_cells[threat_index][threat_index]) {
        threat_cells[threat_index][threat_index] = true;
        if (++count_potential_win >= 2) return true;
    }

    threat_index = eval_isPotentialWin(
                                (board.cells)[0][2],
                                (board.cells)[1][1],
                                (board.cells)[2][0], 
                                player_turn
    );
    if (threat_index != -1 && ! threat_cells[threat_index][2 - threat_index]) {
        threat_cells[threat_index][2 - threat_index] = true;
        if (++count_potential_win >= 2) return true;
    }

    return false;
}

/* Check if the three cell values are a in lined two
 * Return index of the cell won (as arguement 0, 1, 2) or -1 if failed
 */
int32_t eval_isPotentialWin(cell_Value c0, cell_Value c1, cell_Value c2,
                         cell_Value player_turn) {
    if ((c0 == player_turn && c0 == c1 && c2 == BLANK_CELL)) {
        return 2;
    }
    if (c0 == player_turn && c0 == c2 && c1 == BLANK_CELL) {
        return 1;
    }
    if (c1 == player_turn && c1 == c2 && c0 == BLANK_CELL) {
        return 0;
    }
    return -1;
}

/* Count number of winning paths without opponent */
uint32_t eval_countOpenWinningPath(Board board, cell_Value player_turn) {
    if (player_turn == BLANK_CELL) {
        return 0;
    }
    uint32_t count = 0;
    for (int i = 0; i < 3; i++) {
        if (
            (board.cells)[i][0] != (opponent_of(player_turn)) &&
            (board.cells)[i][1] != (opponent_of(player_turn)) &&
            (board.cells)[i][2] != (opponent_of(player_turn)) &&
            ((board.cells)[i][0] || (board.cells)[i][1] || (board.cells)[i][2])
        ) {
            // At least one of your mark controlled an open row (no opponent value)
            count++;
        }
    }

    for (int j = 0; j < 3; j++) {
        if (
            (board.cells)[0][j] != (opponent_of(player_turn)) &&
            (board.cells)[1][j] != (opponent_of(player_turn)) &&
            (board.cells)[2][j] != (opponent_of(player_turn)) &&
            ((board.cells)[0][j] || (board.cells)[1][j] || (board.cells)[2][j])
        ) {
            count++;
        }
    }

    if (
        (board.cells)[0][0] != (opponent_of(player_turn)) &&
        (board.cells)[1][1] != (opponent_of(player_turn)) &&
        (board.cells)[2][2] != (opponent_of(player_turn)) &&
        ((board.cells)[0][0] || (board.cells)[1][1] || (board.cells)[2][2])
    ) {
        count++;
    }

    if (
        (board.cells)[0][2] != (opponent_of(player_turn)) &&
        (board.cells)[1][1] != (opponent_of(player_turn)) &&
        (board.cells)[2][0] != (opponent_of(player_turn)) &&
        ((board.cells)[0][2] || (board.cells)[1][1] || (board.cells)[2][0])
    ) {
        count++;
    }
    return count;
}

/* Return 1 for max player (X) or -1 */
int32_t eval_MaxOrMin(cell_Value player) {
    return (player == X_PLAYER) ? 1 : (player == O_PLAYER) ? -1 : 0;
}
