#include "InputSystem.h"

namespace global
{
	namespace input
	{
		bool inputKeyTable[MAX_KEY];

		// Helper Functions ..
		void Set(const int keyIdx, bool bOn)
		{
			assert(keyIdx >= 0 && keyIdx < MAX_KEY); // ��� �ڵ带 �ۼ��ϴ� �Ϳ� �ͼ������� �մϴ�.  �迭�� �ٷ� ���� �ε����� �����ؾ� �մϴ�!!

			inputKeyTable[keyIdx] = bOn;			// bool Ÿ���� Ȱ�� �սô�.
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

		// �Է� Ű���� ���� �˾� ���ô�. ��ŷ�� �ͺ�ŷ�� ���ؼ��� �����ϰ� ����� �մϴ�!!
		// * ��Ʈ ���꿡 ���� �н��մϴ�. & �� && �� �޶��. 
		// &, * �����ڰ� �ƶ��� ���� �ٸ��� �ؼ��Ǵ� �Ϳ� ���� �ؾ� �մϴ�. 


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
