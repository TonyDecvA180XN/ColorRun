#pragma once

#include "Type.h"

/*
	Defines states of the game.
	Initial option is Menu.
	[Menu] -> [Level] - start game
	[Level] -> [Menu, Result] - pause/finish game
	[Result] -> [Menu, Level] - choose another level/replay
*/
enum class EGameState
{
	GameStateNone = 0,
	GameStateMenu,
	GameStateLevel,
	GameStateResult
};
