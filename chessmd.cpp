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

bool ChessMD::isNotEmpty(char x, char y) {
	return (board[x][y]) ? true : false;
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
		if (board[1][j])
			board[1][j]->pos = *(new Position({ (char)j,1 }));
		if(board[6][j])
			board[6][j]->pos = *(new Position({ (char)j,6 }));
	}
	//EVERYTHING ELSE using pOrder
	for (int j = 0;j < 8;j++) {
		CreatePiece(board[0][j], pOrder[j]);
		CreatePiece(board[8-1][j], pOrder[8 - j - 1]);
		if (board[0][j])
			board[0][j]->pos = *(new Position({ (char)j,0 }));
		if (board[8-1][j])
			board[8-1][j]->pos = *(new Position({ (char)j,7 }));
	}
	//PIECE COLORS
		//BLACK
		for (int j = 0;j < 8;j++) {
			for (int i = 0;i < 2;i++) { // COLOR
				board[i][j]->color = PCOL::BLACK;
				if (board[i][j])
					board[i][j]->pos = *(new Position({ (char)j,(char)i }));
			}
		}
		//WHITE
		for (int j = 0;j < 8;j++) {
			for (int i=8-2;i<8;i++) {
				board[i][j]->color = PCOL::WHITE;
				if (board[i][j])
					board[i][j]->pos = *(new Position({ (char)j,(char)i }));
			}
		}
}

void ChessMD::initGame(Piece* (&board)[8][8]) {
	_running = true;
	turn = PCOL::WHITE;
	initBoard(board);
}

std::string ChessMD::getLastError() {
	return lastError;
}

Position ChessMD::stringToPosition(std::string str) {
	//returns string's current position, can be negative
	char x = tolower(str[0])-'a';
	char y = 7 - (tolower(str[1])-'1');
	Position pos = { x,y };
	return pos;
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

void ChessMD::addMatrix(bool(*source)[8], bool(*target)[8]) {
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (source[i][j]) {
				target[i][j] = true;
			}
		}
	}
}

void ChessMD::Play(Position source, Position dest) {
	board[dest.y][dest.x] = board[source.y][source.x];
	board[source.y][source.x] = nullptr;
	if (board[dest.y][dest.x]) {
		board[dest.y][dest.x]->pos = dest;
		board[dest.y][dest.x]->moved = true;
	}
	pSel = nullptr;
	turn = (turn == PCOL::WHITE) ? PCOL::BLACK : PCOL::WHITE;
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

void ChessMD::update(std::string event) {
	/*
		Update loop for ChessMD
		Current Updates:
		- Event Input
		- LastError
		- Update all Selectons
	*/
	Position pos;
	Piece* pTmp = nullptr;

	if (!this->lastError.empty())
		this->lastError = "";

	if (parseEvent(event)) {
		pos = stringToPosition(event);
		if(!withinBounds(pos.x, pos.y))
			this->lastError = event + " is not a valid piece.";
		else { //Piece has a pointer
			pTmp = this->board[pos.y][pos.x];
			if (this->pSel != pTmp) { 
				if (this->pSel) {//pSel exists and different targeted
					if (this->pSel->move_path[pos.y][pos.x]) {
						Play(this->pSel->pos, pos);
					}
					else {
						this->pSel = nullptr;
					}
				}
				else if (pTmp) {
					if(pTmp->color == this->turn)
						this->pSel = pTmp;
					else if(pTmp->color != turn){
						this->lastError = "It is currently ";
						this->lastError += (turn == PCOL::WHITE) ? "WHITE" : "BLACK";
						this->lastError += "'s turn.";
					}
				}
			}
			else if ((pTmp == nullptr) || this->pSel == pTmp) {
				this->pSel = nullptr;
			}
		}
	}
	pTmp = nullptr;

	this->winner = this->updateSelection();
	

	if (this->winner != PCOL::NONE) {
		this->_game = false;
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

