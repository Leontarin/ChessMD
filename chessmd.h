#include <iostream>
#ifndef CHESSMD_H
#define CHESSMD_H
#define BOARD_SIZE 3

enum class PCOL { NONE, BLACK, WHITE, last }; //PIECE COLOR ENUM
enum class PTYPE { NONE, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, last }; //PIECE TYPE ENUM

class ChessMD {
private:
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
	Cell** board = new Cell * [8];
	void initBoard(Cell**);
	void initGame(Cell**);
	void initBoardPlacement(Cell**);
	std::string handleEvent();

public:
	ChessMD();
	~ChessMD();
	bool getRunning();
	void update(ChessMD* game);
	Cell const* const* getBoard();
};

#endif
