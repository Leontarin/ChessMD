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

void ChessMD_Render::render(ChessMD game) {
	char pLetters[8] = { ' ','P','R','H','B','K','Q' };
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			COLOR col = COLOR::BLACK;
			switch (game.getBoard()[i][j].p.color) {
			case PCOL::WHITE:
				col = COLOR::LBLUE;
				break;
			case PCOL::BLACK:
				col = COLOR::LRED;
				break;
			}
			setPos({ (short)i,(short)j });
			if ((i+j) % 2 ==0) {
				setColor(COLOR::LYELLOW, col);
			}
			else {
				setColor(COLOR::YELLOW, col);
			}
			int pval = (int)game.getBoard()[i][j].p.type;
			std::cout << pLetters[pval];
		}
	}

	setColor(COLOR::BLACK, COLOR::LWHITE);
}