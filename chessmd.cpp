#include "chessmd.h"

ChessMD::ChessMD() {
	//Initialize Memory for board
	board = new Piece * [8];
	for (int i = 0;i < 8;i++) {
		board[i] = new Piece[8];
	}
	
	initGame(board);
}

ChessMD::~ChessMD() {

}

void ChessMD::initBoard(Piece** board) {
	//setup Board Values
	initBoardPlacement(board);
	
}

void ChessMD::initBoardPlacement(Piece** board)  {
	//placement of board pieces and colors.
	PTYPE pOrder[8] = { PTYPE::ROOK,PTYPE::KNIGHT,PTYPE::BISHOP,PTYPE::QUEEN,PTYPE::KING,PTYPE::BISHOP, PTYPE::KNIGHT, PTYPE::ROOK }; //black order
	//PIECE ORDERS
		//PAWNS
	for (int j = 0;j < 8;j++) {
		CreatePiece(&board[j][1],PTYPE::PAWN);
		CreatePiece(&board[j][6],PTYPE::PAWN);
	}
	//EVERYTHING ELSE using pOrder
	for (int j = 0;j < 8;j++) {
		CreatePiece(&board[j][0], pOrder[j]);
		CreatePiece(&board[j][8 - 1], pOrder[8 - j - 1]);
	}
	//PIECE COLORS
		//BLACK
		for (int j = 0;j < 8;j++) {
			for (int i = 0;i < 2;i++) { // COLOR
				board[j][i].color = PCOL::BLACK;
			}
		}
		//WHITE
		for (int j = 0;j < 8;j++) {
			for (int i=8-2;i<8;i++) {
				board[j][i].color = PCOL::WHITE;
			}
		}
}

void ChessMD::initGame(Piece** board) {
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

Piece* ChessMD::stringToPiece(std::string str) {
	char ch0 = tolower(str[0]);
	char ch1 = tolower(str[1]);
	//Gets string of Piece and returns the Pieces' pointer if valid.
	if ((ch0 < 'a' || ch0> 'h') || (ch1 < '1' || ch1 > '8'))
		return nullptr;
	return (Piece*)&board[str[0] - 'a'][str[1]];
}

bool ChessMD::parseEvent(std::string event) {
	if (event == "quit" || event == "exit") {
		_running = false;
		return 0;
	}
	if (event.length() != 2) {
		lastError = "Invalid Command, pick Piece 'e.g. a1' or quit/exit";
		return 0;
	}
	return 1;
}

bool ChessMD::isPieceValid(Piece* c) {
	//Currently checks if Piece is nullptr (out of bounds)
	if (c == nullptr) {
		return 0;
	}
	return 1;
}

PCOL ChessMD::updateSelection() {
	Position pos;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board[i][j].type != PTYPE::NONE) {
				pos.x = i, pos.y = j;
				board[i][j].move_path = board[i][j].Movement(pos);
			}
		}
	}
	return PCOL::NONE;
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
		cSel = stringToPiece(event);
		if (!isPieceValid(cSel))
			lastError = event + " out of bounds.";
		else { //Piece has a pointer
		}
	}
}

bool ChessMD::getRunning() {
	return _running;
}

Piece const* const* ChessMD::getBoard() {
	//Read-Only Pointer of the Board
	return board;
}

