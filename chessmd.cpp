#include "chessmd.h"
#include <map>
#include <string>

ChessMD::ChessMD() {
	//Initialize Memory for board
	board = new Cell * [8];
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
			board[i][j].p = nullptr;
		}
	}
	
	//setup Board Values
	initBoardPlacement(board);
	
}

void ChessMD::initBoardPlacement(Cell** board)  {
	//placement of board pieces and colors.
	PTYPE pOrder[8] = { PTYPE::ROOK,PTYPE::KNIGHT,PTYPE::BISHOP,PTYPE::QUEEN,PTYPE::KING,PTYPE::BISHOP, PTYPE::KNIGHT, PTYPE::ROOK }; //black order
	//PIECE ORDERS
		//PAWNS
	for (int j = 0;j < 8;j++) {
		//board[j][1].p = new Pawn;
		//board[j][6].p = new Pawn;
		//temp test for regular Piece
		CreatePiece(board[j][1].p);
		CreatePiece(board[j][6].p);
	}
	//EVERYTHING ELSE using pOrder
	for (int j = 0;j < 8;j++) {
		//createPiece(board[j][0].p, pOrder[j]);
		//createPiece(board[j][8 - 1].p, pOrder[8 - j - 1]);
		//temp test for regular Piece
		CreatePiece(board[j][0].p);
		CreatePiece(board[j][8 - 1].p);
	}
	//PIECE COLORS
		//BLACK
		for (int j = 0;j < 8;j++) {
			for (int i = 0;i < 2;i++) { // COLOR
				board[j][i].p->color = PCOL::BLACK;
			}
		}
		//WHITE
		for (int j = 0;j < 8;j++) {
			for (int i=8-2;i<8;i++) {
				board[j][i].p->color = PCOL::WHITE;
			}
		}
}

void ChessMD::initGame(Cell** board) {
	_running = true;
	cSel = nullptr;
	initBoard(board);
}

std::string ChessMD::getLastError() {
	return lastError;
}

std::string ChessMD::handleEvent() {
	/*
		Event currently handled by manual commands
		Will change in the future to Mouse Events
	*/
	std::string input;
	std::cin >> input;
	return input;
}

Cell* ChessMD::stringToCell(std::string str) {
	char ch0 = tolower(str[0]);
	char ch1 = tolower(str[1]);
	//Gets string of cell and returns the cells' pointer if valid.
	if ((ch0 < 'a' || ch0> 'h') || (ch1 < '1' || ch1 > '8'))
		return nullptr;
	return (Cell*)&board[str[0] - 'a'][str[1]];
}

bool ChessMD::parseEvent(std::string event) {
	if (event == "quit" || event == "exit") {
		_running = false;
		return 0;
	}
	if (event.length() != 2) {
		lastError = "Invalid Command, pick cell 'e.g. a1' or quit/exit";
		return 0;
	}
	return 1;
}

bool ChessMD::isCellValid(Cell* c) {
	//Currently checks if cell is nullptr (out of bounds)
	if (c == nullptr) {
		return 0;
	}
	return 1;
}

PCOL ChessMD::updateSelection() {

}

void ChessMD::update(ChessMD* game) {
	/*
		Update loop for ChessMD
		Current Updates:
		- LastError
		- Event (Input)
	*/
	if (!lastError.empty())
		lastError = "";

	winner = updateSelection();
	if (winner != PCOL::NONE) {
		_game = false;
	}

	std::string event = handleEvent();
	if (parseEvent(event)) {
		cSel = stringToCell(event);
		if (!isCellValid(cSel))
			lastError = event + " out of bounds.";
		else { //cell has a pointer
		}
	}
}

bool ChessMD::getRunning() {
	return _running;
}

Cell const* const* ChessMD::getBoard() {
	//Read-Only Pointer of the Board
	return board;
}

