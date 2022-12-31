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
		if (board[0][j]) {
			board[0][j]->pos = *(new Position({ (char)j,0 }));
			if (board[0][j]->type == PTYPE::KING) {
				king_black = board[0][j];
			}
		}
		if (board[8 - 1][j]) {
			board[8 - 1][j]->pos = *(new Position({ (char)j,7 }));
			if (board[8-1][j]->type == PTYPE::KING) {
				king_white = board[8-1][j];
			}
		}
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

bool ChessMD::isPlayValid(Position source, Position dest) {
	/*
		Play is valid if:
			1. current team's king is not checked.
			2. the target is not a king.
			3. if checked, the king must move to an unchecked spot.
	*/
	Piece* king = nullptr;

	if (board[source.y][source.x]) {//get current teams color and validate check
		if (board[source.y][source.x]->color == PCOL::WHITE) {
			king = king_white;
		}
		else {
			king = king_black;
		}

		if (king == getChecked() && king != board[source.y][source.x]) {
			lastError = GetPCOLStr(king->color);
			lastError += "'s king is checked";
			return false;
		}
	}

	if (board[dest.y][dest.x]) { //gets target
		if (board[dest.y][dest.x]->type == PTYPE::KING) { //target is not a king
			return false;
		}
	}

	return true;
}

//Plays source piece to dest piece and updates if moved accordingly
void ChessMD::Play(Position source, Position dest) {
	board[dest.y][dest.x] = board[source.y][source.x];
	board[source.y][source.x] = nullptr;
	if (board[dest.y][dest.x]) {
		board[dest.y][dest.x]->pos = dest;
		//Case: Pawn moved 2 && update enpassant
		if (board[dest.y][dest.x]->type == PTYPE::PAWN) {
			if (abs(dest.y - source.y) == 2)
				board[dest.y][dest.x]->enpassant = true;
		}
		//Everything else
		board[dest.y][dest.x]->moved = true;
	}
	pSel = nullptr;
	turn = (turn == PCOL::WHITE) ? PCOL::BLACK : PCOL::WHITE;
}

/*
*	Calculate all pieces and during calculation add to total checked board
*	Also keeps track of kings and calculates loss
*/
PCOL ChessMD::updateSelection() {

	initBoolMatrix(whiteChecked);
	initBoolMatrix(blackChecked);
	bool(*ally_checked)[8] = nullptr;
	bool(*enemy_checked)[8] = nullptr;
	Position pos;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board[i][j] && board[i][j]->type != PTYPE::KING) {
				pos.x = j;
				pos.y = i;
				board[i][j]->Movement(board, pos, nullptr);
				ally_checked = (board[i][j]->color == PCOL::WHITE) ? whiteChecked : blackChecked;
				addMatrix(board[i][j]->move_path, ally_checked);
				addMatrix(board[i][j]->attack_path, ally_checked);
			}
		}
	}

	//reupdates kings for checked movement
	king_black->Movement(board, king_black->pos, whiteChecked);
	king_white->Movement(board, king_white->pos, blackChecked);

	//checks for checkmate / check
	if (whiteChecked[king_black->pos.y][king_black->pos.x]) {
		checked = PCOL::BLACK;
		if (isMoveEmpty(king_black->move_path) && isMoveEmpty(king_black->attack_path))
			return PCOL::WHITE;
	}
	if (blackChecked[king_white->pos.y][king_white->pos.x]) {
		checked = PCOL::WHITE;
		if (isMoveEmpty(king_white->move_path) && isMoveEmpty(king_white->attack_path))
			return PCOL::BLACK;
	}

	return PCOL::NONE;
}

/*
	Update loop for ChessMD
	Current Updates:
	- Event Input
	- LastError
	- Update all Selectons
*/
void ChessMD::update(std::string event) {

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
						if (isPlayValid(this->pSel->pos, pos)) {
							Play(this->pSel->pos, pos);
						}
					}
					else {
						this->pSel = nullptr;
					}
				}
				else if (pTmp) {
					if(pTmp->color == this->turn)
						this->pSel = pTmp;
					else if(pTmp->color != turn) {
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
}

bool ChessMD::getRunning() {
	return _running;
}

//Read-Only Pointer of the Board
Piece_Matrix ChessMD::getBoard() {
	return board;
}

PCOL ChessMD::getWinner() {
	return winner;
}

Piece* ChessMD::getChecked() {
	if (checked == PCOL::NONE) {
		return nullptr;
	}
	else {
		if (checked == PCOL::WHITE)
			return king_white;
		else {
			return king_black;
		}
	}
}

//Read-Only Pointer of the Selected Piece
Piece const* ChessMD::getSelected() {
	return pSel;
}

