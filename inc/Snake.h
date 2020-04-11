#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <tuple>
#include <Glo.h>
#include <iostream>

enum EDirection
{
	Up,
	Down,
	Right,
	Left
};

struct SnakeSegment
{
	SnakeSegment(coord x, coord y)
		: x(x), y(y) {}
	coord getX() const
	{
		return x;
	}
	coord getY() const
	{
		return y;
	}
	bool operator==(const SnakeSegment ss)
	{
		if (x == ss.getX() && y == ss.getY())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator!=(const SnakeSegment ss)
	{
		if (*this == ss)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

private:
	coord x;
	coord y;
};

typedef SnakeSegment RatSegment;

class Rat : public sf::Drawable
{
public:
	Rat() {}
	void setRatLocation(coord x, coord y)
	{
		this->x = x;
		this->y = y;
	}
	RatSegment getRatLocation()
	{
		return RatSegment(x, y);
	}

private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		sf::RectangleShape ratGraphic(sf::Vector2f(SEGMENT_SIZE, SEGMENT_SIZE));
		ratGraphic.setPosition(x, y);
		ratGraphic.setFillColor(sf::Color::Green);
		ratGraphic.setOutlineColor(sf::Color::Black);
		ratGraphic.setOutlineThickness(SEGMENT_OFFSET);
		target.draw(ratGraphic);
	}
	coord x;
	coord y;

	friend class RatPositionGenerator;
};

class Snake : public sf::Drawable
{
public:
	Snake() : currentDirection(Up)
	{
		for (unsigned i = 0; i < STARTING_LENGTH; ++i)
		{
			snakeBody.push_back(SnakeSegment(STARTING_COORD_X, STARTING_COORD_Y + SEGMENT_SIZE * i));
		}
	}
	~Snake() override
	{
	}
	void turnUp()
	{
		if (currentDirection != Down)
		{
			currentDirection = Up;
		}
	}
	void turnDown()
	{
		if (currentDirection != Up)
		{
			currentDirection = Down;
		}
	}
	void turnLeft()
	{
		if (currentDirection != Right)
		{
			currentDirection = Left;
		}
	}
	void turnRight()
	{
		if (currentDirection != Left)
		{
			currentDirection = Right;
		}
	}
	void eat()
	{
		snakeBody.push_back(SnakeSegment(1, 1));
	}

	void move()
	{
		for (int i = snakeBody.size() - 1; i > 0; --i)
		{
			snakeBody[i] = snakeBody[i - 1];
		}
		snakeBody[0] = nextSegment();
		handleBoarderCrossing();
	}

	SnakeSegment getHead() const
	{
		return snakeBody[0];
	}

	std::string getResult() const
	{
		return std::to_string(snakeBody.size() - STARTING_LENGTH);
	}

	std::vector<SnakeSegment> getSnakeBody() const
	{
		return snakeBody;
	}

	void handleBoarderCrossing()
	{
		switch (currentDirection)
		{
		case Up:
			if (snakeBody[0].getY() < 0)
				snakeBody[0] = SnakeSegment(snakeBody[0].getX(), WINDOW_HEIGHT - SEGMENT_SIZE);
			break;
		case Down:
			if (snakeBody[0].getY() == WINDOW_HEIGHT)
				snakeBody[0] = SnakeSegment(snakeBody[0].getX(), 0);
			break;
		case Right:
			if (snakeBody[0].getX() == WINDOW_WIDTH)
				snakeBody[0] = SnakeSegment(0, snakeBody[0].getY());
			break;
		case Left:
			if (snakeBody[0].getX() < 0)
				snakeBody[0] = SnakeSegment(WINDOW_WIDTH - SEGMENT_SIZE, snakeBody[0].getY());
			break;
		default:
			break;
		}
	}

	bool crashed()
	{
		for (size_t i = 1; i < snakeBody.size(); ++i)
		{
			if (getHead() == snakeBody[i])
				return true;
		}
		return false;
	}

private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		std::for_each(snakeBody.begin(),
					  snakeBody.end(),
					  [&target](auto segment) {
						  sf::RectangleShape segmentGraphic(sf::Vector2f(SEGMENT_SIZE, SEGMENT_SIZE));
						  segmentGraphic.setPosition(segment.getX(), segment.getY());
						  segmentGraphic.setFillColor(sf::Color::Green); //make color swapper
						  segmentGraphic.setOutlineColor(sf::Color::Black);
						  segmentGraphic.setOutlineThickness(SEGMENT_OFFSET);
						  target.draw(segmentGraphic);
					  });
	}

	SnakeSegment nextSegment() const
	{
		switch (currentDirection)
		{
		case Up:
			return SnakeSegment(getHead().getX(), getHead().getY() - SEGMENT_SIZE);
			break;
		case Down:
			return SnakeSegment(getHead().getX(), getHead().getY() + SEGMENT_SIZE);
			break;
		case Right:
			return SnakeSegment(getHead().getX() + SEGMENT_SIZE, getHead().getY());
			break;
		case Left:
			return SnakeSegment(getHead().getX() - SEGMENT_SIZE, getHead().getY());
			break;
		default:
			return SnakeSegment(1, 1);
			break;
		}
	}
	EDirection currentDirection;
	std::vector<SnakeSegment> snakeBody;
};

class RatPositionGenerator
{
public:
	RatPositionGenerator(Rat &rat, const Snake &s) : rat(rat), snake(s)
	{
		srand(static_cast<unsigned int>(time(NULL)));
		randomize();
	}
	void randomize()
	{
		RatSegment ratSegment = generateNonSnakeCoords();
		rat.setRatLocation(ratSegment.getX(), ratSegment.getY());
	}

private:
	RatSegment generateNonSnakeCoords()
	{
		RatSegment result(randCoordX(), randCoordY());

		bool isInSnakeBody = true;
		while (isInSnakeBody)
		{
			isInSnakeBody = false;
			for (auto segment : snake.getSnakeBody())
			{
				if (segment.getX() == result.getX() &&
					segment.getY() == result.getY())
				{
					isInSnakeBody = true;
					result = RatSegment(randCoordX(), randCoordY());
					break;
				}
			}
		}
		return result;
	}

	coord randCoordX()
	{ //coord within window's range in SEGMENTs net
		return ((rand() % WINDOW_WIDTH) / SEGMENT_SIZE) * SEGMENT_SIZE;
	}

	coord randCoordY()
	{ //coord within window's range in SEGMENTs net
		return ((rand() % WINDOW_HEIGHT) / SEGMENT_SIZE) * SEGMENT_SIZE;
	}

	unsigned seed;
	const Snake &snake;
	Rat &rat;
};
