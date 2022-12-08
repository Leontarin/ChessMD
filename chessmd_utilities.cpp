#include "chessmd_piece.h"
#include <iostream>

void initBoolMatrix(bool** matrix) {
	matrix = new bool* [8];
	for (int i = 0;i < 8;i++) {
		matrix[i] = new bool[8];
		for (int j = 0;j < 8;j++) {
			matrix[i][j] = false;
		}
	}
};

bool withinBounds(char x, char y) {
	if (x < 8 && x >= 0 && y >= 0 && y < 8) {
		return true;
	}
	return false;
}