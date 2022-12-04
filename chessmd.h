#include <iostream>
#include "chessmd_piece.h"
#include "chessmd_utilities.h"
#ifndef CHESSMD_H
#define CHESSMD_H
#define BOARD_SIZE 3

class ChessMD {
private:
	bool _running;
	bool _game;
	PCOL turn;
	PCOL checked;
	PCOL winner;
	Cell** board = new Cell * [8];
	bool** boardSel = new bool * [8]; //board current piece selection
	bool** whiteChecked = new bool* [8]; //white checked territory
	bool** blackChecked = new bool* [8]; //black checked territory
	void initBoard(Cell**);
	void initGame(Cell**);
	void initBoardPlacement(Cell**);
	std::string handleEvent();
	std::string lastError;
	Cell* cSel;
	Cell* stringToCell(std::string);
	bool parseEvent(std::string);
	bool isCellValid(Cell*);
	PCOL updateSelection();

public:
	ChessMD();
	~ChessMD();
	bool getRunning();
	void update(ChessMD* game);
	Cell const* const* getBoard();
	std::string getLastError();
};

#endif
