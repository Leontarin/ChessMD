#include "chessmd_piece.h"
#include "chessmd_utilities.h"
#include "chessmd.h"

Piece::Piece() {
	moved = false;
	color = PCOL::NONE;
	type = PTYPE::NONE;
	initBoolMatrix(movePath);
}

bool Piece::isEnemy(PCOL col) {
	if (this->color == PCOL::NONE)
		return false;
	if ((short)this->color - (short)col != 0)
		return true;
	return false;
}

bool** Piece::Movement(Cell** board, Position pos) {
	bool** selection = initBoolMatrix(selection);

	return selection;
}

bool** Pawn::Movement(Cell** board, Position pos) {
	int mod = (board[pos.x][pos.y].p->color == PCOL::BLACK) ? -1 : 1; //modifier for Black side (reversed) 
	short int x, y;
	x = pos.x;
	char move_mod = moved ? 1 : 2;
	for (int i = 0; i < move_mod; i++) {
		y = pos.y + (i * mod);
		if (y >= 8 || y < 0)
			break;
		else if (board[x][y].p->color == PCOL::NONE) { //set available path
			movePath[x][y] = true;
		}
	}
	if (withinBounds(x + 1, y + mod)) //X,Y within bounds
		movePath[x + 1][y + mod] = (isEnemy(board[x + 1][y + mod].p->color)) ? true : false;
	return movePath;
};

Pawn::Pawn() {
	type = PTYPE::PAWN;
};

Rook::Rook() {
	type = PTYPE::ROOK;
};

Knight::Knight() {
	type = PTYPE::KNIGHT;
};

Bishop::Bishop() {
	type = PTYPE::BISHOP;
};

King::King() {
	type = PTYPE::KING;
};

Queen::Queen() {
	type = PTYPE::QUEEN;
};