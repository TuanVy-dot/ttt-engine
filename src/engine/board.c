#include <stdlib.h>
#include "engine/board.h"
#include "general_utils.h"

void board_init(Board *board) {
    memset(board -> cells, BLANK_CELL, 9 * sizeof(cell_Value));
    board -> player_turn = X_PLAYER;
}

/* Check if a board is valid,
 * that is cells and player_turn are all in ternary (0, 1, 2)
 * O might move first or O already moves 5 turns before X,
 * It wouldn't matter, this function check if board data is in ternary only
 * Ensure the validness of the position, not if it follow the game rule */
bool board_isValid(Board board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (! II_INRANGE((board.cells)[i][j], 0, 2)) {
                return false;
            }
        }
    }
    return II_INRANGE(board.player_turn, 0, 2);
}

/* Check if a move is valid */
bool board_isInRange(struct board_Pos pos) {
    return II_INRANGE(pos.row, 0, 2) && II_INRANGE(pos.col, 0, 2);
}
bool board_isValidMove(Board board, struct board_Pos pos) {
    return board_isInRange(pos)
           && (board.cells)[pos.row][pos.col] == BLANK_CELL;
}

bool board_isFull(Board board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((board.cells)[i][j] == BLANK_CELL) {
                return false;
            }
        }
    }
    return true;
}

/* Return X_WIN or O_WIN or XO_DRAW
 * or ONGOING if the game is not over
 * If multiple win are in the board, it return the one it found first
 * It shouldn't ever happend in a game, by the way */
board_Status board_checkWin(Board board) {
    uint8_t c;
    for (int i = 0; i < 3; i++) {
        if ((c = (board.cells)[i][0]) 
            && c == (board.cells)[i][1]
            && c == (board.cells)[i][2]) {
            return c;
        }
    }

    for (int j = 0; j < 3; j++) {
        if ((c = (board.cells)[0][j]) 
            && c == (board.cells)[1][j]
            && c == (board.cells)[2][j]) {
            return c;
        }
    }

    if ((c = (board.cells)[0][0]) 
        && c == (board.cells)[1][1]
        && c == (board.cells)[2][2]) {
        return c;
    }

    if ((c = (board.cells)[0][2]) 
        && c == (board.cells)[1][1]
        && c == (board.cells)[2][0]) {
        return c;
    }

    if (board_isFull(board)) {
        return XO_DRAW;
    }
    return ONGOING;
}

/* This function put everything, including a blank */
int32_t board_put(Board *board, struct board_Pos pos, cell_Value val) {
    if (! board_isInRange(pos)) {
        return 1;
    }
    (board -> cells)[pos.row][pos.col] = val;
    return 0;
}

int32_t board_play(Board *board, struct board_Pos pos) {
    if (! (board_isValidMove(*board, pos) && board -> player_turn)) {
        return 1;
    }
    (board -> cells)[pos.row][pos.col] = board -> player_turn;
    board -> player_turn = opponent_of(board -> player_turn);
    return 0;
}

/* Those functions are the same as the correspoding but it returns
 * It won't change board is place, player_turn set to BLANK_CELL on failure */
Board board_putC(Board board, struct board_Pos pos, cell_Value val) {
    if (board_put(&board, pos, val) == 1) {
        board.player_turn = BLANK_CELL;
    }
    return board;
}

Board board_playC(Board board, struct board_Pos pos) {
    if (board_play(&board, pos) == 1) {
        board.player_turn = BLANK_CELL;
    }
    return board;
}

void board_set(Board *board, cell_Value cells[9]) {
    memcpy(board -> cells, cells, 9 * sizeof(cell_Value));
}

/* Get every blank cells position, return number of them */
uint32_t board_getBlankPos(Board board, struct board_Pos blanks[9]) {
    uint32_t top = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((board.cells)[i][j] == BLANK_CELL) {
                blanks[top].row = i;
                blanks[top].col = j;
                top++;
            }
        }
    }
    return top;
}

void board_print(Board board) {
    printf("player_turn: %d\n", board.player_turn);
    for (int32_t i = 0; i < 3; i++) {
        printf("%d %d %d\n", (board.cells)[i][0], (board.cells)[i][1], (board.cells)[i][2]);
    }
}
