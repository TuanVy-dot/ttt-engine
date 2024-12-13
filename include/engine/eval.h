#ifndef EVAL_H

#define EVAL_H

#include <stdbool.h>
#include "engine/board.h"

#define EVAL_MIN -100.0
#define EVAL_MAX 100.0
/* WEIGHTS */
#define CONTROLLED_OPEN_PATH_WEIGHT (EVAL_MAX / 11.0)
#define CONTROLLED_CENTER_WEIGHT (EVAL_MAX / 12.0)
#define CONTROLLED_CORNER_WEIGHT (EVAL_MAX / 12.0)

bool eval_HaveM1(Board board, cell_Value player_turn);
bool eval_HaveM2(Board board, cell_Value player_turn);
int32_t eval_isPotentialWin(cell_Value c0, cell_Value c1, cell_Value c2,
                         cell_Value player_turn);
uint32_t eval_countOpenWinningPath(Board board, cell_Value player_turn);
int32_t eval_MaxOrMin(cell_Value player);
double eval_EvaluateStatic(Board board);
double eval_EvaluateDepth(Board position, uint32_t depth);
struct board_Pos eval_getBestMove(Board position, uint32_t depth);

#endif
