#include "chessmd_piece.h"
#include <iostream>

void initBoolMatrix(bool** matrix) {
	bool** new_matrix;

};

bool withinBounds(char x, char y) {
	if (x < 8 && x >= 0 && y >= 0 && y < 8) {
		return true;
	}
	return false;
}