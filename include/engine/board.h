#ifndef BOARD

#define BOARD

#include <stdint.h>
#include <stdbool.h>

/* The opponent_of operator return opponent of a player
 * Should not be use for other values beyond 1 and 2 */
#define opponent_of(p) p ^ 3

enum cell_Value {
    BLANK_CELL, X_PLAYER, O_PLAYER
};
typedef enum cell_Value cell_Value;
enum board_Status {
    ONGOING = BLANK_CELL,
    X_WIN = X_PLAYER,
    O_WIN = O_PLAYER,
    XO_DRAW
    // It would be much easier if they match, right?
};
typedef enum board_Status board_Status;

struct Board {
    cell_Value cells[3][3];
    cell_Value player_turn; 
    /* BLANK_CELL is allowed,
     * but the evaluation functions will simply return -1 */
};
typedef struct Board Board;

/* To be able to use row and column as a single variable
 * Which is more manageable, who don't hate 100000 arguement functions? */
struct board_Pos {
    uint8_t row;
    uint8_t col;
};
/* To use as function arguements */
#define POS(i, j) (struct board_Pos){.row = i, .col = j}

/* Initiallize board, after that it is ready to be used */
void board_init(Board *board);
/* Check if a board is valid,
 * that is cells and player_turn are all in ternary (0, 1, 2)
 * O might move first or O already moves 5 turns before X,
 * It wouldn't matter, this function check if board data is in ternary only
 * Ensure the validness of the position, not if it follow the game rule */
bool board_isValid(Board board);
/* Check if a move is valid by range */
bool board_isInRange(struct board_Pos pos);
/* Check if a move is input base on the game rule (no overlap) */
bool board_isValidMove(Board board, struct board_Pos pos);
bool board_isFull(Board board);
board_Status board_checkWin(Board board);
/* This function put everything, including a blank, 
 * return 0 for success and 1 for failure */
int32_t board_put(Board *board, struct board_Pos pos, cell_Value val);
/* This function put based on the player_turn field
 * if it is BLANK_CELL, nothing will be played
 * It update player_turn to the next mark X -> O and O -> X */
int32_t board_play(Board *board, struct board_Pos pos);
/* The same but return Board instead of change in place
 * player_turn will be set to BLANK_CELL on failure */
Board board_putC(Board board, struct board_Pos pos, cell_Value val);
Board board_playC(Board board, struct board_Pos pos);
/* Copy cells into board.cells */
void board_set(Board *board, cell_Value cells[9]);

#endif
