#pragma once
#include<string>

typedef short coord;

//Window parameters
const std::string WINDOW_NAME =		"SNAKE";
std::string GAME_OVER_MSG =			"GAME OVER\nSCORE: ";
const std::string FONT_PATH =		"C:/Windows/Fonts/RAVIE.TTF";
const std::string FONT_PATH_ARIAL = "C:/Windows/Fonts/Arial.ttf";
const unsigned int WINDOW_WIDTH =	420; //must be multiplifier of SEGMENT_SIZE
const unsigned int WINDOW_HEIGHT =	420; //must be multiplifier of SEGMENT_SIZE
const unsigned int FRAMERATE =		4;
const unsigned int TEXT_SIZE =		50;

//Snake parameters
size_t STARTING_LENGTH =		6;
const coord SEGMENT_SIZE =		30;
const coord SEGMENT_OFFSET =	-2;
const coord STARTING_COORD_X =	WINDOW_WIDTH / 2;
const coord STARTING_COORD_Y =	WINDOW_HEIGHT / 2;
