#ifndef CHESSMD_RENDER_H
#define CHESSMD_RENDER_H
#include "chessmd.h"
#include <windows.h>

const LPCWSTR TITLE = TEXT("ChessMD 0.0.1");

enum class COLOR { BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, LGRAY, GRAY, LBLUE, LGREEN, LAQUA, LRED, LPURPLE, LYELLOW, LWHITE }; //Console Colors

	class ChessMD_Render {
	private:
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		void setPos(COORD coord);
		void setColor(COLOR fg, COLOR bg);
		void cls(HANDLE hout);
		void drawBoard(ChessMD game);
		int CellCenter(int);
	public:
		ChessMD_Render();
		~ChessMD_Render();
		void initWindow();
		void render(ChessMD game);
	};


#endif