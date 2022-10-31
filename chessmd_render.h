#ifndef CHESSMD_RENDER_H
#define CHESSMD_RENDER_H
#include "chessmd.h"
#include <windows.h>


	class ChessMD_Render {
	private:
		enum class COLOR { BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, LGRAY, GRAY, LBLUE, LGREEN, LAQUA, LRED, LPURPLE, LYELLOW, LWHITE }; //Console Colors
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		void setPos(COORD coord);
		void setColor(COLOR color);
		void cls();
		void drawBoard(ChessMD game);

	public:
		ChessMD_Render();
		~ChessMD_Render();
		void render(ChessMD game);
	};


#endif