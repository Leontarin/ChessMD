#ifndef CHESSMD_UTILITIES_H
#define CHESSMD_UTILITIES_H
#include <iostream>

enum class PCOL { NONE, BLACK, WHITE, last }; //PIECE COLOR ENUM
enum class PTYPE { NONE, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, last }; //PIECE TYPE ENUM

typedef bool(&Bool_Matrix)[8][8];

typedef struct Position{
	char x;
	char y;
}Position;

void initBoolMatrix(bool (*matrix)[8]);

bool withinBounds(char x, char y);

bool isMoveEmpty(bool (*path)[8]);

std::string GetPCOLStr(PCOL color);

#endif
