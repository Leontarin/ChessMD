#include "chessmd_piece.h"
#include "chessmd_utilities.h"
#include <iostream>
Piece::Piece() {
	moved = false;
	color = PCOL::NONE;
	type = PTYPE::NONE;
	pos.x = -1;
	pos.y = -1;
	initBoolMatrix(move_path);
	initBoolMatrix(attack_path);
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

bool Piece::isEnemy(Piece* p) {
	if (p && p->color != this->color) {
		return true;
	}
	return false;
}

Bool_Matrix Piece::Movement(Piece* (&board)[8][8], Position pos, bool(*checked)[8] = nullptr) {
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
	return this->move_path;
}

Pawn::Pawn() {
	type = PTYPE::PAWN;
	initBoolMatrix(move_path);
	initBoolMatrix(attack_path);
};

Bool_Matrix Pawn::Movement(Piece* (&board)[8][8], Position pos, bool(*checked)[8] = nullptr) {
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
	short int mod = (this->color == PCOL::BLACK) ? 1 : -1; //Modifier for Black movement
	short int distance = (this->moved) ? 1 : 2;
	char x, y;
	//forward movement
	for (int i = 1;i <= distance;i += 1) {
		x = pos.x;
		y = pos.y + i*mod;
		if (withinBounds(x, y)) {
			if (!board[y][x]) {
				this->move_path[y][x] = true;
				break;
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
				this->attack_path[y][x + i] = true;
				if (board[y][x+i]->color != this->color) {
					this->move_path[y][x+i] = true;
				}
			}
		}
	}
	return this->move_path;
}

Bool_Matrix Knight::Movement(Piece* (&board)[8][8], Position pos, bool(*checked)[8] = nullptr) {
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
	char iY[8] = { -2,-2,-1,-1,1,1,2,2 };
	char iX[8] = { -1,1,-2,2,-2,2,-1,1 };
	char x, y;
	for (int i = 0;i < 8;i++) {
		y = iY[i] + pos.y;
		x = iX[i] + pos.x;
		if (withinBounds(x,y)) {
			if (board[y][x]) {
				this->attack_path[y][x] = true;
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

Bool_Matrix Rook::Movement(Piece* (&board)[8][8], Position pos, bool(*checked)[8] = nullptr) {
	initBoolMatrix(this->move_path);
	
	for (int mod = -1; mod <= 1; mod += 2) { //-1, 1
		for (int i = pos.y + mod; i < 8 && i>= 0; i += mod) {
			if (board[i][pos.x]) {
				if (board[i][pos.x]->color != this->color)
					this->move_path[i][pos.x] = true;
				break;
			}
			else {
				this->move_path[i][pos.x] = true;
			}
		}
		for (int i = pos.x + mod; i < 8 && i >= 0; i += mod) {
			if (board[pos.y][i]) {
				this->attack_path[pos.y][i] = true;
				if (board[pos.y][i]->color != this->color)
					this->move_path[pos.y][i] = true;
				break;
			}
			else {
				this->move_path[pos.y][i] = true;
			}
		}
	}

	return this->move_path;
}

Bool_Matrix Bishop::Movement(Piece* (&board)[8][8], Position pos, bool(*checked)[8] = nullptr) {
	initBoolMatrix(this->move_path);
	int i, j;
	for (int modY = -1; modY <= 1; modY += 2) { //-1, 1
		for (int modX = -1; modX <= 1; modX += 2) { //-1, 1
			for (i = pos.y + modY, j = pos.x + modX; withinBounds(j, i); i += modY, j += modX) {
				if (board[i][j]) {
					if (board[i][j]->color != this->color)
						this->move_path[i][j] = true;
					break;
				}
				else {
					this->move_path[i][j] = true;
				}
			}
		}
	}
	return this->move_path;
}

Bool_Matrix King::Movement(Piece* (&board)[8][8], Position pos, bool(*checked)[8] = nullptr) {
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
	for (int i = pos.y-1; i < pos.y+2; i++) {
		for (int j = pos.x-1; j < pos.x+2; j++) {
			if (withinBounds(j, i)) {
				if (board[i][j] && !checked[i][j]) {
					this->attack_path[i][j] = true;
					if (isEnemy(board[i][j])) {
						this->move_path[i][j] = true;
					}
					break;
				}
				else {
					if(!checked[i][j])
						this->move_path[i][j] = true;
				}
			}
		}
	}
	return this->move_path;
}

Bool_Matrix Queen::Movement(Piece* (&board)[8][8], Position pos, bool(*checked)[8] = nullptr) {
	initBoolMatrix(this->move_path);
	int i, j;
	for (int modY = -1; modY <= 1; modY++) { //-1, 0,1
		for (int modX = -1; modX <= 1; modX++) { //-1, 0,1
			for (i = pos.y + modY, j = pos.x + modX; withinBounds(j, i); i += modY, j += modX) {
				if (board[i][j]) {
					this->attack_path[i][j] = true;
					if (board[i][j]->color != this->color)
						this->move_path[i][j] = true;
					break;
				}
				else {
					this->move_path[i][j] = true;
				}
			}
		}
	}
	return this->move_path;
}

Rook::Rook() {
	type = PTYPE::ROOK;
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
};

Knight::Knight() {
	type = PTYPE::KNIGHT;
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
};

Bishop::Bishop() {
	type = PTYPE::BISHOP;
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
};

King::King() {
	type = PTYPE::KING;
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
};

Queen::Queen() {
	type = PTYPE::QUEEN;
	initBoolMatrix(this->move_path);
	initBoolMatrix(this->attack_path);
};