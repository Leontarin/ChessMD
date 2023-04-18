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
		CreatePiece(board[8-1][j], pOrder[j]);
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
	_played = true;
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

//Smaller version of Play thats used for SimulatePlay()
void ChessMD::FakePlay(Position source, Position dest, bool reverse) {
	//check for pawn plays
	if (board[source.y][source.x]->type == PTYPE::PAWN) {
		if (CheckEnPassantPlay(source, dest)) //check play of enpassant (the attack dest is empty)
		{
			int mod = (board[source.y][source.x]->color == PCOL::BLACK) ? 1 : -1; //get direction of enpassant
			if (!reverse) {
				if (board[dest.y - mod][dest.x]) {
					board[dest.y - mod][dest.x] = nullptr;
				}
			}
		}
	}
	board[dest.y][dest.x] = board[source.y][source.x];
	board[source.y][source.x] = nullptr;
	if (board[dest.y][dest.x]) { //play
		board[dest.y][dest.x]->pos = dest;
	}
};

//Simulates a Play and returns if its possible or not (king is checked)
bool ChessMD::SimulatePlay(Position source, Position dest) {
	int mod_passant;
	bool valid_move = true;
	//Pawn Play before Pawn
	Piece* tmp = nullptr, *tmp_passant = nullptr; //temporary pawn pointer (enpassant play)
	bool tmp_white_chk = king_white->checked;
	bool tmp_black_chk = king_black->checked;

	if (CheckEnPassantPlay(source, dest)) {
		mod_passant = (board[source.y][source.x]->color == PCOL::BLACK) ? 1 : -1;
		tmp_passant = board[source.y - mod_passant][source.x]; //fix this (source)
	}

	tmp = board[dest.y][dest.x]; //backup destination for restoration
	FakePlay(source, dest);
	updateSelection();
	//Check Checking
	if (tmp_black_chk && king_black->checked && king_black->color == turn) {
		valid_move = false;
	}
	if (tmp_white_chk && king_white->checked && king_white->color == turn) {
		valid_move = false;
	}
	//return to original
	FakePlay(dest, source, true); //switch this with custom Play to not update EnPassant and movement
	if (tmp) board[dest.y][dest.x] = tmp;
	updateSelection();
	return valid_move;
}

bool ChessMD::isPlayValid(Position source, Position dest) {
	/*
		Play is valid if:
			1. current team's king is not checked.
			2. the target is not a king.
			3. if checked, the king must move to an unchecked spot.
	*/
	Piece* king = nullptr;
	bool valid_move;
	if (board[source.y][source.x]) {//get current teams color and validate check
		if (board[source.y][source.x]->color == PCOL::WHITE) {
			king = king_white;
		}
		else {
			king = king_black;
		}

		valid_move = SimulatePlay(source, dest);

		if (!valid_move) {
			lastError = GetPCOLStr(king->color);
			lastError += "'s king is/will be checked";
			return false;
		}
	}

	if (board[dest.y][dest.x]) { //gets target
		if (board[dest.y][dest.x]->type == PTYPE::KING) { //target should not be a king
			return false;
		}
	}

	return true;
}

bool ChessMD::CheckEnPassantPlay(Position source, Position dest) {
	if (board[source.y][source.x] && board[source.y][source.x]->type == PTYPE::PAWN) //if pawn
		if (!board[dest.y][dest.x] && dest.x != source.x) //check play of enpassant (the attack dest is empty)
			return true;
	return false;
}

void ChessMD::UpdateEnPassant() {
	//process self enpassant
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] && board[i][j]->type == PTYPE::PAWN)
				if (board[i][j]->enpassant)
					board[i][j]->enpassant = false;
		}
	}
}

