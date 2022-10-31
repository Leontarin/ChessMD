#include <iostream>
#ifndef CHESSMD_H
#define CHESSMD_H
#define BOARD_SIZE 8


class ChessMD {
private:

	enum class PCOL { NONE, BLACK, WHITE, last }; //PIECE COLOR ENUM
	enum class PTYPE { NONE, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, last }; //PIECE TYPE ENUM
	bool _running;
	short int turn;
	typedef struct Piece {
		PTYPE type;
		PCOL color;
	} Piece;
	typedef struct Cell {
		struct Piece p;
		bool sel;
		bool mov;
	} Cell;
	Cell** board = new Cell * [BOARD_SIZE];
	void initBoard(Cell**);
	void initGame(Cell**);
public:
	ChessMD();
	~ChessMD();
	bool getRunning();
	void update(ChessMD* game);
	const Cell** getBoard();
};

#endif
