//	Window 11 이상의 콘솔창은 콘솔 창 크기 강제 조절이 안됩니다.
//	콘솔 창의 설정에서 기본 시작 크기를 조정해서 게임플레이의 적당한 크기를 미리 설정하고 다시 실행해주세요.
//	260325 지호나
#include "Standard.h"
#include "Graphics/ConsoleGraphic.h"
#include "Graphics//Interface.h"
#include "Input/Input.h"
#include "Time/Timer.h"
#include "InGameMenu/Menu.h"
#include "FrontMenu/MainMenu.h"
#include "World/GameWorld.h"
#include "Item/Item.h"
#include "Shop/Shop.h"
#include "Character/Player.h"
#include "FrontMenu/TextMenu.h"

#include "Manager/ResourceManager/ResourceManager.h"
#include "Manager/SoundManager/SoundManager.h"
#include "Resource/Sound/Sound.h"

//	게임 메인 루프
int Loop();
void LoadSound();

int main(){
	SetConsoleOutputCP(CP_UTF8);
	srand((unsigned int)time(NULL));

	///////////////////////////////////////////////
	//	여기서부터 게임 로직을 작성합니다
	///////////////////////////////////////////////

	// 사운드 테스트
	HWND hHwnd = GetConsoleWindow();
	GET_SINGLE(CResourceManager)->Init(hHwnd);
	GET_SINGLE(CSoundManager)->Init(hHwnd);

	LoadSound();

	bool bInitialized = false;
	int result = MAIN_MENU;
	while (1)
	{
		switch (result)
		{
		case MAIN_MENU:
		{
			GET_SINGLE(CSoundManager)->GetInstance()->PlayBGM(L"Main");
			CMainMenu menu;
			result = menu.vRun();
		}
		break;
		case GOTO_GAME:
			if (!bInitialized)
			{
				CTextMenu textMenu;
				textMenu.Intro();
				CGraphic::GetInstance()->Initialize();
				CGameWorld::GetInstance()->Initialize();
				bInitialized = true;
			}
			GET_SINGLE(CSoundManager)->GetInstance()->PlayBGM(L"Game");
			result = Loop();
			if (result != STORE_MENU)
			{
				CGraphic::Release();
				CInput::Release();
				CInterface::Release();
				CGameWorld::Release();
				CTimer::Release();
				bInitialized = false;
			}
			break;

		case GAME_CLEARED:
			//	게임 종료 화면 추가해야함
			CTextMenu textMenu;
			textMenu.Outro();
			result = MAIN_MENU;
			break;
		}

		if (result == EXIT_GAME)
		{
			break;
		}
	}

	CGraphic::Release();
	CInput::Release();
	CInterface::Release();
	CGameWorld::Release();
	CTimer::Release();
	GET_SINGLE(CResourceManager)->Clear();

	return 0;
}


int Loop()
{
	CTimer* Timer = CTimer::GetInstance();
	Timer->Start();
	CGraphic* pGraphic = CGraphic::GetInstance();
	CInput* pInput = CInput::GetInstance();
	CInterface* UI = CInterface::GetInstance();
	CGameWorld* World = CGameWorld::GetInstance();
	while (1)
	{
		int result = GOTO_GAME;
		double DeltaTime = Timer->Update();
		pInput->Update();

		if ((pInput->IsKeyDown(VK_ESCAPE)) || pInput->IsKeyDown('G'))
		{	//	게임 일시 정지
			Timer->Pause();
			//GET_SINGLE(CSoundManager)->StopBGM();
			GET_SINGLE(CSoundManager)->PlaySFX(L"Select");


			CMenu inGameMenu;
			int result = inGameMenu.ShowMenu();

			if (result == 1)
			{
				GET_SINGLE(CSoundManager)->RePlayBGM();
			}
			else if (result == 2)
			{
				GET_SINGLE(CSoundManager)->PlayBGM(L"Main");
				return MAIN_MENU;
			}
			else if (result == 3)
			{
				CShop shop;

				shared_ptr<CActor> actor = CGameWorld::GetInstance()->GetPlayerActor();
				CPlayer* player = static_cast<CPlayer*>(actor.get());

				if (player != nullptr)
				{
					shop.Enter(player);
				}
				//GET_SINGLE(CSoundManager)->RePlayBGM();
				pGraphic->ReDraw();
				Timer->Resume();
			}

			pGraphic->ReDraw();
			Timer->Resume();
		}

		// TAB키 → 바로 상점 진입 (일시정지)
		if (pInput->IsKeyDown(VK_TAB))
		{
			Timer->Pause();
			//GET_SINGLE(CSoundManager)->StopBGM();
			GET_SINGLE(CSoundManager)->PlaySFX(L"Select");
			CShop shop;
			shared_ptr<CActor> actor = CGameWorld::GetInstance()->GetPlayerActor();
			CPlayer* player = static_cast<CPlayer*>(actor.get());
			if (player != nullptr)
			{
				shop.Enter(player);
			}
			//GET_SINGLE(CSoundManager)->RePlayBGM();
			pGraphic->ReDraw();
			Timer->Resume();
		}

		result = World->Update(DeltaTime);
		if (result == MAIN_MENU)
		{
			GET_SINGLE(CSoundManager)->StopBGM();
			return MAIN_MENU;
		}
		if (result == GAME_CLEARED)
		{
			GET_SINGLE(CSoundManager)->StopBGM();
			return GAME_CLEARED;
		}
		//	그리기 시작
		//	각 액터의 Render를 이 함수 이후에 실행하세요
		pGraphic->StartDraw();

		World->Render();

		//	그리기 종료
		//	액터의 Render를 이 함수 이후에는 실행하지 마세요
		pGraphic->EndDraw();
	}
}

void LoadSound()
{
	// 로드 
	{
		// BGM
		GET_SINGLE(CResourceManager)->LoadSound(L"Game", L"BGM01.wav", ESoundType::BGM);
		GET_SINGLE(CResourceManager)->LoadSound(L"Main", L"Main.wav", ESoundType::BGM);
		GET_SINGLE(CResourceManager)->LoadSound(L"BossBGM", L"BossBGM.wav", ESoundType::BGM);

		// SFX
		// 기타
		GET_SINGLE(CResourceManager)->LoadSound(L"Boom", L"Boom.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"Select", L"Select.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"LevelUp", L"LevelUp.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"GunChange", L"GunChange.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"String", L"Key.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"LASER", L"LASER.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"Fall", L"Fall.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"Leave", L"Leave.wav", ESoundType::SFX);

		// 몬스터
		GET_SINGLE(CResourceManager)->LoadSound(L"Boss", L"Boss.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"Skeleton", L"Skeleton.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"Slime", L"Slime.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"Golem", L"Golem.wav", ESoundType::SFX);

		// 몬스터 사망
		GET_SINGLE(CResourceManager)->LoadSound(L"SkeletonDead", L"SkeletonDead.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"SlimeDead", L"SlimeDead.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"GolemDead", L"GolemDead.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"BossDead", L"BossDead.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"FBossDead", L"FBossDead.wav", ESoundType::SFX);
		GET_SINGLE(CResourceManager)->LoadSound(L"FBossLastHit", L"FBossLastHit.wav", ESoundType::SFX);
	}
}
