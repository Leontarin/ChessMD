#include "chessmd.h"
#include "chessmd_render.h"

int main() {
	ChessMD game;
	ChessMD_Render renderer;
	renderer.initWindow();
	renderer.render(game);
	while (game.getRunning()) {
		game.update(renderer.handleEvent());
		renderer.render(game);
	}
	renderer.ShowResults(game.getWinner());
	return 0;
}	