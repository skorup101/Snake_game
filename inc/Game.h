#pragma once
#include <Snake.h>

class Game
{
public:
	Game()
		: ratPosGen(rat, snake), window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME)
	{ //clean it up there is also Rat.h file in git to remove or refactor whole game to headers
		font.loadFromFile(FONT_PATH);
		gameOverMsg.setCharacterSize(TEXT_SIZE);
		gameOverMsg.setFont(font);
		gameOverMsg.setPosition(STARTING_COORD_X - 200, STARTING_COORD_Y - TEXT_SIZE);

		window.setFramerateLimit(FRAMERATE);
	}

	void run()
	{
		while (window.isOpen())
		{
			render();
			readInput();
			updateLogic();
		}
	}

private:
	void render()
	{
		window.clear(sf::Color::Black);
		window.draw(snake);
		window.draw(rat);
		window.display();
	}
	void readInput()
	{
		sf::Event event;
		bool eventNotPulledYet = true;
		while (window.pollEvent(event) && eventNotPulledYet)
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					snake.turnLeft();
					eventNotPulledYet = false;
					break;
				case sf::Keyboard::Right:
					snake.turnRight();
					eventNotPulledYet = false;
					break;
				case sf::Keyboard::Up:
					snake.turnUp();
					eventNotPulledYet = false;
					break;
				case sf::Keyboard::Down:
					snake.turnDown();
					eventNotPulledYet = false;
					break;
				default:
					break;
				}
			}
		}
	}
	void updateLogic()
	{
		if (snake.crashed())
		{
			gameOver();
		}
		handleRatEating();
		snake.move();
	}

	void handleRatEating()
	{
		if (snake.getHead() == rat.getRatLocation())
		{
			snake.eat();
			ratPosGen.randomize();
		}
	}

	void gameOver()
	{
		const std::string finalMessage = GAME_OVER_MSG.append(snake.getResult());
		gameOverMsg.setString(finalMessage);
		window.draw(gameOverMsg);
		window.display();

		sf::Event lastEvent;
		while (window.isOpen())
		{
			if (window.pollEvent(lastEvent))
			{
				if (lastEvent.type == sf::Event::Closed)
				{
					window.close();
				}
			}
		}
	}

	Snake snake;
	sf::RenderWindow window;
	Rat rat;
	RatPositionGenerator ratPosGen;
	sf::Font font;
	sf::Text gameOverMsg;
};
