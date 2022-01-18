#pragma once

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

/*
	class that acts as the game engine
	wrapper class
*/

class Game
{
private:
	//variables
	//window
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	//mouse positions
	sf::Vector2i	mousePosWindow;
	sf::Vector2f	mousePosView;

	//Resources
	sf::Font font;

	//Text
	sf::Text		uitext;

	//game logic
	unsigned		points;
	int				health;
	bool			endGame;
	float			enemySpawnTimer;
	float			enemySpawnTimerMax;
	int				maxEnimies;
	bool			mouseHeld;

	//game objects
	std::vector<sf::RectangleShape>enemies;
	sf::RectangleShape enemy;

	//private functions
	void			initializeVariables();
	void			initializeWindow();
	void			initializeEnimies();
	void			initializeFonts();
	void			initializeText();

public:
	Game(); //constructor
	virtual ~Game(); //destructor

	//Accessors
	const bool		running() const;
	const bool		getEndGame() const;

	//functions
	void			spawnEnemy();
	void			pollEvents();

	void			updateMousePositions();
	void			updateEnemies();
	void			update();
	void			updateText();

	void			renderText(sf::RenderTarget& target);
	void			render();
	void			renderenemies(sf::RenderTarget& target);
};

