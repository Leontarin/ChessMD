#include "chessmd_utilities.h"
#ifndef CHESSMD_PIECE_H
#define CHESSMD_PIECE_H
class Piece {
public:
	PCOL color;
	PTYPE type;
	Piece();
	virtual ~Piece();
	bool** move_path;
	bool** Movement(Piece **board, Position);
	bool isEnemy(PCOL col);
	bool moved;
};

void CreatePiece(Piece* p, PTYPE type = PTYPE::NONE);

class Pawn : public Piece {
public:
	Pawn();
	virtual ~Pawn();
	bool** Movement(Position);
};

class Rook : public Piece {
public:
	Rook();
	virtual ~Rook();
};

class Knight : public Piece {
public:
	Knight();
	virtual ~Knight();
};

class Bishop : public Piece {
public:
	Bishop();
	virtual ~Bishop();
};

class King : public Piece {
public:
	King();
	virtual ~King();
};

class Queen : public Piece {
public:
	Queen();
	virtual ~Queen();
};



#endif