//	Window 11 이상의 콘솔창은 콘솔 창 크기 강제 조절이 안됩니다.
//	콘솔 창의 설정에서 기본 시작 크기를 조정해서 게임플레이의 적당한 크기를 미리 설정하고 다시 실행해주세요.
//	260325 지호나

#include "Graphics/ConsoleGraphic.h"
#include "Player.h"

#include "Input/Input.h"
#include "Time/Timer.h"
#include "Character/Player.h"

// [2026-03-25, 박재현] 권한 테스트 2

int main()
{
	system("chcp 65001");
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

	/// player 만들기
	CPlayer* player = new CPlayer("강민서");
	
	player->ShowName();


	if (graphinc == nullptr)
	{
		return 0;
	}

	if (pInput == nullptr)
	{	//	입력이 없습니다
		return 0;
	}

	CPlayer* pPlayer = new CPlayer(Pixel::square, TEXT_BACKGROUND_MAGENTA | TEXT_FOREGROUND_CYAN);

	Timer.Start();
	///	게임 루프

	while (1)
	{
		player->ShowName();
		double deltaTime = Timer.Update();
		pInput->Update();	//	입력 갱신

		//	그리기 시작
		//	각 액터의 Render를 이 함수 이후에 실행하세요
		pGraphic->StartDraw();

		pPlayer->Tick(deltaTime);

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
