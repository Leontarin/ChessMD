
#ifndef CHESSMD_H
#define CHESSMD_H
#include <string>
#include <iostream>
#include "chessmd_piece.h"
#include "chessmd_utilities.h"
#define BOARD_SIZE 3

class ChessMD {
private:
	bool _running;
	bool _game;
	PCOL turn;
	PCOL checked;
	PCOL winner;
	Piece* board[8][8];
	bool (*boardSel)[8] = nullptr; //board current piece selection
	bool whiteChecked[8][8]; //white checked territory
	bool blackChecked[8][8]; //black checked territory
	void initBoard(Piece* (&)[8][8]);
	void initGame(Piece* (&)[8][8]);
	void initBoardPlacement(Piece* (&)[8][8]);
	std::string handleEvent();
	std::string lastError;
	Piece* pSel = nullptr;
	Position stringToPosition(std::string);
	bool parseEvent(std::string);
	PCOL updateSelection();
	void addMatrix(bool(*source)[8], bool(*target)[8]);
	void Play(Position source, Position dest);

public:
	ChessMD();
	~ChessMD();
	bool getRunning();
	void update(ChessMD* game);
	Piece_Matrix getBoard();
	Piece const* getSelected();
	bool isNotEmpty(char x, char y);
	std::string getLastError();
	void debug(); //use for debugging, remove after program finish
};

#endif
