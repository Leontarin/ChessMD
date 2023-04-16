#include "chessmd_render.h"
#include "chessmd.h"
#include <iostream>
#include <string>
#define BOARD_SIZE 3
const COORD EXIT_COORD = { (BOARD_SIZE) * 8 + (BOARD_SIZE)*3 , BOARD_SIZE};
const COORD RESULT_COORD = { (BOARD_SIZE) * 8 + (BOARD_SIZE) * 3 , BOARD_SIZE*3 };
const COORD LAST_ERROR = { 0,(BOARD_SIZE * 8) + 4 };
ChessMD_Render::ChessMD_Render() {

}

ChessMD_Render::~ChessMD_Render() {

}

int ChessMD_Render::convertToIndex(int i) {
	//returns the index of given cell and defined board_size
	return (int)((i / BOARD_SIZE));
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
	DWORD count, PieceCount;
	COORD homeCoords = { 0,0 };
	if (!GetConsoleScreenBufferInfo(hout, &csbi)) return;
	PieceCount = csbi.dwSize.X * csbi.dwSize.Y;
	if (!FillConsoleOutputCharacter(hout, (TCHAR)' ', PieceCount, homeCoords, &count)) return;
	if (!FillConsoleOutputAttribute(hout, csbi.wAttributes, PieceCount, homeCoords, &count)) return;
	setPos({ 0,0 });
	setColor(COLOR::BLACK, COLOR::WHITE);
}

void ChessMD_Render::clean() {
	setColor(COLOR::BLACK, COLOR::BLACK);
	setPos(LAST_ERROR);
	std::cout << "                                                  ";
}

int ChessMD_Render::PieceCenter(int c) {
	return c * BOARD_SIZE + (BOARD_SIZE / 2);
}

void ChessMD_Render::drawBoard(ChessMD game) {
	char pLetters[8] = { ' ','P','R','H','B','K','Q' };
	int iPiece, jPiece;
	COLOR colors_board[2] = { COLOR::LGRAY, COLOR::GRAY };
	COLOR colors_selected_move[2] = { COLOR::LYELLOW, COLOR::LYELLOW };
	COLOR colors_selected_target[2] = { COLOR::LAQUA, COLOR::AQUA };
	COLOR colors_checked[2] = { COLOR::LRED, COLOR::RED };
	COLOR* colors_arr;
	Piece const* selected = game.getSelected();
	//Board Render
	for (int i = 0; i < (BOARD_SIZE * 8); i++) {
		for (int j = 0; j < (BOARD_SIZE * 8); j++) {
			iPiece = convertToIndex(i);
			jPiece = convertToIndex(j);
			COLOR col = COLOR::BLACK;
			colors_arr = colors_board;
			if (game.getBoard()[iPiece][jPiece]) {
				switch (game.getBoard()[iPiece][jPiece]->color) {
				case PCOL::WHITE:
					col = COLOR::BLUE;
					break;
				case PCOL::BLACK:
					col = COLOR::LRED;
					break;
				}
			}
			if (selected) { //selected pathing coloring
				if (selected->move_path[iPiece][jPiece]) {
					colors_arr = colors_selected_move;
				}
			}
			if (game.getSelected() && game.getBoard()[iPiece][jPiece] == game.getSelected()) { //selected unit coloring
				colors_arr = colors_selected_target;
			}
			if (game.getBoard()[iPiece][jPiece] && game.getChecked() == game.getBoard()[iPiece][jPiece]) {
				colors_arr = colors_checked;
				col = (game.getChecked()->color == PCOL::WHITE) ? COLOR::WHITE : COLOR::BLACK;
			}
			setPos({ (short)j,(short)i });
			if ((iPiece + jPiece) % 2 == 0) {
				setColor(colors_arr[0], col);
			}
			else {
				setColor(colors_arr[1], col);
			}
			int pval = 0;
			if (i == PieceCenter(iPiece) && j == PieceCenter(jPiece)) {
				if (game.getBoard()[iPiece][jPiece])
					pval = (int)game.getBoard()[iPiece][jPiece]->type;
			}
			std::cout << pLetters[pval];
		}
	}
	//Board Letters
	setColor(COLOR::BLACK, COLOR::WHITE);
	for (int x = 0; x < 8; x++) {
		setPos({ (short)PieceCenter(x), (BOARD_SIZE * 8) });
		std::cout << (char)('a' + x);
	}
	for (int y = 0; y < 8; y++) {
		setPos({ (BOARD_SIZE * 8) + 1,(short)PieceCenter(y) });
		std::cout << 8 - y;
	}
}

