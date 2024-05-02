#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <random>
#include "Refs.h"
#include "InputSystem.h"
#include "RenderSystem.h"

struct COORD2
{
	COORD A;
	COORD B;
};
struct ENEMY {
	int x;
	int y;
	int dx;
	int dy;
};

namespace global
{
	namespace time
	{
		ULONGLONG previousTime;
		ULONGLONG currentTime;
		ULONGLONG deltaTime;

		int updateCount;
		int fixedUpdateCount;

		void InitTime()
		{
			currentTime = previousTime = GetTickCount64();
		}

		void UpdateTime()
		{
			previousTime = currentTime;
			currentTime = GetTickCount64();

			deltaTime = currentTime - previousTime;
		}

		ULONGLONG GetDeltaTime()
		{
			return deltaTime;
		}
	};
	COORD curPlayerPos;

	COORD2 drawStartPos;
	COORD2 drawEndPos;

	int MyAreaPersent = 0;


	ENEMY enemies[50] = { 0, };

	const int WinPersent = 70;
	const int enemyMoveSpeed = 80;
	const int playerMoveSpeed = 60;
};
void Clamp(short& n, short min, short max)
{
	if (n < min) n = min;
	if (n > max) n = max;
}
void DrawPlayer()
{
	render::ScreenDraw(global::curPlayerPos.X, global::curPlayerPos.Y, "○");
}
short CheckcurPlayerCondition()
{
	int x = global::curPlayerPos.X;
	int y = global::curPlayerPos.Y;

	return global::CoordCondition[y - 1][x - 1];
}
short CheckNextPlayerCondition()
{
	int x = global::curPlayerPos.X;
	int y = global::curPlayerPos.Y;

	if (global::curDirection == Direction::Stop)
	{

	}
	if (global::curDirection == Direction::Up)
	{
		y--;
	}
	if (global::curDirection == Direction::Down)
	{
		y++;
	}
	if (global::curDirection == Direction::Left)
	{
		x--;
	}
	if (global::curDirection == Direction::Right)
	{
		x++;
	}
	if (y - 1 < 0 || y - 1 > global::height - 1 || x - 1 < 0 || x - 1 > global::width - 1)
	{
		return -1;
	}
	else
	{
		return global::CoordCondition[y - 1][x - 1];
	}
}
void Linestop()
{
	if (CheckNextPlayerCondition() == Condition::Land)
	{
		global::curDirection = Direction::Stop;
	}
}
void CheckDrawDeath()
{
	if (CheckNextPlayerCondition() == DrawingLineV ||
		CheckNextPlayerCondition() == DrawingLineH ||
		CheckNextPlayerCondition() == DrawingLineUL ||
		CheckNextPlayerCondition() == DrawingLineUR ||
		CheckNextPlayerCondition() == DrawingLineDL ||
		CheckNextPlayerCondition() == DrawingLineDR)
	{
		global::GameRun = false;
		global::Dead = true;
	}
	return;
}
void DrawLine()
{
	if (CheckcurPlayerCondition() == Condition::Blank)
	{
		int x = global::curPlayerPos.X;
		int y = global::curPlayerPos.Y;

		if (global::oldDirection == Direction::Up && global::curDirection == Direction::Up ||
			global::oldDirection == Direction::Down && global::curDirection == Direction::Down)
		{
			global::CoordCondition[y - 1][x - 1] = Condition::DrawingLineV;
		}
		if (global::oldDirection == Direction::Left && global::curDirection == Direction::Left ||
			global::oldDirection == Direction::Right && global::curDirection == Direction::Right)
		{
			global::CoordCondition[y - 1][x - 1] = Condition::DrawingLineH;
		}
		if (global::oldDirection == Direction::Up && global::curDirection == Direction::Left ||
			global::oldDirection == Direction::Right && global::curDirection == Direction::Down)
		{
			global::CoordCondition[y - 1][x - 1] = Condition::DrawingLineUL;
		}
		if (global::oldDirection == Direction::Up && global::curDirection == Direction::Right ||
			global::oldDirection == Direction::Left && global::curDirection == Direction::Down)
		{
			global::CoordCondition[y - 1][x - 1] = Condition::DrawingLineUR;
		}
		if (global::oldDirection == Direction::Down && global::curDirection == Direction::Left ||
			global::oldDirection == Direction::Right && global::curDirection == Direction::Up)
		{
			global::CoordCondition[y - 1][x - 1] = Condition::DrawingLineDL;
		}
		if (global::oldDirection == Direction::Down && global::curDirection == Direction::Right ||
			global::oldDirection == Direction::Left && global::curDirection == Direction::Up)
		{
			global::CoordCondition[y - 1][x - 1] = Condition::DrawingLineDR;
		}
	}
}
void FillCheck(int x, int y, int* count)
{
	if (y - 1 < 0 || y - 1 > global::height - 1 || x - 1 < 0 || x - 1 > global::width - 1)
	{
		return;
	}
	if (global::CoordCondition[y - 1][x - 1] != Condition::Blank)
	{
		return;
	}
	global::CoordCondition[y - 1][x - 1] = -2;
	(*count)++;
	FillCheck(x + 1, y, count);
	FillCheck(x - 1, y, count);
	FillCheck(x, y + 1, count);
	FillCheck(x, y - 1, count);
}
COORD2 LandSizeCheck()
{
	COORD2 point = { 0, 0, 0, 0 };
	int size1, size2;
	int Count[4] = { 0, };
	int* pCount[4];
	pCount[0] = &Count[0];
	pCount[1] = &Count[1];
	FillCheck(global::drawEndPos.A.X, global::drawEndPos.A.Y, pCount[0]);
	FillCheck(global::drawEndPos.B.X, global::drawEndPos.B.Y, pCount[1]);
	for (int i = 0; i < global::height; i++)
	{
		for (int j = 0; j < global::width; j++)
		{
			if (global::CoordCondition[i][j] == -2)
			{
				global::CoordCondition[i][j] = 0;
			}
		}
	}
	int Count3 = 0, Count4 = 0;
	pCount[2] = &Count[2];
	pCount[3] = &Count[3];
	FillCheck(global::drawStartPos.A.X, global::drawStartPos.A.Y, pCount[2]);
	FillCheck(global::drawStartPos.B.X, global::drawStartPos.B.Y, pCount[3]);
	for (int i = 0; i < global::height; i++)
	{
		for (int j = 0; j < global::width; j++)
		{
			if (global::CoordCondition[i][j] == -2)
			{
				global::CoordCondition[i][j] = 0;
			}
		}
	}
	size1 = Count[0] + Count[2];
	size2 = Count[1] + Count[3];
	if (size1 > size2)
	{
		point.A = global::drawEndPos.B;
		point.B = global::drawStartPos.B;
	}
	else
	{
		point.A = global::drawEndPos.A;
		point.B = global::drawStartPos.A;
	}

	return point;
}
void FillArea(int x, int y)
{
	if (y - 1 < 0 || y - 1 > global::height - 1 || x - 1 < 0 || x - 1 > global::width - 1)
	{
		return;
	}
	if (global::CoordCondition[y - 1][x - 1] != Condition::Blank)
	{
		return;
	}
	global::CoordCondition[y - 1][x - 1] = Condition::Land;
	FillArea(x + 1, y);
	FillArea(x - 1, y);
	FillArea(x, y + 1);
	FillArea(x, y - 1);
}
void DrawingStartPointCheck()
{
	//(global::oldDirection == Direction::Stop && global::curDirection != Direction::Stop)
	if ((global::BlankMove == true && global::TempBlankMove == false))
	{
		COORD Area1 = { global::curPlayerPos.X ,global::curPlayerPos.Y };
		COORD Area2 = { global::curPlayerPos.X ,global::curPlayerPos.Y };
		if (global::curDirection == Direction::Up)
		{
			Area1.X--;
			Area2.X++;
			Area1.Y++;
			Area2.Y++;
		}
		else if (global::curDirection == Direction::Down)
		{
			Area1.X++;
			Area2.X--;
			Area1.Y--;
			Area2.Y--;
		}
		else if (global::curDirection == Direction::Left)
		{
			Area1.X++;
			Area2.X++;
			Area1.Y++;
			Area2.Y--;
		}
		else if (global::curDirection == Direction::Right)
		{
			Area1.X--;
			Area2.X--;
			Area1.Y--;
			Area2.Y++;
		}
		global::drawStartPos.A = Area1;
		global::drawStartPos.B = Area2;
	}
}
void DrawLand();
void DrawingEndPointCheck()
{
	//(global::oldDirection != Direction::Stop && global::curDirection == Direction::Stop)
	if ((global::BlankMove == false && global::TempBlankMove == true) &&
		(CheckcurPlayerCondition() == Condition::Line ||
			CheckcurPlayerCondition() == Condition::LineV ||
			CheckcurPlayerCondition() == Condition::LineH ||
			CheckcurPlayerCondition() == Condition::LineUR ||
			CheckcurPlayerCondition() == Condition::LineUL ||
			CheckcurPlayerCondition() == Condition::LineDR ||
			CheckcurPlayerCondition() == Condition::LineDL))
	{
		COORD Area1 = { global::curPlayerPos.X ,global::curPlayerPos.Y };
		COORD Area2 = { global::curPlayerPos.X ,global::curPlayerPos.Y };
		if (global::oldDirection == Direction::Up)
		{
			Area1.X--;
			Area2.X++;
			Area1.Y++;
			Area2.Y++;
		}
		else if (global::oldDirection == Direction::Down)
		{
			Area1.X++;
			Area2.X--;
			Area1.Y--;
			Area2.Y--;
		}
		else if (global::oldDirection == Direction::Left)
		{
			Area1.X++;
			Area2.X++;
			Area1.Y++;
			Area2.Y--;
		}
		else if (global::oldDirection == Direction::Right)
		{
			Area1.X--;
			Area2.X--;
			Area1.Y--;
			Area2.Y++;
		}
		global::drawEndPos.A = Area1;
		global::drawEndPos.B = Area2;
		DrawLand();
	}
}
void UpdatePlayerPosition()
{
	if (global::input::IsEscapeCmdOn())
	{
		global::input::Set(global::input::ESCAPE_KEY_INDEX, false);
		//TODO : 게임화면 전환이나 종료 처리 
		return;
	}
	CheckDrawDeath();
	if (CheckcurPlayerCondition() == Condition::Blank ||
		CheckcurPlayerCondition() == Condition::DrawingLineV ||
		CheckcurPlayerCondition() == Condition::DrawingLineH ||
		CheckcurPlayerCondition() == Condition::DrawingLineUR ||
		CheckcurPlayerCondition() == Condition::DrawingLineUL ||
		CheckcurPlayerCondition() == Condition::DrawingLineDR ||
		CheckcurPlayerCondition() == Condition::DrawingLineDL)
	{
		global::BlankMove = true;
		Linestop();
		if (global::curDirection == Direction::Up)
		{
			global::input::Set(global::input::USER_CMD_UP, false);
			global::curPlayerPos.Y--;
			Clamp(global::curPlayerPos.Y, global::playerMoveableRect.Top, global::playerMoveableRect.Bottom);
		}

		if (global::curDirection == Direction::Down)
		{
			global::input::Set(global::input::USER_CMD_UP, false);
			global::curPlayerPos.Y++;
			Clamp(global::curPlayerPos.Y, global::playerMoveableRect.Top, global::playerMoveableRect.Bottom);
		}

		if (global::curDirection == Direction::Left)
		{
			global::input::Set(global::input::USER_CMD_UP, false);
			global::curPlayerPos.X--;
			Clamp(global::curPlayerPos.X, global::playerMoveableRect.Left, global::playerMoveableRect.Right);
		}

		if (global::curDirection == Direction::Right)
		{
			global::input::Set(global::input::USER_CMD_UP, false);
			global::curPlayerPos.X++;
			Clamp(global::curPlayerPos.X, global::playerMoveableRect.Left, global::playerMoveableRect.Right);
		}
		DrawingStartPointCheck();
		DrawingEndPointCheck();
		global::TempBlankMove = global::BlankMove;
	}
	else if (CheckcurPlayerCondition() == Condition::Line ||
		CheckcurPlayerCondition() == Condition::LineV ||
		CheckcurPlayerCondition() == Condition::LineH ||
		CheckcurPlayerCondition() == Condition::LineUR ||
		CheckcurPlayerCondition() == Condition::LineUL ||
		CheckcurPlayerCondition() == Condition::LineDR ||
		CheckcurPlayerCondition() == Condition::LineDL)
	{
		global::BlankMove = false;
		Linestop();
		if (global::curDirection == Direction::Up && global::inputKey == true)
		{
			global::input::Set(global::input::USER_CMD_UP, false);
			global::curPlayerPos.Y--;
			Clamp(global::curPlayerPos.Y, global::playerMoveableRect.Top, global::playerMoveableRect.Bottom);
		}

		if (global::curDirection == Direction::Down && global::inputKey == true)
		{
			global::input::Set(global::input::USER_CMD_DOWN, false);
			global::curPlayerPos.Y++;
			Clamp(global::curPlayerPos.Y, global::playerMoveableRect.Top, global::playerMoveableRect.Bottom);
		}

		if (global::curDirection == Direction::Left && global::inputKey == true)
		{
			global::input::Set(global::input::USER_CMD_LEFT, false);
			global::curPlayerPos.X--;
			Clamp(global::curPlayerPos.X, global::playerMoveableRect.Left, global::playerMoveableRect.Right);
		}

		if (global::curDirection == Direction::Right && global::inputKey == true)
		{
			global::input::Set(global::input::USER_CMD_RIGHT, false);
			global::curPlayerPos.X++;
			Clamp(global::curPlayerPos.X, global::playerMoveableRect.Left, global::playerMoveableRect.Right);
		}
		DrawingStartPointCheck();
		DrawingEndPointCheck();
		global::TempBlankMove = global::BlankMove;
	}
}
void ConvertLine()
{
	for (int i = 0; i < global::height; i++)
	{
		for (int j = 0; j < global::width; j++)
		{
			if (global::CoordCondition[i][j] == Condition::DrawingLineV)
			{
				global::CoordCondition[i][j] = Condition::LineV;
			}
			if (global::CoordCondition[i][j] == Condition::DrawingLineH)
			{
				global::CoordCondition[i][j] = Condition::LineH;
			}
			if (global::CoordCondition[i][j] == Condition::DrawingLineUL)
			{
				global::CoordCondition[i][j] = Condition::LineUL;
			}
			if (global::CoordCondition[i][j] == Condition::DrawingLineUR)
			{
				global::CoordCondition[i][j] = Condition::LineUR;
			}
			if (global::CoordCondition[i][j] == Condition::DrawingLineDL)
			{
				global::CoordCondition[i][j] = Condition::LineDL;
			}
			if (global::CoordCondition[i][j] == Condition::DrawingLineDR)
			{
				global::CoordCondition[i][j] = Condition::LineDR;
			}
		}
	}
}
void DrawLand()
{
	COORD2 Point = LandSizeCheck();
	FillArea(Point.A.X, Point.A.Y);
	FillArea(Point.B.X, Point.B.Y);
	ConvertLine();
}
void UpdatePlayer()
{
	static ULONGLONG elapsedTime;

	elapsedTime += global::time::GetDeltaTime();

	while (elapsedTime >= global::playerMoveSpeed)
	{
		global::oldDirection = global::curDirection;
		global::input::UpdateInput();
		DrawLine();
		UpdatePlayerPosition();
		elapsedTime -= global::playerMoveSpeed;
	}
}
void Testrender()
{
	render::GotoXY(global::playerMoveableRect.Left, global::playerMoveableRect.Top);
	for (int i = 0; i < global::height; i++)
	{
		render::GotoXY(global::playerMoveableRect.Left, global::playerMoveableRect.Top + i);
		for (int j = 0; j < global::width; j++)
		{
			printf("%d", global::CoordCondition[i][j]);
		}
		printf("\n");
	}

}

