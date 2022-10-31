#include <iostream>
#include "chessmd.h"
#include "windows.h"
#include "chessmd_render.h"


int main() {
	ChessMD game;
	ChessMD_Render renderer;
	while (game.getRunning()) {
		game.update(&game);
		renderer.render(game);
	}
	return 0;
}	