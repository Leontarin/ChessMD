#include <iostream>
#include "chessmd.h"
#include "windows.h"
#include "chessmd_render.h"


int main() {
	ChessMD game;
	ChessMD_Render renderer;
	renderer.initWindow																																						();
	while (game.getRunning()) {
		renderer.render(game);
		game.update(&game);
	}
	return 0;
}	