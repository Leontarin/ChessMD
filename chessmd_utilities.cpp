#include "chessmd_utilities.h"
#include "chessmd_piece.h"



bool** initBoolMatrix(bool** matrix) {
	if (matrix == nullptr) {
		bool** matrix = new bool* [8];
	}
	for (int i = 0; i < 8; i++) {
		if (matrix[i] = nullptr)
			matrix[i] = new bool[8];
		for (int j = 0; j < 8; j++) {
			matrix[i][j] = false;
		}
	}

	return matrix;
}