#ifndef EVAL_H

#define EVAL_H

#include <stdbool.h>
#include "engine/board.h"

#define EVAL_MIN -100
#define EVAL_MAX 100
/* WEIGHTS */
#define OPPONENT_POTENTIAL_WIN_WEIGHT 75

bool eval_HaveM1(Board board, cell_Value player_turn);
bool eval_isPotentialWin(cell_Value c0, cell_Value c1, cell_Value c2,
                         cell_Value player_turn);

#endif
