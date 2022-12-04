#ifndef CHESSMD_UTILITIES_H
#define CHESSMD_UTILITIES_H

enum class PCOL { NONE, BLACK, WHITE, last }; //PIECE COLOR ENUM
enum class PTYPE { NONE, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, last }; //PIECE TYPE ENUM

typedef struct Position{
	char x;
	char y;
}Position;


class Cell {
public:
	Piece* p;
	bool** sel = new bool* [8]; //Cell selection
};

bool** initBoolMatrix(bool** matrix);

bool withinBounds(char x, char y) {
	//create within bounds function
	return false;
}

#endif
