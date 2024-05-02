#pragma once
#include <windows.h>
#include <cassert>
#include <conio.h>
#include <stdbool.h>
#include "Refs.h"
short CheckcurPlayerCondition();

namespace global
{
	namespace input
	{
		const int ESCAPE_KEY_INDEX = 0;
		const int USER_CMD_LEFT = 1;
		const int USER_CMD_RIGHT = 2;
		const int USER_CMD_UP = 3;
		const int USER_CMD_DOWN = 4;

		const int MAX_KEY = 5;

		void Set(const int keyIdx, bool bOn);
		void Set(bool bOn);

		bool IsEscapeCmdOn();
		bool IsLeftCmdOn();
		bool IsRightCmdOn();
		bool IsUpCmdOn();
		bool IsDownCmdOn();
		int UpdateInput();
	};
};