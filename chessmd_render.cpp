#include "chessmd_render.h"
#include <iostream>

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
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			COLOR col = COLOR::BLACK;
			switch (game.getBoard()[i][j].p.color) {
			case PCOL::WHITE:
				col = COLOR::LWHITE;
				break;
			case PCOL::BLACK:
				col = COLOR::BLACK;
				break;
			}
			setPos({ (short)i,(short)j });
			if ((i+j) % 2 ==0) {
				setColor(COLOR::LYELLOW, col);
			}
			else {
				setColor(COLOR::YELLOW, col);
			}
			std::cout << ((int)game.getBoard()[i][j].p.type == (int)PTYPE::NONE) ? ' ' : (int)game.getBoard()[i][j].p.type;
		}
	}

	setColor(COLOR::BLACK, COLOR::LWHITE);
}