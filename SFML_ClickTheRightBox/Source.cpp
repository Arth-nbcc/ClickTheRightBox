#include<iostream>
#include "Game.h"
int main()
{
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init game engine
	Game game;

	//Game loop
	while (game.running() && !game.getEndGame())
	{
		//update
		game.update();

		//render
		game.render();
	}

	//end of game
	return 0;
}