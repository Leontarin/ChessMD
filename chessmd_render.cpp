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

void ChessMD_Render::render(ChessMD game) {
	cls(hout);
	char pLetters[8] = { ' ','P','R','H','B','K','Q' };
	int iCell, jCell;
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
			if (i == (iCell * BOARD_SIZE + (BOARD_SIZE/2)) && j == (jCell * BOARD_SIZE + (BOARD_SIZE/2) )){
				pval = (int)game.getBoard()[iCell][jCell].p.type;
			}
			std::cout << pLetters[pval];
		}
	}
	setPos({ 0,(BOARD_SIZE*8) + 3 });
	setColor(COLOR::BLACK, COLOR::LWHITE);
	std::cout << "COMMAND: ";
}