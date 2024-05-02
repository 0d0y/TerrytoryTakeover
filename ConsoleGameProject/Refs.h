#pragma once
#include <Windows.h>
#include <iostream>
enum Direction { Stop = 0, Up = 1, Left = 2, Down = 3, Right = 4 };
enum Condition {
	Blank,
	Line,
	LineV,
	LineH,
	LineUR,
	LineUL,
	LineDR,
	LineDL,
	DrawingLineV,
	DrawingLineH,
	DrawingLineUR,
	DrawingLineUL,
	DrawingLineDR,
	DrawingLineDL,
	Land
};

enum COLOR {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VIOLET,
	DARK_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

namespace global
{
	extern int oldDirection;
	extern int curDirection;
	extern bool inputKey;
	extern bool BlankMove;
	extern bool TempBlankMove;
	extern int width, height;
	extern int Start;
	extern int Stage;
	extern bool Dead;
	extern bool GameRun;
	extern bool bMapprepared;
	extern SMALL_RECT playerMoveableRect;
	extern short** CoordCondition; //CoordCondition[height][width]
	extern WORD* colors;
}