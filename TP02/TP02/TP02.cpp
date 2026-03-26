//	Window 11 이상의 콘솔창은 콘솔 창 크기 강제 조절이 안됩니다.
//	콘솔 창의 설정에서 기본 시작 크기를 조정해서 게임플레이의 적당한 크기를 미리 설정하고 다시 실행해주세요.
//	260325 지호나

#include "Graphics/ConsoleGraphic.h"
#include "Input/Input.h"
#include "Time/Timer.h"

#include "Graphics//Interface.h"
#include "Character/Player.h"
#include "Boss/Boss.h"


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

	///////////////////////////////////////////////
	//	여기서부터 게임 로직을 작성합니다
	///////////////////////////////////////////////

	///	게임에 필요한 객체들 준비 구간
	CGraphic* pGraphic = CGraphic::GetInstance();
	CInput* pInput = CInput::GetInstance();
	CTimer Timer;
	CInterface UI;

	if (pGraphic == nullptr)
	{	//	엔진이 없습니다
		return 0;
	}

	if (pInput == nullptr)
	{	//	입력이 없습니다
		return 0;
	}

	CPlayer* pPlayer = new CPlayer(Pixel::square, TEXT_BACKGROUND_MAGENTA | TEXT_FOREGROUND_CYAN);
	//std::shared_ptr<CBoss> pBoss = make_shared<CBoss>(Pixel::triangle, TEXT_BACKGROUND_BLACK | TEXT_BACKGROUND_BLUE_INT, FGridSize(2, 2));

	// 사운드 테스트
	{
		HWND hHwnd = GetConsoleWindow();

		GET_SINGLE(CResourceManager)->Init(hHwnd);
		GET_SINGLE(CSoundManager)->Init(hHwnd);

		// 로드 
		GET_SINGLE(CResourceManager)->LoadSound(L"BGM1", L"DJ-Okawari-Flower-Dance-2010.wav");
		GET_SINGLE(CResourceManager)->LoadSound(L"BGM2", L"OST-Second-Run.wav");
		{
			//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM1");
			//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM2");
			//pSound->Play(true);
		}
	}


	UI.AddUI(0, "FPS Count : ");
	UI.AddUI(1, "FPS : ");
	
	Timer.Start();

	///	게임 루프
	while (1)
	{
		double deltaTime = Timer.Update();
		UI.SetValue(0, Timer.GetFpsCount());
		UI.SetValue(1, Timer.GetFPS());
		pInput->Update();	//	입력 갱신
		//	테스트 로직

		if (pInput->IsKeyDown('W'))	y--;	//	*** 콘솔 그래픽은 4사분면((0, 0)의 위치가 좌상단)이므로 y축이 반대로 동작해야합니다. ***
		if (pInput->IsKeyDown('A')) x--;
		if (pInput->IsKeyDown('S')) y++;
		if (pInput->IsKeyDown('D')) x++;
		if (pInput->IsKeyDown('G')) pGraphic->AddLog("G키를 눌렀습니다.");
		if (pInput->IsKeyDown('F')) pGraphic->AddLog("F키를 눌렀을까요? 와랄랄라 와랄랄루.");
		if (pInput->IsKeyDown('T'))
		{
			pGraphic->AddLog("Toggle UI");
			if (UI.AddUI(0, "FPS Count : "))
			{
				UI.AddUI(1, "FPS :");
			}
			else
			{
				UI.RemoveUI(0);
				UI.RemoveUI(1);
			}
		}
		if (pInput->IsKeyDown('Y')) Timer.SetTargetFps(60);

		//	참고 https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		//← 좌측 방향키 : VK_LEFT
		//→ 우측 방향키 : VK_RIGTH
		//↑ 위 방향키: VK_UP
		//↓ 아래 방향키 : VK_DOWN
		//Enter키 : VK_RETURN

		double DeltaTime = Timer.Update();
		pInput->Update();	//	입력 갱신


		//	그리기 시작
		//	각 액터의 Render를 이 함수 이후에 실행하세요
		pGraphic->StartDraw();

		//	테스트 그리기
		// pGraphic->RenderToBuffer(x, y, pix, tex);
		pPlayer->Tick(DeltaTime);
		//pBoss->Tick(DeltaTime);

		//	그리기 종료
		//	액터의 Render를 이 함수 이후에는 실행하지 마세요
		pGraphic->EndDraw();

		//	종료 키 입력
		if (pInput->IsKeyDown(VK_ESCAPE)) break;
	}


	///////////////////////////////////////////////
	//	게임이 종료되었습니다. 종료처리를 작성해주세요
	///////////////////////////////////////////////

	CGraphic::Release();
	CInput::Release();

	return 0;
}