void CreateEnemy()
{
	for (int i = 0; i < 2 + global::Stage * 3; i++)
	{
		ENEMY enemy;
		enemy.x = rand() % (global::width - 2);
		enemy.y = rand() % (global::height - 2);
		enemy.dx = (rand() % 2 == 0) ? -1 : 1;
		enemy.dy = (rand() % 2 == 0) ? -1 : 1;
		global::enemies[i] = enemy;
	}
}
void MoveEnemy()
{
	for (int i = 0; i < 2 + global::Stage * 3; i++)
	{
		global::enemies[i].x += global::enemies[i].dx;
		global::enemies[i].y += global::enemies[i].dy;

		if (global::enemies[i].x < 0 ||
			global::enemies[i].x >= global::width - 2 ||
			global::CoordCondition[global::enemies[i].y + 1][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::LineV ||
			global::CoordCondition[global::enemies[i].y + 1][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::Land)
		{
			global::enemies[i].dx = -global::enemies[i].dx;
		}
		if (global::enemies[i].y < 0 ||
			global::enemies[i].y >= global::height - 2 ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][global::enemies[i].x + 1] == Condition::LineH ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][global::enemies[i].x + 1] == Condition::Land)
		{
			global::enemies[i].dy = -global::enemies[i].dy;
		}
		if (global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::LineUR ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::LineUL ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::LineDR ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::LineDL)
		{
			global::enemies[i].dx = -global::enemies[i].dx;
			global::enemies[i].dy = -global::enemies[i].dy;
		}

		if (global::CoordCondition[global::enemies[i].y + 1][(global::enemies[i].x + 1) + global::enemies[i].dx] == DrawingLineV ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][global::enemies[i].x + 1] == DrawingLineH ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::DrawingLineUR ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::DrawingLineUL ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::DrawingLineDR ||
			global::CoordCondition[(global::enemies[i].y + 1) + global::enemies[i].dy][(global::enemies[i].x + 1) + global::enemies[i].dx] == Condition::DrawingLineDL)
		{
			global::GameRun = false;
			global::Dead = true;
		}
	}
}
void UpdateEnemy()
{
	static ULONGLONG elapsedTime;

	elapsedTime += global::time::GetDeltaTime();

	while (elapsedTime >= global::enemyMoveSpeed)
	{
		MoveEnemy();
		elapsedTime -= global::enemyMoveSpeed;
	}
}
void DrawEnemy()
{
	for (int i = 0; i < 2 + global::Stage * 3; i++)
	{
		render::ScreenDraw(global::enemies[i].x + 2, global::enemies[i].y + 2, "X");
	}
}