void ChessMD_Render::drawEtc() {
	setColor(COLOR::RED, COLOR::WHITE);
	COORD coord;
	std::string symbol[3] = {"   "," X ","   "};
	for (int i = 0; i < 3; i++) {
			coord.X = EXIT_COORD.X;
			coord.Y = EXIT_COORD.Y + i;
			setPos(coord);
			std::cout << symbol[i];
	}
}

void ChessMD_Render::render(ChessMD game) {
	//cls(hout);
	clean();
	
	drawBoard(game);
	drawEtc();

	//Error Message
	setPos(LAST_ERROR);
	setColor(COLOR::BLACK, COLOR::LRED);
	std::cout << game.getLastError();
	
	
	setColor(COLOR::BLACK, COLOR::WHITE);
}

void ChessMD_Render::initWindow() {
	SetConsoleTitleW(TITLE);
	GetConsoleCursorInfo(hout, &cci);
	cci.dwSize = 25;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ~ENABLE_QUICK_EDIT_MODE);

	//Get Desktop Resolution
	RECT desktop;
	HWND hwndesk = GetDesktopWindow();
	GetWindowRect(hwndesk, &desktop);

	if (hwnd != NULL) { MoveWindow(hwnd, (desktop.right - 800) / 2, (desktop.bottom - 600) / 2, 800, 600, TRUE); } //set Window size

	ShowScrollBar(hwnd, SB_BOTH, FALSE);
	ShowScrollBar(hwnd, SB_BOTH, FALSE);
	
	setColor(COLOR::BLACK, COLOR::WHITE);
	cls(hout);

}

COORD ChessMD_Render::ReadMouse() {

	ReadConsoleInput(hin, &InputRecord, 1, &Events);
	if (InputRecord.EventType == MOUSE_EVENT && InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
		COORD coord = { InputRecord.Event.MouseEvent.dwMousePosition.X, InputRecord.Event.MouseEvent.dwMousePosition.Y };
		return coord;
	}
	return { -1,-1 };
}

std::string ChessMD_Render::positionToString(char x, char y) {
	//returns current position as string
		std::string pos = "";
		pos += x + 'a';
		pos += '8'-y;
		return pos;
}

std::string ChessMD_Render::handleEvent() {
	/*
		Event currently handled by manual commands
		Will change in the future to Mouse Events
	*/
	COORD coord = { -1,-1 };
	std::string event = "";
	while (coord.X == -1 && coord.Y == -1) {
		coord = ReadMouse();
		Sleep(100);
	}
	int x = convertToIndex(coord.X);
	int y = convertToIndex(coord.Y);

	if (withinBounds(x, y)) {
		event = positionToString(x, y);
	}
	else {
		if (coord.X >= EXIT_COORD.X && coord.X < EXIT_COORD.X + BOARD_SIZE && coord.Y >= EXIT_COORD.Y && coord.Y < EXIT_COORD.Y + BOARD_SIZE) //exit button coord
			event = "quit";
	}
	return event;
}

void ChessMD_Render::ShowResults(PCOL winner) {
	setPos(RESULT_COORD);
	setColor(COLOR::BLACK, COLOR::YELLOW);
	std::cout << "THE WINNER IS ";
	if (winner == PCOL::BLACK) {
		setColor(COLOR::BLACK, COLOR::LRED);
		std::cout << "BLACK";
	}
	else {
		setColor(COLOR::BLACK, COLOR::LBLUE);
		std::cout << "WHITE";
	}
	setColor(COLOR::BLACK, COLOR::WHITE);
}

void ChessMD_Render::debug_render(ChessMD game) {
	char pLetters[8] = { ' ','P','R','H','B','K','Q' };
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			std::cout << game.getSelected()->move_path[i][j];
		}
		std::cout << std::endl;
	}
}