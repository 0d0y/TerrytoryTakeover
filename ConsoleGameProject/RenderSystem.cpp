#include "RenderSystem.h"
int RandomColor();

namespace render
{
	bool bScreenIndex;
	HANDLE hScreen[2];

	SMALL_RECT consoleScreenSize;
	INT consoleScreenX;
	INT consoleScreenY;

	SMALL_RECT updateScreenSize;
	INT updateScreenX;
	INT updateScreenY;


	SMALL_RECT GetPlayerMoveableRect()
	{
		return updateScreenSize;
	}

	HANDLE GetScreenHandle()
	{
		int index = (bScreenIndex ? 1 : 0);

		return hScreen[index];
	}

	void GotoXY(int x, int y)
	{
		COORD Cur = { x, y };

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}

	void InitScreen()
	{
		// 화면 버퍼 2개를 만든다.
		hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

		// 깜박이는 커서를 좀 진정 시키자.
		CONSOLE_CURSOR_INFO cursorInfo = { 0, };
		cursorInfo.bVisible = 0; // 커서를 보일지 말지 결정(0이면 안보임, 0제외 숫자 값이면 보임)
		cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)
		SetConsoleCursorInfo(hScreen[0], &cursorInfo);
		SetConsoleCursorInfo(hScreen[1], &cursorInfo);


		// 콘솔창 크기를 가져 오자.
		// 지정할 수도 있는데 학습 용으로 그냥, 가져오는 예제로 작성.
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

		consoleScreenSize.Left = csbi.srWindow.Left;
		consoleScreenSize.Right = csbi.srWindow.Right;
		consoleScreenSize.Bottom = csbi.srWindow.Bottom;
		consoleScreenSize.Top = csbi.srWindow.Top;

		consoleScreenX = csbi.dwSize.X;
		consoleScreenY = csbi.dwSize.Y;

		// 실제 갱신할 화면 영역을 지정하자. 콘솔 크기 안쪽 사각형이라고 생각하면 됩니다.
		updateScreenSize.Left = consoleScreenSize.Left + 1;
		updateScreenSize.Right = consoleScreenSize.Right - 1;
		updateScreenSize.Bottom = consoleScreenSize.Bottom - 1;
		updateScreenSize.Top = consoleScreenSize.Top + 1;

		updateScreenX = consoleScreenX - 2;
		updateScreenY = consoleScreenY - 2;
	}

	void ScreenFlipping()
	{
		SetConsoleActiveScreenBuffer(GetScreenHandle());
		bScreenIndex = !bScreenIndex;
	}
	//void ScreenClear()
	//{
	//	COORD Coor = { updateScreenSize.Left, updateScreenSize.Top };
	//	for (int i = 0; i < global::height; i++)
	//	{
	//		for (int j = 0; j < global::width; j++)
	//		{
	//			ScreenDraw(Coor.X + j, Coor.Y + i, ' ');
	//		}
	//	}

	//}

	void ScreenClear()
	{
		COORD Coor = { updateScreenSize.Left, updateScreenSize.Top };
		DWORD dw;

		for (int y = 0; y < updateScreenY; y++)
		{
			Coor.Y = updateScreenSize.Top + y;

			FillConsoleOutputCharacter(GetScreenHandle(), ' ', updateScreenX, Coor, &dw);
		}
	}

	void ScreenRelease()
	{
		CloseHandle(hScreen[0]);
		CloseHandle(hScreen[1]);
	}

	void ScreenDraw(int x, int y, const char c)
	{
		DWORD dw;
		COORD Cur = { x, y };
		char buffer[10];
		sprintf_s(buffer, "%c", c);

		SetConsoleCursorPosition(GetScreenHandle(), Cur);

		WriteFile(GetScreenHandle(), buffer, 1, &dw, NULL);
	}

	void ScreenDraw2(int x, int y, const char* pStr)
	{
		DWORD dw;
		COORD Cur = { x, y };

		SetConsoleCursorPosition(hScreen[0], Cur);
		SetConsoleCursorPosition(hScreen[1], Cur);

		WriteFile(hScreen[0], pStr, strlen(pStr), &dw, NULL);
		WriteFile(hScreen[1], pStr, strlen(pStr), &dw, NULL);
	}


	void ScreenDraw(int x, int y, const char* pStr)
	{
		DWORD dw;
		COORD Cur = { x, y };

		SetConsoleCursorPosition(GetScreenHandle(), Cur);

		WriteFile(GetScreenHandle(), pStr, strlen(pStr), &dw, NULL);
	}


	void DrawBorder()
	{
		// 위쪽 라인. Y 값이 고정 된다.
		for (int x = updateScreenSize.Left; x < updateScreenSize.Right + 1; x++)
		{
			ScreenDraw(x, updateScreenSize.Top, "─");
		}

		// 아래쪽 라인. Y 값이 고정 된다.
		for (int x = updateScreenSize.Left; x < updateScreenSize.Right + 1; x++)
		{
			ScreenDraw(x, updateScreenSize.Bottom, "─");
		}

		// 왼쪽 라인, X 값이 고정 된다.
		for (int y = updateScreenSize.Top; y < updateScreenSize.Bottom + 1; y++)
		{
			ScreenDraw(updateScreenSize.Left, y, "│");
		}

		// 오른쪽 라인, X 값이 고정 된다.
		for (int y = updateScreenSize.Top; y < updateScreenSize.Bottom + 1; y++)
		{
			ScreenDraw(updateScreenSize.Right, y, "│");
		}
		ScreenDraw(updateScreenSize.Left, updateScreenSize.Top, "┌");
		ScreenDraw(updateScreenSize.Right, updateScreenSize.Top, "┐");
		ScreenDraw(updateScreenSize.Left, updateScreenSize.Bottom, "└");
		ScreenDraw(updateScreenSize.Right, updateScreenSize.Bottom, "┘");


		for (int i = 0; i < global::height; i++)
		{
			for (int j = 0; j < global::width; j++)
			{
				if (global::CoordCondition[i][j] == Condition::DrawingLineV ||
					global::CoordCondition[i][j] == Condition::LineV)
				{
					ScreenDraw(updateScreenSize.Left + j, updateScreenSize.Top + i, "┃");
				}
				if (global::CoordCondition[i][j] == Condition::DrawingLineH ||
					global::CoordCondition[i][j] == Condition::LineH)
				{
					ScreenDraw(updateScreenSize.Left + j, updateScreenSize.Top + i, "━");
				}
				if (global::CoordCondition[i][j] == Condition::DrawingLineUL ||
					global::CoordCondition[i][j] == Condition::LineUL)
				{
					ScreenDraw(updateScreenSize.Left + j, updateScreenSize.Top + i, "┓");
				}
				if (global::CoordCondition[i][j] == Condition::DrawingLineUR ||
					global::CoordCondition[i][j] == Condition::LineUR)
				{
					ScreenDraw(updateScreenSize.Left + j, updateScreenSize.Top + i, "┏");
				}
				if (global::CoordCondition[i][j] == Condition::DrawingLineDL ||
					global::CoordCondition[i][j] == Condition::LineDL)
				{
					ScreenDraw(updateScreenSize.Left + j, updateScreenSize.Top + i, "┛");
				}
				if (global::CoordCondition[i][j] == Condition::DrawingLineDR ||
					global::CoordCondition[i][j] == Condition::LineDR)
				{
					ScreenDraw(updateScreenSize.Left + j, updateScreenSize.Top + i, "┗");
				}
				if (global::CoordCondition[i][j] == Condition::Land)
				{
					ScreenDraw(updateScreenSize.Left + j, updateScreenSize.Top + i, "0");
				}

			}
		}

	}
};


