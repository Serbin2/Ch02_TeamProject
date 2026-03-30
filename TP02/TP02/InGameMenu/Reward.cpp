#include "Reward.h"


int CReward::GetReward()
{	//	임시 보상선택 창
	HANDLE m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_CYAN);

	for (int i = 0; i < 10; i++)
	{
		if(i == 1)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_BLACK);
		if(i == 9)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_CYAN);
		SetConsoleCursorPosition(m_hOP, COORD(10, 10+i));
		std::cout << "레벨업레벨업레벨업레벨업레벨업레벨업레벨업레벨업레벨업";
	}

	SetConsoleCursorPosition(m_hOP, COORD(15, 12));
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_BLACK);
	std::cout << ">>>>>>>>   보상을 선택하시오 <<<<<<<<<<<<";
	SetConsoleCursorPosition(m_hOP, COORD(15, 14));
	std::cout << ">>>> 1. 세갈래 총알   2. 튕기는 총알 <<<<";
	while (1)
	{
		int in = _getch() - '0';

		switch (in)
		{
		case 1:
			return 1;
		case 2:
			return 2;
		}
	}
	return 0;
}