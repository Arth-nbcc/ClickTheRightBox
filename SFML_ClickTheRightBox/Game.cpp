#include "Game.h"
#include <iostream>

//private functions
void Game::initializeVariables()
{
	this->window = nullptr;

	//game logic
	this->points = 0;
	this->health = 20;
	this->endGame = false;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnimies = 5;
	this->mouseHeld = false;
}

void Game::initializeWindow()
{
	this->videomode.height = 800;
	this->videomode.width = 1000;
	this->window = new sf::RenderWindow(this->videomode, "ClickTheRightBox", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
}

void Game::initializeEnimies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(50.f, 50.f));
	this->enemy.setFillColor(sf::Color::Cyan);
}

void Game::initializeFonts()
{
	if (this->font.loadFromFile("Fonts/Sansation.ttf"))
	{
		std::cout << "Error loading fonts." << "\n";
	}
}

void Game::initializeText()
{
	this->uitext.setFont(this->font);
	this->uitext.setCharacterSize(24);
	this->uitext.setString("NONE");
}

//constructors
Game::Game()
{
	this->initializeVariables();
	this->initializeWindow();
	this->initializeFonts();
	this->initializeText();
	this->initializeEnimies();

}

Game::~Game()
{
	delete this->window;
}


//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return false;
}

void Game::spawnEnemy()
{
	//spawn enimies and sets their types, colors and random position
	//-sets a random position
	//-sets a random color
	//--adds enemy to the vector

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getScale().x)),
		0.f	//because you dont need y to be random.
	);

	//randomize enemy type
	int type = rand() % 5;	//random position for 5 enimies
	switch (type)
	{
	case 0:
		this->enemy.setFillColor(sf::Color::Magenta);
		this->enemy.setSize(sf::Vector2f(20.f, 20.f));
		break;

	case 1:
		this->enemy.setFillColor(sf::Color::Blue);
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		break;

	case 2:
		this->enemy.setFillColor(sf::Color::Cyan);
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		break;

	case 3:
		this->enemy.setFillColor(sf::Color::Green);
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		break;

	case 4:
		this->enemy.setFillColor(sf::Color::Red);
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		break;

	default:
		this->enemy.setFillColor(sf::Color::Yellow);
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		break;
	}

	//spawn the enemy to vector
	this->enemies.push_back(this->enemy);
}

//functions
void Game::pollEvents()
{
	//event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
			//X button
		case sf::Event::Closed:
			this->window->close();
			break;

			//ESC button
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{
	//updates the mouse positions:
	//updates position relative to window(Vector2i)

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	//updates the enimies spawn timer and spawn enemies when the total amount of enimies is smaller than the maximum.
	//updating the timer for enemy spawning
	//move the enimies downwards.
	//removes the enimies at the edge of the screen
	if (this->enemies.size() < this->maxEnimies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}

		else
			this->enemySpawnTimer += 1.f;
	}

	//move and update the enimies
	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;
		this->enemies[i].move(0.f, 2.f);

		//if the enimies passed the botton of screen, delete it
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 4;
		}
	}

	//check of clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					//gain points		//simple way, isnt the best way!
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
					{
						this->points += 10;
						this->health += 10;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
					{
						this->points += 7;
						this->health += 7;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
					{
						this->points += 5;
						this->health += 5;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
					{
						this->points += 3;
						this->health += 3;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
					{
						this->points += 1;
						this->health += 1;
					}

					//delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePositions();
		this->updateText();
		this->updateEnemies();
	}

	if (this->health <= 0)
	{
		std::stringstream ss;

		ss << "Game Over! Try Again";
		this->uitext.setPosition(400, 400);
		this->uitext.setString(ss.str());
		this->endGame = true;
	}

	//update mouse position
	//relative to the window mouse position
	/*std::cout << "Mouse pos: "
		<< sf::Mouse::getPosition(*this->window).x
		<< " " << sf::Mouse::getPosition(*this->window).y
		<< "\n";
	*/
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n";

	this->uitext.setString(ss.str());
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uitext);
}

void Game::render()
{
	this->window->clear();

	//draw game objects

	this->renderenemies(*this->window);

	this->renderText(*this->window);
	this->window->display();
}

void Game::renderenemies(sf::RenderTarget& target)
{
	//rendering all enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}
