#include "chessmd.h"
#include "chessmd_piece.h"
#include "chessmd_utilities.h"
#include <iostream> //unneeded, debug

ChessMD::ChessMD() {
	//Initialize Memory for board

	initGame(board);
}

ChessMD::~ChessMD() {

}

bool ChessMD::isNotEmpty(Position pos) {
	return (board[pos.y][pos.x]) ? true : false;
}

void ChessMD::debug() {
	pSel = board[1][0];

	initBoolMatrix(pSel->move_path);
	if (pSel->move_path) {
		for (int i = 2;i < 8;i++) {
			pSel->move_path[i][0] = true;
		}
	}
	std::cout << std::endl;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			std::cout << (pSel->move_path[i][j]) ? 1 : 0;
		}
		std::cout << std::endl;
	}
}

void ChessMD::initBoard(Piece* (&board)[8][8]) {
	//setup Board Values
	initBoardPlacement(board);
	initBoolMatrix(whiteChecked);
	initBoolMatrix(blackChecked);
}

void ChessMD::initBoardPlacement(Piece* (&board)[8][8])  {
	//placement of board pieces and colors.
	PTYPE pOrder[8] = { PTYPE::ROOK,PTYPE::KNIGHT,PTYPE::BISHOP,PTYPE::QUEEN,PTYPE::KING,PTYPE::BISHOP, PTYPE::KNIGHT, PTYPE::ROOK }; //black order
	//PIECE ORDERS
		//PAWNS
	for (int j = 0;j < 8;j++) {
		CreatePiece(board[1][j],PTYPE::PAWN);
		CreatePiece(board[6][j],PTYPE::PAWN);
	}
	//EVERYTHING ELSE using pOrder
	for (int j = 0;j < 8;j++) {
		CreatePiece(board[0][j], pOrder[j]);
		CreatePiece(board[8-1][j], pOrder[8 - j - 1]);
	}
	//PIECE COLORS
		//BLACK
		for (int j = 0;j < 8;j++) {
			for (int i = 0;i < 2;i++) { // COLOR
				board[i][j]->color = PCOL::BLACK;
			}
		}
		//WHITE
		for (int j = 0;j < 8;j++) {
			for (int i=8-2;i<8;i++) {
				board[i][j]->color = PCOL::WHITE;
			}
		}
}

void ChessMD::initGame(Piece* (&board)[8][8]) {
	_running = true;
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
	if (withinBounds((int)(ch0 - 'a'), (int)(ch1-'1'))) {
		return board[(int)(ch1-'1')][(int)(ch0 - 'a')];
	}
	return nullptr;
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

void ChessMD::addMatrix(bool(*source)[8], bool(*target)[8]) {
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (source[i][j]) {
				target[i][j] = true;
			}
		}
	}
}

PCOL ChessMD::updateSelection() {
	/*
	*	Calculate all pieces and during calculation add to total checked board
	*	Also keeps track of kings and calculates loss
	*/
	initBoolMatrix(whiteChecked);
	initBoolMatrix(blackChecked);
	bool(*target)[8] = nullptr;
	Position pos;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board[i][j]) {
				pos.x = j;
				pos.y = i;
				board[i][j]->Movement(board, pos);
				target = (board[i][j]->color == PCOL::WHITE) ? whiteChecked : blackChecked;
				addMatrix(board[i][j]->move_path, target);
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

	std::string event = handleEvent();
	if (parseEvent(event)) {
		pSel = stringToPiece(event);
		if (!isPieceValid(pSel))
			lastError = event + " out of bounds.";
		else { //Piece has a pointer
			boardSel = pSel->move_path;
		}
	}

	if (!lastError.empty())
		lastError = "";

	winner = updateSelection();
	

	if (winner != PCOL::NONE) {
		_game = false;
	}
	else {
		
	}
}

bool ChessMD::getRunning() {
	return _running;
}

Piece_Matrix ChessMD::getBoard() {
	//Read-Only Pointer of the Board
	return board;
}

Piece const* ChessMD::getSelected() {
	//Read-Only Pointer of the Selected Piece
	return pSel;
}

