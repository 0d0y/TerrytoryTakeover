#include "InputSystem.h"

namespace global
{
	namespace input
	{
		bool inputKeyTable[MAX_KEY];

		// Helper Functions ..
		void Set(const int keyIdx, bool bOn)
		{
			assert(keyIdx >= 0 && keyIdx < MAX_KEY); // 방어 코드를 작성하는 것에 익숙해져야 합니다.  배열을 다룰 때는 인덱스를 조심해야 합니다!!

			inputKeyTable[keyIdx] = bOn;			// bool 타입을 활용 합시다.
		}
		void Set(bool bOn)
		{
			inputKey = bOn;
		}

		bool IsEscapeCmdOn()
		{
			return inputKeyTable[ESCAPE_KEY_INDEX];
		}

		bool IsLeftCmdOn()
		{
			return inputKeyTable[USER_CMD_LEFT];
		}

		bool IsRightCmdOn()
		{
			return inputKeyTable[USER_CMD_RIGHT];
		}

		bool IsUpCmdOn()
		{
			return inputKeyTable[USER_CMD_UP];
		}

		bool IsDownCmdOn()
		{
			return inputKeyTable[USER_CMD_LEFT];
		}

		// 입력 키값에 대해 알아 봅시다. 블러킹과 넌블러킹에 대해서만 이해하고 가기로 합니다!!
		// * 비트 연산에 대해 학습합니다. & 와 && 은 달라요. 
		// &, * 연산자가 맥락에 따라 다르게 해석되는 것에 주의 해야 합니다. 


		int UpdateInput()
		{
			int tempDirection = global::curDirection;
			if (CheckcurPlayerCondition() == Condition::Line ||
				CheckcurPlayerCondition() == Condition::LineV ||
				CheckcurPlayerCondition() == Condition::LineH ||
				CheckcurPlayerCondition() == Condition::LineUR ||
				CheckcurPlayerCondition() == Condition::LineUL ||
				CheckcurPlayerCondition() == Condition::LineDR ||
				CheckcurPlayerCondition() == Condition::LineDL)
			{
				tempDirection = Direction::Stop;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				tempDirection = Direction::Left;
			}
			else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				tempDirection = Direction::Right;
			}
			else if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				tempDirection = Direction::Up;
			}
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				tempDirection = Direction::Down;
			}

			if (CheckcurPlayerCondition() == Condition::Blank)
			{
				if ((global::curDirection == Direction::Up && tempDirection == Direction::Down)
					|| (global::curDirection == Direction::Down && tempDirection == Direction::Up)
					|| (global::curDirection == Direction::Left && tempDirection == Direction::Right)
					|| (global::curDirection == Direction::Right && tempDirection == Direction::Left))
				{
				}
				else
				{
					global::curDirection = tempDirection;
				}
			}
			else if (CheckcurPlayerCondition() == Condition::Line ||
				CheckcurPlayerCondition() == Condition::LineV ||
				CheckcurPlayerCondition() == Condition::LineH ||
				CheckcurPlayerCondition() == Condition::LineUR ||
				CheckcurPlayerCondition() == Condition::LineUL ||
				CheckcurPlayerCondition() == Condition::LineDR ||
				CheckcurPlayerCondition() == Condition::LineDL)
			{
				global::curDirection = tempDirection;
			}

			if (GetAsyncKeyState(VK_UP) < 0 ||
				GetAsyncKeyState(VK_DOWN) < 0 ||
				GetAsyncKeyState(VK_LEFT) < 0 ||
				GetAsyncKeyState(VK_RIGHT) < 0)
			{
				inputKey = true;
			}
			else
			{
				inputKey = false;
			}
			return global::curDirection;
		}
	};
};
