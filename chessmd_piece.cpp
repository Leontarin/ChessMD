#include "chessmd_piece.h"
#include "chessmd_utilities.h"
#include <iostream>
Piece::Piece() {
	moved = false;
	color = PCOL::NONE;
	type = PTYPE::NONE;
	initBoolMatrix(move_path);
}

void CreatePiece(Piece* &p, PTYPE type) {
	switch (type) {
	case PTYPE::PAWN:
		p = dynamic_cast<Piece*>(new Pawn);
		break;
	case PTYPE::ROOK:
		p = dynamic_cast<Piece*>(new Rook);
		break;
	case PTYPE::BISHOP:
		p = dynamic_cast<Piece*>(new Bishop);
		break;
	case PTYPE::KNIGHT:
		p = dynamic_cast<Piece*>(new Knight);
		break;
	case PTYPE::QUEEN:
		p = dynamic_cast<Piece*>(new Queen);
		break;
	case PTYPE::KING:
		p = dynamic_cast<Piece*>(new King);
		break;
	case PTYPE::NONE:
		if (p == nullptr)
			p = new Piece();
		break;
	default:
		break;
	}
}

bool Piece::isEnemy(PCOL col) {
	if (this->color == PCOL::NONE)
		return false;
	if ((short)this->color - (short)col != 0)
		return true;
	return false;
}

Bool_Matrix Piece::Movement(Piece* (&board)[8][8], Position pos) {
	initBoolMatrix(this->move_path);

	return this->move_path;
}

/*
bool** Pawn::Movement(Piece** board, Position pos) {
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
*/
Pawn::Pawn() {
	type = PTYPE::PAWN;
	initBoolMatrix(move_path);
};

Bool_Matrix Pawn::Movement(Piece* (&board)[8][8], Position pos) {
	initBoolMatrix(this->move_path);
	short int mod = (this->color == PCOL::BLACK) ? 1 : -1; //Modifier for Black movement
	short int distance = (this->moved) ? 1 : 2;
	char x, y;
	//forward movement
	for (int i = 1;i <= distance;i += 1) {
		x = pos.x;
		y = pos.y + i*mod;
		if (withinBounds(x, y)) {
			if (board[y][x]) {
				if (board[y][x]->color != this->color) {
					this->move_path[y][x] = true;
				}
				break;
			}
			else {
				this->move_path[y][x] = true;
			}
		}
		else {
			break;
		}
	}
	//detect enemy movement
	y = pos.y + mod;
	x = pos.x;
	for (int i = -1;i <= 1;i += 2) {
		if (withinBounds(x+i, y)) {
			if (board[y][x+i]) {
				if (board[y][x+i]->color != this->color) {
					this->move_path[y][x+i] = true;
				}
			}
		}
	}
	return this->move_path;
}

Bool_Matrix Knight::Movement(Piece* (&board)[8][8], Position pos) {
	initBoolMatrix(this->move_path);
	char iY[8] = { -2,-2,-1,-1,1,1,2,2 };
	char iX[8] = { -1,1,-2,2,-2,2,-1,1 };
	char x, y;
	for (int i = 0;i < 8;i++) {
		y = iY[i] + pos.y;
		x = iX[i] + pos.x;
		if (withinBounds(x,y)) {
			if (board[y][x]) {
				if (board[y][x]->color != this->color) {
					this->move_path[y][x] = true;
				}
			}
			else {
				this->move_path[y][x] = true;
			}
		}
	}
	return this->move_path;
}

Rook::Rook() {
	type = PTYPE::ROOK;
	initBoolMatrix(this->move_path);
};

Knight::Knight() {
	type = PTYPE::KNIGHT;
	initBoolMatrix(this->move_path);
};

Bishop::Bishop() {
	type = PTYPE::BISHOP;
	initBoolMatrix(this->move_path);
};

King::King() {
	type = PTYPE::KING;
	initBoolMatrix(this->move_path);
};

Queen::Queen() {
	type = PTYPE::QUEEN;
	initBoolMatrix(this->move_path);
};
