#include "Refs.h"

namespace global
{
	int oldDirection = Direction::Stop;
	int curDirection = Direction::Stop;
	bool inputKey;
	bool BlankMove = false;
	bool TempBlankMove = false;
	int width, height;
	int Start = 0;
	int Stage = 1;
	bool Dead = false;
	bool GameRun = true;
	bool bMapprepared = false;
	SMALL_RECT playerMoveableRect = { 4, 4, 30, 30 };
	short** CoordCondition; //CoordCondition[height][width]
	WORD* colors;
}