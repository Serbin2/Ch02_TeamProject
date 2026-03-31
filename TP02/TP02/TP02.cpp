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
#include "Graphics/Effect.h"
#include "Shop/Shop.h"
#include "Character/Player.h"


// [2026-03-25, 박재현] 권한 테스트 2
#include "Manager/ResourceManager/ResourceManager.h"
#include "Manager/SoundManager/SoundManager.h"
#include "Resource/Sound/Sound.h"

//	게임 메인 루프
int Loop();

int main()
{
	//_CrtSetBreakAlloc(160);	//	메모리 누수 위치 확인용
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SetConsoleOutputCP(CP_UTF8);

	///////////////////////////////////////////////
	//	여기서부터 게임 로직을 작성합니다
	///////////////////////////////////////////////

	// 사운드 테스트
	HWND hHwnd = GetConsoleWindow();

	GET_SINGLE(CResourceManager)->Init(hHwnd);
	GET_SINGLE(CSoundManager)->Init(hHwnd);

	// 로드 
	GET_SINGLE(CResourceManager)->LoadSound(L"BGM1", L"DJ-Okawari-Flower-Dance-2010.wav", ESoundType::BGM);
	GET_SINGLE(CResourceManager)->LoadSound(L"BGM2", L"OST-Second-Run.wav", ESoundType::BGM);
	GET_SINGLE(CResourceManager)->LoadSound(L"BGM3", L"Pixel_Velocity.wav", ESoundType::BGM);
	GET_SINGLE(CResourceManager)->LoadSound(L"BGM4", L"BGM01.wav", ESoundType::BGM);
	GET_SINGLE(CResourceManager)->LoadSound(L"Select", L"Select.wav", ESoundType::SFX);
	GET_SINGLE(CResourceManager)->LoadSound(L"Skeleton", L"Skeleton.wav", ESoundType::SFX);
	GET_SINGLE(CResourceManager)->LoadSound(L"Slime", L"Slime.wav", ESoundType::SFX);
	GET_SINGLE(CResourceManager)->LoadSound(L"Golem", L"Golem.wav", ESoundType::SFX);
	{
		//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM1");
		//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM2");
	}
	//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM3");
	//pSound->Play(true);
	std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM4");
	pSound->Play(true);

	bool bInitialized = false;
	int result = MAIN_MENU;
	while (1)
	{
		switch (result)
		{
		case MAIN_MENU:
		{
			CMainMenu menu;
			result = menu.vRun();
		}
		break;
		case GOTO_GAME:
			if (!bInitialized)
			{
				CGraphic::GetInstance()->Initialize();
				CGameWorld::GetInstance()->Initialize();
				bInitialized = true;
			}
			result = Loop();
			if (result != STORE_MENU)
			{
				CGraphic::Release();
				CInput::Release();
				CInterface::Release();
				CGameWorld::Release();
				bInitialized = false;
			}
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

	_CrtDumpMemoryLeaks();	//	메모리 누수 감지
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
		double DeltaTime = Timer->Update();
		pInput->Update();

		if ((pInput->IsKeyDown(VK_ESCAPE)) || pInput->IsKeyDown('G'))
		{	//	게임 일시 정지
			Timer->Pause();

			//pSound->Stop();

			CMenu inGameMenu;
			int result = inGameMenu.ShowMenu();

			if (result == 1)
			{
				//pSound->Play();
			}
			else if (result == 2)
			{
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
			CShop shop;
			shared_ptr<CActor> actor = CGameWorld::GetInstance()->GetPlayerActor();
			CPlayer* player = static_cast<CPlayer*>(actor.get());
			if (player != nullptr)
			{
				shop.Enter(player);
			}
			pGraphic->ReDraw();
			Timer->Resume();
		}

		World->Update(DeltaTime);

		//	그리기 시작
		//	각 액터의 Render를 이 함수 이후에 실행하세요
		pGraphic->StartDraw();

		World->Render();

		//	그리기 종료
		//	액터의 Render를 이 함수 이후에는 실행하지 마세요
		pGraphic->EndDraw();
	}
}