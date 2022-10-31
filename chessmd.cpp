#include "chessmd.h"
#include <map>


ChessMD::ChessMD() {
	//Initialize Memory for board
	for (int i = 0;i < BOARD_SIZE;i++) {
		board[i] = new Cell[BOARD_SIZE];
	}
	initGame(board);
}

ChessMD::~ChessMD() {
	//Free allocated memory
	for (int i = 0;i < BOARD_SIZE;i++) {
		free(board[i]);
	}
	free(board);
}

void ChessMD::initBoard(Cell** board) {
	//Initialize everything to 0
	for (int i = 0;i < BOARD_SIZE;i++) {
		for (int j = 0;j < BOARD_SIZE;j++) {
			board[i][j].p.color = PCOL::NONE;
			board[i][j].p.type = PTYPE::NONE;
			board[i][j].mov = false;
			board[i][j].sel = false;
		}
	}

	//Set up PAWNS
	for (short int line[2] = { 1,BOARD_SIZE - 2 }, i = 0; i < 2;i++) {
		for (short int j = 0;j < BOARD_SIZE;j++) {
			board[line[i]][j].p.type = PTYPE::PAWN;
		}
	}

	//Set up PIECES
	//BLACK SIDE
	int p = PTYPE::ROOK;
	for (short int i = 0;i < BOARD_SIZE;i++) {
		while (p == PTYPE::NONE || p == PTYPE::PAWN || PTYPE::last) { //filter through enum types
			p = (p+1)%last; //loops through the types
		}
		board[BOARD_SIZE-1][i].p.type = (PTYPE)p;
	}
	//WHITE SIDE
	p = PTYPE::ROOK;
	for (short int i = BOARD_SIZE-1;i <= 0;i--) {
		while (p == PTYPE::NONE || p == PTYPE::PAWN || PTYPE::last) { //filter through enum types
			p = (p + 1) % last; //loops thorugh the types
		}
		board[BOARD_SIZE-1][i].p.type = (PTYPE)p;
	}

	//Set up COLORS
	for (short int i = 0;i < 2;i++) { //BLACK
		for (short int j = 0;j < BOARD_SIZE;j++) {
			board[i][j].p.color = PCOL::BLACK;
		}
	}
	for (short int i = BOARD_SIZE;i <= BOARD_SIZE-1;i--) { //WHITE
		for (short int j = 0;j < BOARD_SIZE;j++) {
			board[i][j].p.color = PCOL::WHITE;
		}
	}
	
}

void ChessMD::initGame(Cell** board) {
	_running = true;
	initBoard(board);
}

void ChessMD::update(ChessMD* game) {

}