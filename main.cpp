#include "chessmd.h"
#include "chessmd_render.h"

int main() {
	ChessMD game;
	ChessMD_Render renderer;
	std::string event;
	renderer.initWindow();
	renderer.render(game);

	while (game.getRunning()) {
		event = renderer.handleEvent();
		game.update(event);
		renderer.render(game);
	}
	renderer.ShowResults(game.getWinner());
	game.deleteBoard();
	return 0;
}	