#include "chessmd_utilities.h"
#ifndef CHESSMD_PIECE_H
#define CHESSMD_PIECE_H

class Piece {
public:
	PCOL color;
	PTYPE type;
	Position pos;
	Piece();
	virtual ~Piece() = default;
	bool move_path[8][8];
	bool attack_path[8][8];
	virtual Bool_Matrix Movement(Piece* (&Piece_Matrix)[8][8], Position, bool(*checked)[8]);
	bool isEnemy(Piece* p);
	bool moved = false;
	bool enpassant = false;
	bool castle[2] = { false, false };
	bool checked = false;
	void RemSelfPos(); //removes the attack and move position of self from self
};

typedef Piece* (&Piece_Matrix)[8][8];
void CreatePiece(Piece* &p, PTYPE type = PTYPE::NONE);

class Pawn : public Piece {
public:
	Pawn();
	virtual ~Pawn() = default;
	virtual Bool_Matrix Movement(Piece* (&Piece_Matrix)[8][8], Position, bool(*checked)[8]);
};

class Rook : public Piece {
public:
	Rook();
	virtual ~Rook() = default;
	virtual Bool_Matrix Movement(Piece* (&Piece_Matrix)[8][8], Position, bool(*checked)[8]);
};

class Knight : public Piece {
public:
	Knight();
	virtual ~Knight() = default;
	virtual Bool_Matrix Movement(Piece* (&Piece_Matrix)[8][8], Position, bool(*checked)[8]);
};

class Bishop : public Piece {
public:
	Bishop();
	virtual ~Bishop() = default;
	virtual Bool_Matrix Movement(Piece* (&Piece_Matrix)[8][8], Position, bool(*checked)[8]);
};

class King : public Piece {
public:
	bool checked = false;
	King();
	virtual ~King() = default;
	virtual Bool_Matrix Movement(Piece* (&Piece_Matrix)[8][8], Position, bool(*checked)[8]);
};

class Queen : public Piece {
public:
	Queen();
	virtual ~Queen() = default;
	virtual Bool_Matrix Movement(Piece* (&Piece_Matrix)[8][8], Position, bool(*checked)[8]);
};



#endif