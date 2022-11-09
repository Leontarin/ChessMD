#include "chessmd.h"
#include <map>
#include <string>

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
			board[i][j].p.type = (PTYPE)0;
			board[i][j].mov = false;
			board[i][j].sel = false;
		}
	}

	//setup Board Values
	initBoardPlacement(board);
	
}

void ChessMD::initBoardPlacement(Cell** board) {
	//placement of board pieces and colors.
	PTYPE pOrder[8] = { PTYPE::ROOK,PTYPE::KNIGHT,PTYPE::BISHOP,PTYPE::QUEEN,PTYPE::KING,PTYPE::BISHOP, PTYPE::KNIGHT, PTYPE::ROOK }; //black order
	//PIECE COLORS
		//BLACK
		for (int j = 0;j < 8;j++) {
			for (int i = 0;i < 2;i++) { // COLOR
				board[j][i].p.color = PCOL::BLACK;
			}
		}
		//WHITE
		for (int j = 0;j < 8;j++) {
			for (int i=8-2;i<8;i++) {
				board[j][i].p.color = PCOL::WHITE;
			}
		}
	//PIECE ORDERS
		//PAWNS
		for (int j = 0;j < 8;j++) {
			board[j][1].p.type =			PTYPE::PAWN; //black
			board[j][6].p.type = PTYPE::PAWN; //white
		}
		//EVERYTHING ELSE using pOrder
		for (int j = 0;j < 8;j++) {
			board[j][0].p.type = pOrder[j];
			board[j][8 - 1].p.type = pOrder[8 -j-1];
		}
}

void ChessMD::initGame(Cell** board) {
	_running = true;
	initBoard(board);
}

std::string ChessMD::handleEvent() {
	std::string input;
	std::cin >> input;
	return input;
}

void ChessMD::update(ChessMD* game) {
	std::string event = handleEvent();
	if (event == "quit" || event == "exit") {
		_running = false;
	}
}

bool ChessMD::getRunning() {
	return _running;
}

ChessMD::Cell const* const* ChessMD::getBoard() {
	return board;
}

