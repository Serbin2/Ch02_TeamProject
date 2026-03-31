#include "Reward.h"
#include "../World/GameWorld.h"
#include "../Character/Player.h"
#include "../Projectile/TripleProjectile.h"
#include "../Projectile/BouncingProjectile.h"
#include "../Projectile/ExplosiveProjectile.h"
#include "../Projectile/HomingProjectile.h"
#include "../Projectile/SplitProjectile.h"

int CReward::GetReward()
{	//	임시 보상선택 창
	int in;
	while (_kbhit())
	{ // 버퍼에 입력이 있는 동안
		in = _getch();      // 문자를 가져와 버퍼에서 제거
	}

	HANDLE m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_CYAN);

	for (int i = 0; i < 10; i++)
	{
		if (i == 1)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_BLACK);
		if (i == 9)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_CYAN);
		SetConsoleCursorPosition(m_hOP, COORD(10, 10 + i));
	std::cout << "레벨업레벨업레벨업레벨업레벨업레벨업레벨업레벨업레벨업";
	}

	SetConsoleCursorPosition(m_hOP, COORD(15, 12));
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_BLACK);
	std::cout << "   >>>>>>>>   보상을 선택하시오 <<<<<<<<<<<<";
	SetConsoleCursorPosition(m_hOP, COORD(15, 15));
	std::cout << "   >>>> 1. 세갈래 총알   2. 튕기는 총알 <<<<";
	SetConsoleCursorPosition(m_hOP, COORD(15, 17));
	std::cout << "   >>>> 3. 유도탄  4. 폭발탄  5. 분할탄 <<<<";

	auto pPlayerActor = CGameWorld::GetInstance()->GetPlayerActor();
	auto pPlayer = std::dynamic_pointer_cast<CPlayer>(pPlayerActor);

	while (1)
	{
		while (_kbhit())
		{ // 버퍼에 입력이 있는 동안
			in = _getch();      // 문자를 가져와 버퍼에서 제거
		}
		in = _getch() - '0';

		switch (in)
		{
		case 1:
			if (pPlayer != nullptr)
				pPlayer->SetProjectile(std::make_shared<CTripleProjectile>());
			return 1;
		case 2:
			if (pPlayer != nullptr)
				pPlayer->SetProjectile(std::make_shared<CBouncingProjectile>());
			return 2;
		case 3:
			if (pPlayer != nullptr)
				pPlayer->SetProjectile(std::make_shared<CHomingProjectile>());
			return 2;
		case 4:
			if (pPlayer != nullptr)
				pPlayer->SetProjectile(std::make_shared<CExplosiveProjectile>());
			return 2;
		case 5:
			if (pPlayer != nullptr)
				pPlayer->SetProjectile(std::make_shared<CSplitProjectile>());
			return 2;
		}
	}
	return 0;
}

int CDead::Dead()
{
	int in;
	while (_kbhit())
	{ // 버퍼에 입력이 있는 동안
		in = _getch();      // 문자를 가져와 버퍼에서 제거
	}

	HANDLE m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_CYAN);

	for (int i = 0; i < 10; i++)
	{
		if (i == 1)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_BLACK);
		if (i == 9)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_CYAN);
		SetConsoleCursorPosition(m_hOP, COORD(10, 10 + i));
	std::cout << "사망사망사망사망사망사망사망사망사망사망사망사망사망사망사망";
	}

	SetConsoleCursorPosition(m_hOP, COORD(10, 12));
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_BLACK);
	std::cout << "        >>>>>>>>   사망했습니다! <<<<<<<<<<<<";
	SetConsoleCursorPosition(m_hOP, COORD(10, 14));
	std::cout << "              >>>> 로비로 나가기 <<<<";

	while (_kbhit())
	{ // 버퍼에 입력이 있는 동안
		in = _getch();      // 문자를 가져와 버퍼에서 제거
	}
	in = _getch() - '0';

	return 0;
}

int CWarning::Warning()
{
	int in;
	while (_kbhit())
	{ // 버퍼에 입력이 있는 동안
		in = _getch();      // 문자를 가져와 버퍼에서 제거
	}

	HANDLE m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_RED_INT | TEXT_BACKGROUND_RED_INT);

	for (int i = 0; i < 10; i++)
	{
		if (i == 1)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_BLACK);
		if (i == 9)	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_RED_INT | TEXT_BACKGROUND_RED_INT);
		SetConsoleCursorPosition(m_hOP, COORD(10, 10 + i));
	std::cout << "경고경고경고경고경고경고경고경고경고경고경고경고경고경고경고";
	}

	SetConsoleCursorPosition(m_hOP, COORD(10, 12));
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_YELLOW);
	std::cout << "    >>>>>>>>  강력한 울림이 느껴집니다!  <<<<<<<<<<<<";
	SetConsoleCursorPosition(m_hOP, COORD(10, 14));
	std::cout << "    >>>>>> 나는 다가오는 위험에 준비가 되었다! <<<<<<";
	SetConsoleCursorPosition(m_hOP, COORD(10, 18));
	std::cout << "       >>>>>>>> 아무 키나 눌러서 계속하기 <<<<<<<<";
	
	while (_kbhit())
	{ // 버퍼에 입력이 있는 동안
		in = _getch();      // 문자를 가져와 버퍼에서 제거
	}
	in = _getch() - '0';

	return 0;
}