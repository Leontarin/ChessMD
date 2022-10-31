#include "chessmd.h"
#include <map>


ChessMD::ChessMD() {
	//Initialize Memory for board
	for (int i = 0;i < 8;i++) {
		board[i] = new Cell[8];
	}
	initGame(board);
}

ChessMD::~ChessMD() {
	//Free allocated memory
}

void ChessMD::initBoard(Cell** board) {
	//Initialize everything to 0
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			board[i][j].p.color = PCOL::NONE;
			board[i][j].p.type = PTYPE::NONE;
			board[i][j].mov = false;
			board[i][j].sel = false;
		}
	}

	//Set up PAWNS
	for (short int line[2] = { 1,6 }, i = 0; i < 2;i++) {
		for (short int j = 0;j < 8;j++) {
			board[line[i]][j].p.type = PTYPE::PAWN;
		}
	}

	//Set up PIECES
	//BLACK SIDE
	int p = (int)PTYPE::ROOK;
	for (int i = 0;i < 8;i++) {
		while (p == (int)PTYPE::NONE || p == (int)PTYPE::PAWN || p == (int)PTYPE::last) { //filter through enum types
			p = (p+1)% (int)PTYPE::last; //loops through the types
		}
		board[8-1][i].p.type = (PTYPE)p;
	}
	//WHITE SIDE
	p = (int)PTYPE::ROOK;
	for (int i = 8-1;i <= 0;i--) {
		while (p == (int)PTYPE::NONE || p == (int)PTYPE::PAWN || p == (int)PTYPE::last) { //filter through enum types
			p = (p + 1) % (int)PTYPE::last; //loops through the types
		}
		board[8-1][i].p.type = (PTYPE)p;
	}

	//Set up COLORS
	for (int i = 0;i < 2;i++) { //BLACK
		for (short int j = 0;j < 8;j++) {
			board[i][j].p.color = PCOL::BLACK;
		}
	}
	for (int i = 8;i <= 8-1;i--) { //WHITE
		for (int j = 0;j < 8;j++) {
			board[i][j].p.color = PCOL::WHITE;
		}
	}
	
}

void ChessMD::initGame(Cell** board) {
	_running = true;
	initBoard(board);
}

void ChessMD::update(ChessMD* game) {
	_running = false;
}

bool ChessMD::getRunning() {
	return _running;
}
ChessMD::Cell const* const* ChessMD::getBoard() {
	return board;
}