void textColor(int foreColor, int backColor)
{
	int color = foreColor + backColor * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	SetConsoleTextAttribute(render::hScreen[0], color);
	SetConsoleTextAttribute(render::hScreen[1], color);
}

void drawText(int x, int y, const char* text, COLOR fore = WHITE, COLOR back = BLACK)
{
	render::GotoXY(x, y);
	textColor(fore, back);
	std::cout << text;
}

void DrawTitle()
{
	global::Stage = 1;
	global::Dead = false;
	render::InitScreen();
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	std::string clear1 = R"(


	    _|_|_|_|_|  _|_|_|_|  _|_|_|    _|_|_|    _|      _|  _|_|_|_|_|    _|_|    _|_|_|    _|      _|
		_|      _|        _|    _|  _|    _|    _|  _|        _|      _|    _|  _|    _|    _|  _|
		_|      _|_|_|    _|_|_|    _|_|_|        _|          _|      _|    _|  _|_|_|        _|
		_|      _|        _|    _|  _|    _|      _|          _|      _|    _|  _|    _|      _|
		_|      _|_|_|_|  _|    _|  _|    _|      _|          _|        _|_|    _|    _|      _|
)";

	std::string clear2 = R"(
	          _|_|_|_|_|    _|_|    _|    _|  _|_|_|_|    _|_|    _|      _|  _|_|_|_|  _|_|_|   
		      _|      _|    _|  _|  _|    _|        _|    _|  _|      _|  _|        _|    _|  
		      _|      _|_|_|_|  _|_|      _|_|_|    _|    _|  _|      _|  _|_|_|    _|_|_|   
		      _|      _|    _|  _|  _|    _|        _|    _|    _|  _|    _|        _|    _|  
		      _|      _|    _|  _|    _|  _|_|_|_|    _|_|        _|      _|_|_|_|  _|    _|    
)";

	int x = 55;
	int y = 20;
	int xStart = x;
	int yStart = y;






	while (true)
	{
		textColor(DARK_GREEN, BLACK);
		render::ScreenDraw(50, 1, clear1.c_str());
		textColor(VIOLET, BLACK);
		render::ScreenDraw(50, 10, clear2.c_str());
		textColor(SKYBLUE, BLACK);
		render::ScreenDraw(x, y, "게 임 시 작");
		render::ScreenDraw(x, y + 2, "게 임 설 명");
		render::ScreenDraw(x, y + 4, "게 임 종 료");
		textColor(DARK_BLUE, BLACK);
		render::ScreenDraw(x + 40, y, "KEY");
		render::ScreenDraw(x + 40, y + 2, "이동 - 방향키");
		render::ScreenDraw(x + 40, y + 4, "선택 - 스페이스바");
		textColor(DARK_RED, BLACK);
		render::ScreenDraw(xStart - 2, yStart, ">");

		if (GetAsyncKeyState(VK_UP) & 0x0001 && yStart > 20)
		{
			render::ScreenDraw(xStart - 2, yStart, " ");
			render::ScreenDraw(xStart - 2, ----yStart, ">");
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001 && yStart < 24)
		{
			render::ScreenDraw(xStart - 2, yStart, " ");
			render::ScreenDraw(xStart - 2, ++++yStart, ">");
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		{
			if (yStart == 20)
			{
				global::Start = 1;
				break;
			}
			else if (yStart == 22)
			{
				global::Start = 2;
				break;
			}
			else if (yStart == 24)
			{
				global::Start = 3;
				break;
			}
		}
		render::ScreenFlipping();
		render::ScreenClear();
	}
}


