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

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "윈도우 10 이상의 콘솔창은 코드에서의 콘솔 창 크기 강제 조절이 동작하지 않을 수 있습니다.\n";
	std::cout << "게임을 시작하기 전에 콘솔 창 크기를 적절하게 조절 한 후 진행하시기 바랍니다.\n";
	std::cout << "아무 키를 눌러 계속합니다.\n";
	char in = _getch();

	CMainMenu menu;
	menu.vRun();


	///////////////////////////////////////////////
	//	여기서부터 게임 로직을 작성합니다
	///////////////////////////////////////////////
	CGraphic* pGraphic = CGraphic::GetInstance();
	CInput* pInput = CInput::GetInstance();
	CTimer Timer;
	CInterface* UI = CInterface::GetInstance();
	CGameWorld* World = CGameWorld::GetInstance();

	if (!pGraphic || !pInput) return 0;

	if (pInput == nullptr)
	{	//	입력이 없습니다
		return 0;
	}

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
	{
		//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM1");
		//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM2");
	}
	std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM4");
	pSound->Play(true);


	UI->AddUI(0, "FPS Count : ");
	UI->AddUI(1, "FPS : ");
	pGraphic->Initialize();
	World->Initialize();
	Timer.Start();

	//	이펙트 테스트용
	int testPosition = 10;
	while (1)
	{
		double DeltaTime = Timer.Update();
		UI->SetValue(0, Timer.GetFpsCount());
		UI->SetValue(1, Timer.GetFPS());
		pInput->Update();

		if ((pInput->IsKeyDown(VK_ESCAPE)) || pInput->IsKeyDown('G'))
		{	//	게임 일시 정지
			Timer.Pause();

			pSound->Stop();

			CMenu inGameMenu;
			int result = inGameMenu.ShowMenu();

			if (result == 1)
			{
				pSound->Play();
			}
			else if (result == 2)
			{
				break;
				// 게임 종료 → 메인메뉴로 복귀
				CGraphic::Release();
				CInput::Release();
				CMainMenu frontMenu;
				frontMenu.vRun();
				return 0;
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
				Timer.Resume();
			}

			pGraphic->FlushingBuffer();
			pGraphic->ReDraw();
			Timer.Resume();
		}

		// TAB키 → 바로 상점 진입 (일시정지)
		if (pInput->IsKeyDown(VK_TAB))
		{
			Timer.Pause();
			CShop shop;
			shared_ptr<CActor> actor = CGameWorld::GetInstance()->GetPlayerActor();
			CPlayer* player = static_cast<CPlayer*>(actor.get());
			if (player != nullptr)
			{
				shop.Enter(player);
			}
			pGraphic->ReDraw();
			Timer.Resume();
		}

#ifdef _DEBUG	//	이펙트 테스트용 
		if (pInput->IsKeyDown('O'))
		{	//	이펙트 테스트
			shared_ptr<CEffect> FX = make_shared<CEffect>();
			COORD LT;
			LT.X = 10;
			LT.Y = testPosition;
			COORD RB = { LT.X + 1, LT.Y + 2 };
			FX->CreateStaticEffect(Pixel::star, TEXT_FOREGROUND_RED | TEXT_BACKGROUND_YELLOW, LT, RB, 3.0);
			CGameWorld::GetInstance()->AddActor(FX);
			testPosition += 2;
		}

		if (pInput->IsKeyDown('P'))
		{
			shared_ptr<CEffect> FX = make_shared<CEffect>();
			vector<pair<int, int >> material;
			vector<pair<COORD, COORD>> shape;
			material.push_back(make_pair(Pixel::cross, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
			material.push_back(make_pair(Pixel::square, TEXT_FOREGROUND_RED_INT | TEXT_BACKGROUND_RED));
			material.push_back(make_pair(Pixel::cross, TEXT_FOREGROUND_RED | TEXT_BACKGROUND_YELLOW));
			material.push_back(make_pair(Pixel::dust, TEXT_FOREGROUND_YELLOW_INT | TEXT_BACKGROUND_YELLOW));
			shape.push_back(make_pair(COORD(22, 22), COORD(23, 23)));
			shape.push_back(make_pair(COORD(20, 20), COORD(25, 25)));
			shape.push_back(make_pair(COORD(21, 21), COORD(24, 24)));
			shape.push_back(make_pair(COORD(22, 22), COORD(23, 23)));
			vector<double> duration;
			duration.push_back(0.1);
			duration.push_back(0.5);
			duration.push_back(1.0);
			duration.push_back(1.0);
			FX->CreateDynamicEffect(4, material, shape, duration);
			CGameWorld::GetInstance()->AddActor(FX);
		}
#endif

		World->Update(DeltaTime);

		//	그리기 시작
		//	각 액터의 Render를 이 함수 이후에 실행하세요
		pGraphic->StartDraw();

		World->Render();

		//	그리기 종료
		//	액터의 Render를 이 함수 이후에는 실행하지 마세요
		pGraphic->EndDraw();
	}

	CGraphic::Release();
	CInput::Release();
	CInterface::Release();
	CGameWorld::Release();
	return 0;
}
