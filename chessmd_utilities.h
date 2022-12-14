#include <iostream>
#ifndef CHESSMD_UTILITIES_H
#define CHESSMD_UTILITIES_H

enum class PCOL { NONE, BLACK, WHITE, last }; //PIECE COLOR ENUM
enum class PTYPE { NONE, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, last }; //PIECE TYPE ENUM

typedef bool(&Bool_Matrix)[8][8];

typedef struct Position{
	char x;
	char y;
}Position;

void initBoolMatrix(bool (*matrix)[8]);

bool withinBounds(char x, char y);

#endif
