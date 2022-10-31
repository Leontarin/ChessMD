#ifndef CHESSMD_RENDER_H
#define CHESSMD_RENDER_H
#include "chessmd.h"
#include <windows.h>
enum CCOL {BLACK,}; //Console Colors

	class ChessMD_Render {
	private:
		void setPos(COORD coord);
	public:
		void render(ChessMD game);
	};


#endif