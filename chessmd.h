
#ifndef CHESSMD_H
#define CHESSMD_H
#include <string>
#include <iostream>
#include "chessmd_piece.h"
#include "chessmd_utilities.h"
#define BOARD_SIZE 1

class ChessMD {
private:
	bool _running;
	bool _game;
	PCOL turn;
	PCOL checked;
	PCOL winner;
	Piece** board;
	bool** boardSel = new bool * [8]; //board current piece selection
	bool** whiteChecked = new bool* [8]; //white checked territory
	bool** blackChecked = new bool* [8]; //black checked territory
	void initBoard(Piece**);
	void initGame(Piece**);
	void initBoardPlacement(Piece**);
	std::string handleEvent();
	std::string lastError;
	Piece* cSel;
	Piece* stringToPiece(std::string);
	bool parseEvent(std::string);
	bool isPieceValid(Piece*);
	PCOL updateSelection();

public:
	ChessMD();
	~ChessMD();
	bool getRunning();
	void update(ChessMD* game);
	Piece const* const* getBoard();
	std::string getLastError();
};

#endif
