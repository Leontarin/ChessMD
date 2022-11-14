#include <iostream>
#ifndef CHESSMD_H
#define CHESSMD_H
#define BOARD_SIZE 1

enum class PCOL { NONE, BLACK, WHITE, last }; //PIECE COLOR ENUM
enum class PTYPE { NONE, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, last }; //PIECE TYPE ENUM

class ChessMD {
private:
	bool _running;
	bool _game;
	PCOL turn;
	PCOL checked;
	PCOL winner;
	typedef struct Piece {
		PTYPE type;
		PCOL color;
	} Piece;
	typedef struct Cell {
		struct Piece p;
		bool** sel = new bool * [8]; //Cell selection
		bool mov;
	} Cell;
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
