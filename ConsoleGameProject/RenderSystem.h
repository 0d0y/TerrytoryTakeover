#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include "Refs.h"


void DrawTitle();
void DrawManual();
void DrawFailed();
void DrawClear();
void textColor(int foreColor, int backColor);
namespace render
{
	extern bool bScreenIndex;
	extern HANDLE hScreen[2];

	void InitScreen();
	void ScreenRelease();

	void ScreenFlipping();
	void ScreenClear();

	void ScreenDraw(int x, int y, const char c);
	void ScreenDraw(int x, int y, const char* pStr);
	void GotoXY(int x, int y);


	SMALL_RECT GetPlayerMoveableRect();

	void DrawBorder();
};