//Plays source piece to dest piece and updates if moved accordingly
void ChessMD::Play(Position source, Position dest) {
	Piece* king_p = (board[source.y][source.x]->color == PCOL::BLACK) ? king_black : king_white; //king 
	int mod;
	//check and update pawn plays (EnPassant, Promotion)
	if (board[source.y][source.x]->type == PTYPE::PAWN) {
		//Enpassant
		if (abs(dest.y - source.y) == 2) //enable enpassant if moved double
			board[source.y][source.x]->enpassant = true;
		if (CheckEnPassantPlay(source, dest)) //check play of enpassant (the attack dest is empty)
		{
			mod = (board[source.y][source.x]->color == PCOL::BLACK) ? 1 : -1; //get direction of enpassant
			if (board[dest.y - mod][dest.x]) {
				board[dest.y - mod][dest.x] = nullptr;
			}
		}
		//Promotion
		mod = (board[source.y][source.x]->color == PCOL::BLACK) ? 7 : 0;
		if(dest.y == mod){
			promotion = true;
			promoted = board[source.y][source.x];
		}
	}
	//checkand update castling for Rook plays
	if (board[source.y][source.x]->type == PTYPE::ROOK && !board[source.y][source.x]->moved) {
		if (source.x == 0) 
			king_p->castle[1] = false;
		else
			king_p->castle[0] = false;
	}//long castle[1] , short castle[0] if black castle[1-mod]
	//update king plays (Castling)
	if (board[source.y][source.x]->type == PTYPE::KING && board[source.y][source.x]->castle[0] || board[source.y][source.x]->castle[1]) {
		//update castle
		board[source.y][source.x]->castle[0] = false;
		board[source.y][source.x]->castle[1] = false;
		//short castle
		if ((source.x - dest.x) == -2) {
			//Move relevant rook
			board[source.y][dest.x - 1] = board[source.y][7];
			board[source.y][7] = nullptr;
		}//long castle
		else if ((source.x - dest.x) == 2) {
			//move short rook
			board[source.y][dest.x + 1] = board[source.y][0];
			board[source.y][0] = nullptr;
		}
	}
	
	//Play move
	board[dest.y][dest.x] = board[source.y][source.x];
	board[source.y][source.x] = nullptr;
	if (board[dest.y][dest.x]) { //play
		board[dest.y][dest.x]->pos = dest;
		board[dest.y][dest.x]->moved = true;
	}
}

