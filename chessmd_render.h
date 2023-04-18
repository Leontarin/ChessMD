#ifndef CHESSMD_RENDER_H
#define CHESSMD_RENDER_H
#include "chessmd.h"
#include <windows.h>

const LPCWSTR TITLE = TEXT("ChessMD 0.0.5 (Not Working Yet)");

enum class COLOR { BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, LGRAY, GRAY, LBLUE, LGREEN, LAQUA, LRED, LPURPLE, LYELLOW, WHITE }; //Console Colors

	class ChessMD_Render {
	private:
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
		HWND hwnd = GetConsoleWindow();
		INPUT_RECORD InputRecord;
		DWORD Events;
		CONSOLE_CURSOR_INFO cci;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		void setPos(COORD coord);
		void setColor(COLOR fg, COLOR bg);
		void cls(HANDLE hout);
		void clean();
		void drawBoard(ChessMD game);
		void drawEtc();
		void drawPromotion();
		bool withinPromo(COORD coord);
		int PieceCenter(int);
		std::string positionToString(char x, char y);
		std::string positionToPromo(COORD coord);
		int convertToIndex(int);
		COORD ReadMouse();
	public:
		ChessMD_Render();
		~ChessMD_Render();
		void initWindow();
		void render(ChessMD game);
		std::string handleEvent();
		void ShowResults(PCOL winner);
		//debug render
		void debug_render(std::string event);
	};


#endif