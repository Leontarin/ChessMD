#include "chessmd_render.h"
#include <iostream>
#include <string>

ChessMD_Render::ChessMD_Render() {
	
}

ChessMD_Render::~ChessMD_Render() {

}

void ChessMD_Render::setPos(COORD coord) {
	SetConsoleCursorPosition(hout, coord);
}

void ChessMD_Render::setColor(COLOR bg, COLOR fg) {
	SetConsoleTextAttribute(hout, ((int)bg * 16) + (int)fg);
}

void ChessMD_Render::cls(HANDLE hout) {
	setColor(COLOR::BLACK, COLOR::BLACK);
	if (hout == INVALID_HANDLE_VALUE) return;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count, cellCount;
	COORD homeCoords = { 0,0 };
	if (!GetConsoleScreenBufferInfo(hout, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;
	if (!FillConsoleOutputCharacter(hout, (TCHAR)' ', cellCount, homeCoords, &count)) return;
	if (!FillConsoleOutputAttribute(hout, csbi.wAttributes, cellCount, homeCoords, &count)) return;
	setPos({ 0,0 });
	setColor(COLOR::BLACK, COLOR::LWHITE);
}

int ChessMD_Render::CellCenter(int c) {
	return c * BOARD_SIZE + (BOARD_SIZE / 2);
}

void ChessMD_Render::render(ChessMD game) {
	cls(hout);
	char pLetters[8] = { ' ','P','R','H','B','K','Q' };
	int iCell, jCell;
	//Board Render
	for (int i = 0; i < (BOARD_SIZE*8); i++) {
		for (int j = 0; j < (BOARD_SIZE*8); j++){
			iCell = (int)((i / BOARD_SIZE));
			jCell = (int)((j / BOARD_SIZE));
			COLOR col = COLOR::BLACK;
			switch (game.getBoard()[iCell][jCell].p.color) {
			case PCOL::WHITE:
				col = COLOR::LBLUE;
				break;
			case PCOL::BLACK:
				col = COLOR::LRED;
				break;
			}
			setPos({ (short)i,(short)j });
			if ((iCell+jCell) % 2 ==0) {
				setColor(COLOR::LYELLOW, col);
			}
			else {
				setColor(COLOR::YELLOW, col);
			}
			int pval = 0;
			if (i == CellCenter(iCell) && j == CellCenter(jCell)){
				pval = (int)game.getBoard()[iCell][jCell].p.type;
			}
			std::cout << pLetters[pval];
		}
	}
	//Board Letters
	setColor(COLOR::BLACK, COLOR::LWHITE);
	for (int x = 0;x < 8;x++) {
		setPos({ (short)CellCenter(x), (BOARD_SIZE*8) });
		std::cout << (char)('a' + x);
	}
	for (int y = 0;y < 8;y++) {
		setPos({(BOARD_SIZE * 8) + 1,(short)CellCenter(y)});
		std::cout << 8-y;
	}
	
	//Error Message
	setPos({ 0,(BOARD_SIZE * 8) + 4 });
	setColor(COLOR::BLACK, COLOR::LRED);
	std::cout << game.getLastError();

	//Command
	setPos({ 0,(BOARD_SIZE * 8) + 3 });
	setColor(COLOR::BLACK, COLOR::LWHITE);
	std::cout << "COMMAND: ";
}