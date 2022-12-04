#ifndef CHESSMD_PIECE_H
#define CHESSMD_PIECE_H
#include "chessmd_utilities.h"

class Piece {
public:
	PCOL color;
	PTYPE type;
	Piece();
	~Piece();
	bool** movePath;
	bool** Movement(Position);
	bool isEnemy(PCOL col);
	bool moved;
};

void CreatePiece(Piece* p, PTYPE type = PTYPE::NONE);

class Pawn : public Piece {
public:
	Pawn();
	~Pawn();
	bool** Movement(Position);
};

class Rook : public Piece {
public:
	Rook();
	~Rook();
};

class Knight : public Piece {
public:
	Knight();
	~Knight();
};

class Bishop : public Piece {
public:
	Bishop();
	~Bishop();
};

class King : public Piece {
public:
	King();
	~King();
};

class Queen : public Piece {
public:
	Queen();
	~Queen();
};



#endif