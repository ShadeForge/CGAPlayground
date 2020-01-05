#include <iostream>
#include <GL/glew.h>
#include "Game.h"
#include "MSGame.h"

GLint main() {
	Game* game = new MSGame();

	game->loop();

	delete game;

	return 0;
}