/*
	Sub-function for updateSelection,
	Simulates Play to each piece's allowed moves and reupdates accordingly.
	disclaimer: requires initialized board
*/
void ChessMD::validateSelection(Position src) {
	Position dest;
	bool move_path[8][8];
	bool attack_path[8][8];
	initBoolMatrix(move_path);
	initBoolMatrix(attack_path);
	addMatrix(board[src.y][src.x]->move_path, move_path);
	addMatrix(board[src.y][src.x]->attack_path, attack_path);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[src.y][src.x]->move_path[j][i]) {
				dest.x = i;
				dest.y = j;
				if (!SimulatePlay(src, dest)) {
					move_path[j][i] = false;
					attack_path[j][i] = false;
				}
			}
		}
	}
	//Reset and Copy new paths
	initBoolMatrix(board[src.y][src.x]->move_path);
	initBoolMatrix(board[src.y][src.x]->attack_path);
	addMatrix(move_path, board[src.y][src.x]->move_path);
	addMatrix(attack_path, board[src.y][src.x]->attack_path);
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
			if (board[j][i]) { //&& board[j][i]->type != PTYPE::KING) {
				pos.x = i;
				pos.y = j;
				if (board[j][i]->type == PTYPE::KING) { //King default Movement, will recalibrate later
					board[j][i]->Movement(board, board[j][i]->pos, nullptr);
				}
				else {
					board[j][i]->Movement(board, pos, nullptr);
				}
				ally_checked = (board[j][i]->color == PCOL::WHITE) ? whiteChecked : blackChecked;
				addMatrix(board[j][i]->attack_path, ally_checked);
			}
		}
	}

	//reupdates kings for checked movement
	king_black->Movement(board, king_black->pos, whiteChecked);
	addMatrix(king_black->attack_path, blackChecked);
	king_white->Movement(board, king_white->pos, blackChecked);
	addMatrix(king_white->attack_path, whiteChecked);

	//checks for checkmate / check
	checked = PCOL::NONE;
	king_white->checked = false;
	king_black->checked = false;
	if (whiteChecked[king_black->pos.y][king_black->pos.x]) {
		checked = PCOL::BLACK;
		king_black->checked = true;
		if (_played) {
			attacker = pSel;
		}
		if (isMoveEmpty(king_black->move_path) && isKingDefendable())
			return PCOL::WHITE;
	}
	if (blackChecked[king_white->pos.y][king_white->pos.x]) {
		checked = PCOL::WHITE;
		king_white->checked = true;
		if (_played) {
			attacker = pSel;
		}
		if (isMoveEmpty(king_white->move_path) && isKingDefendable())
			return PCOL::BLACK;
	}
	if (checked == PCOL::NONE && attacker) {
		attacker = nullptr;
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
		if (promotion) {
			promote(event);
		}
		else {
			if (!withinBounds(pos.x, pos.y)) {
				this->lastError = event + " is not a valid piece.";
			}
			else { //Piece has a pointer
				pTmp = this->board[pos.y][pos.x];
				if (pTmp) {
					validateSelection(pos);
				}
				if (this->pSel != pTmp) {
					if (this->pSel) {//pSel exists and different targeted
						if (this->pSel->move_path[pos.y][pos.x]) {
							if (isPlayValid(this->pSel->pos, pos)) {
								//PLAY
								UpdateEnPassant();
								Play(this->pSel->pos, pos);
								pSel = nullptr;
								turn = (turn == PCOL::WHITE) ? PCOL::BLACK : PCOL::WHITE;
								_played = true;
							}
						}
						else {
							this->pSel = nullptr;
						}
					}
					else if (pTmp) {
						if (pTmp->color == this->turn)
							this->pSel = pTmp;
						else if (pTmp->color != turn) {
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

	}
	pTmp = nullptr;

	if (_played) {
		this->winner = this->updateSelection();
		_played = false;
	}
	

	if (this->winner != PCOL::NONE) {
		this->_game = false;
		this->_running = false; //change when there is a mainmenu
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

bool ChessMD::isKingDefendable() {
	bool (*ally_checked)[8];
	bool defendable = false;
	if (attacker == nullptr) {
		return false;
	}
	else {
		ally_checked = (getChecked()->color == PCOL::WHITE) ? whiteChecked : blackChecked;
		//checks if attacker is edible
		if (ally_checked[attacker->pos.y][attacker->pos.x]) {
			return true;
		}
		//for every attacker's move, try to see if blocking it with a new piece helps
		for (int i = 0; i < 8; i++) {
			for (int j = 0; i < 8; j++) {
				if (attacker->move_path[j][i] && !board[j][i]) {
					CreatePiece(board[j][i], PTYPE::NONE);
					board[j][i]->pos.x = i;
					board[j][i]->pos.y = j;
					if (SimulatePlay(board[j][i]->pos, board[j][i]->pos)) {
						board[j][i]->~Piece();
						board[j][i] = nullptr;
						return true;
					}
					board[j][i]->~Piece();
					board[j][i] = nullptr;
				}
			}
		}
	}
	return false;
}

void ChessMD::deleteBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[j][i]) {
				delete(board[j][i]);
			}
		}
	}
}

//Promote the promoted pawn to one of the other types
void ChessMD::promote(std::string event) {
	/*
		Event required to be <p#>
		p1 = knight
		p2 = bishop
		p3 = rook
		p4 = queen
	*/
	if (!promoted || !promotion) { //misuse of promote
		return;
	}

	PTYPE promo_types[] = { PTYPE::KNIGHT, PTYPE::BISHOP, PTYPE::ROOK, PTYPE::QUEEN };
	PTYPE type;
	Position tmp_pos;
	PCOL tmp_col;
	//checks for "p#" format
	if (event.length() == 2 && event[0] == 'p' && event[1] >= '1' && event[1] <= '4') {
			tmp_pos = promoted->pos;
			tmp_col = promoted->color;
			promoted->~Piece();
			CreatePiece(board[tmp_pos.y][tmp_pos.x], promo_types[event[1] - '1']);
			board[tmp_pos.y][tmp_pos.x]->pos = tmp_pos;
			board[tmp_pos.y][tmp_pos.x]->moved = true;
			board[tmp_pos.y][tmp_pos.x]->color = tmp_col;
			promoted = nullptr;
			promotion = false;
	}
}

bool ChessMD::getPromotion() {
	return promotion;
}