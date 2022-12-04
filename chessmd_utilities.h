#ifndef CHESSMD_UTILITIES_H
#define CHESSMD_UTILITIES_H
enum class PCOL { NONE, BLACK, WHITE, last }; //PIECE COLOR ENUM
enum class PTYPE { NONE, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, last }; //PIECE TYPE ENUM

typedef struct Position{
	char x;
	char y;
}Position;


typedef struct Cell {
	Piece* p;
	bool** sel = new bool* [8]; //Cell selection
} Cell;

bool** initBoolMatrix(bool** matrix) {
	if (matrix == nullptr) {
		bool** matrix = new bool* [8];
	}
	for (int i = 0; i < 8; i++) {
		if (matrix[i] == nullptr)
			matrix[i] = new bool[8];
		for (int j = 0; j < 8; j++) {
			matrix[i][j] = false;
		}
	}

	return matrix;
}

bool withinBounds(char x, char y) {
	//create within bounds function
	return false;
}

#endif