void GameArraySet()
{
	global::width = global::playerMoveableRect.Right - global::playerMoveableRect.Left + 1;
	global::height = global::playerMoveableRect.Bottom - global::playerMoveableRect.Top + 1;
	//global::colors = new WORD[global::height * global::width];

	global::CoordCondition = (short**)malloc(sizeof(short*) * global::height);

	for (int i = 0; i < global::height; i++)
	{
		global::CoordCondition[i] = (short*)malloc(sizeof(short) * global::width);
	}

	for (int i = 0; i < global::height; i++)
	{
		for (int j = 0; j < global::width; j++)
		{
			global::CoordCondition[i][j] = Condition::Blank;
		}
	}

	for (int i = 0; i < global::height; i++)
	{
		for (int j = 0; j < global::width; j++)
		{
			if (i == 0 || i == global::height - 1 || j == 0 || j == global::width - 1)
				global::CoordCondition[i][j] = Condition::Line;
		}
	}
	global::bMapprepared = true;
}
void ArrayReset();
void StartGame()
{
	global::GameRun = true;
	render::InitScreen();
	global::playerMoveableRect = render::GetPlayerMoveableRect();
	//ArrayReset();
	GameArraySet();
	CreateEnemy();
	render::DrawBorder(); // 벽을 그려 놓자!
	global::curPlayerPos.X = global::playerMoveableRect.Left + (global::playerMoveableRect.Left + global::playerMoveableRect.Right) / 2;
	global::curPlayerPos.Y = global::playerMoveableRect.Bottom;
	DrawPlayer();
	//Testrender();

}
void ArrayReset()
{
	for (int i = 1; i < global::height - 1; i++)
	{
		for (int j = 1; j < global::width - 1; j++)
		{
			global::CoordCondition[i][j] = Condition::Blank;
		}
	}
}
void EndGame()
{
	for (int i = 0; i < global::height; i++)
	{
		free(global::CoordCondition[i]);
	}
	free(global::CoordCondition);

	//delete[] global::colors;

	render::ScreenRelease();
}
void PrintPersent();
void PrintStage();
void DrawEnemy();
void Render()
{
	render::ScreenClear();
	PrintPersent();
	PrintStage();
	render::DrawBorder();
	DrawPlayer();
	DrawEnemy();
	// WriteConsoleOutputAttribute(render::hScreen[render::bScreenIndex], global::colors, global::width * global::height, { 0, 0 }, &dw);
	render::ScreenFlipping();
}
bool IsGameRun()
{
	return global::GameRun;
}
void FixeUpdate()
{
	static ULONGLONG elapsedTime;

	elapsedTime += global::time::GetDeltaTime();

	while (elapsedTime >= 1000) //0.02초
	{
		global::time::fixedUpdateCount += 1;

		elapsedTime -= 1000;
	}
}
void StageClear();
void Update()
{

	StageClear();

}
void PrintStage()
{
	char buffer[50] = "";
	sprintf_s(buffer, "Stage: %d", global::Stage);
	render::ScreenDraw(45, 0, buffer);

}
void PrintPersent()
{
	char buffer[50] = "";
	sprintf_s(buffer, "Clear: %d%% / 70%%", global::MyAreaPersent);
	render::ScreenDraw(55, 0, buffer);
}
void StageClear()
{
	int MyArea = 0;
	for (int i = 0; i < global::height; i++)
	{
		for (int j = 0; j < global::width; j++)
		{
			if (global::CoordCondition[i][j] == Condition::LineV ||
				global::CoordCondition[i][j] == Condition::LineH ||
				global::CoordCondition[i][j] == Condition::LineUR ||
				global::CoordCondition[i][j] == Condition::LineUL ||
				global::CoordCondition[i][j] == Condition::LineDR ||
				global::CoordCondition[i][j] == Condition::LineDL ||
				global::CoordCondition[i][j] == Condition::Land)
			{
				MyArea++;
			}
		}
	}
	global::MyAreaPersent = ((double)MyArea / (global::width * global::height)) * 100;
	if (global::MyAreaPersent >= global::WinPersent)
	{
		global::Stage++;
		global::GameRun = false;
		global::MyAreaPersent = 0;
	}
}
int RandomColor()
{
	int Color;
	Color = (rand() % 14) + 1;
	return Color;
}
int main()
{

	global::time::InitTime();
	srand((unsigned int)time(NULL));
	while (true)
	{
		DrawTitle();
		if (global::Start == 1)
		{
			while (!global::Dead)
			{
				textColor(RandomColor(), BLACK);
				StartGame();
				while (IsGameRun())
				{
					global::time::UpdateTime();
					FixeUpdate();
					Update();
					UpdatePlayer();
					UpdateEnemy();
					Render();
				}
				if (global::Dead == true)
				{
					DrawFailed();
					EndGame();
					global::TempBlankMove = false;
				}
				else if (global::Dead == false)
				{
					DrawClear();
				}
			}
		}
		else if (global::Start == 2)
		{
			DrawManual();
		}
		else if (global::Start == 3)
		{
			break;
		}
	}
	return 0;
}