void DrawManual()
{
	render::ScreenClear();
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	textColor(GREEN, BLACK);
	std::string clear1 = R"(
	TERRYTORY TAKEOVER는 땅따먹기 게임입니다.

	선을 그려 공간을 만들면 내 영역이 됩니다. 

	테두리 영역과 내 영역에서는 이동을 멈출 수 있고, 

	선을 그리기 시작하면 그리기가 끝날 때까지 이동을 멈출 수 없습니다.

	공간을 만들어 영역이 되는 순간에 그 곳에 적이 있다면 그 적은 죽습니다.

	영역을 그리는 중에 선에 적이나 플레이어가 닿으면 패배합니다.

	영역을 70% 이상 점령하면 다음 스테이지로 넘어갑니다.

	스테이지가 올라갈수록 적의 수가 증가합니다. 

	몇 스테이지까지 갈 수 있는지 도전해보세요!

	버그가 좀 있습니다...

	타이틀 화면으로 돌아가려면 스페이스 바를 누르세요.
)";
	render::ScreenDraw(60, 3, clear1.c_str());
	render::ScreenFlipping();
	render::ScreenClear();

	while (true)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		{
			return;
		}
	}
}


void DrawFailed()
{
	render::ScreenClear();
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	textColor(RED, BLACK);
	std::string clear1 = R"(
                                                                                            
               8b        d8  ,ad8888ba,    88        88     88888888ba,    88  88888888888  88888888ba,    
                Y8,    ,8P  d8"'    `"8b   88        88     88      `"8b   88  88           88      `"8b   
                 Y8,  ,8P  d8'        `8b  88        88     88        `8b  88  88           88        `8b  
                  "8aa8"   88          88  88        88     88         88  88  88aaaaa      88         88  
                   `88'    88          88  88        88     88         88  88  88"""""      88         88  
                    88     Y8,        ,8P  88        88     88         8P  88  88           88         8P  
                    88      Y8a.    .a8P   Y8a.    .a8P     88      .a8P   88  88           88      .a8P   
                    88       `"Y8888Y"'     `"Y8888Y"'      88888888Y"'    88  88888888888  88888888Y"'    
                                                                                            
                                                                                            
)";

	std::string clear2 = R"(

				타이틀 화면으로 돌아가려면 스페이스 바를 누르세요.)";

	while (true)
	{
		render::ScreenDraw(0, 0, "                                                                                                 ");
		render::ScreenDraw2(55, 5, clear1.c_str());
		render::ScreenDraw2(50, 50, clear2.c_str());
		render::ScreenFlipping();
		render::ScreenClear();
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		{
			return;
		}
	}
}

void DrawClear()
{
	render::ScreenClear();
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);



	textColor(YELLOW, BLACK);
	std::string clear1 = R"(

		         ,ad8888ba,   88           88888888888         db         88888888ba   
		        d8"'    `"8b  88           88                 d88b        88      "8b  
		       d8'            88           88                d8'`8b       88      ,8P  
		       88             88           88aaaaa          d8'  `8b      88aaaaaa8P'  
		       88             88           88"""""         d8YaaaaY8b     88""""88'    
		       Y8,            88           88             d8""""""""8b    88    `8b    
		        Y8a.    .a8P  88           88            d8'        `8b   88     `8b   
		         `"Y8888Y"'   88888888888  88888888888  d8'          `8b  88      `8b  
	
)";


	std::string clear2 = R"(

				Clear! 다음 스테이지로 넘어가려면 스페이스바를 누르세요.)";



	while (true)
	{
		render::ScreenDraw(0, 0, "                                                                                                 ");
		render::ScreenDraw(55, 5, clear1.c_str());
		render::ScreenDraw(50, 50, clear2.c_str());
		render::ScreenFlipping();
		render::ScreenClear();
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		{
			return;
		}
	}

}

