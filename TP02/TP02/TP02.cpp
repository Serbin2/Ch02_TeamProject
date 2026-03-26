//	Window 11 이상의 콘솔창은 콘솔 창 크기 강제 조절이 안됩니다.
//	콘솔 창의 설정에서 기본 시작 크기를 조정해서 게임플레이의 적당한 크기를 미리 설정하고 다시 실행해주세요.
//	260325 지호나

#include "Graphics/ConsoleGraphic.h"
#include "Input/Input.h"

// [2026-03-25, 박재현] 권한 테스트 2

int main()
{
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

	if (pGraphic == nullptr)
	{	//	엔진이 없습니다
		return 0;
	}

	if (pInput == nullptr)
	{	//	입력이 없습니다
		return 0;
	}

	//	그리기 테스트용
	CGraphic::Pixel pix = CGraphic::Pixel::square;
	CGraphic::TextColor tex = static_cast<CGraphic::TextColor>(CGraphic::TextColor::TEXT_BACKGROUND_MAGENTA | CGraphic::TextColor::TEXT_FOREGROUND_CYAN);
	int x = 15;
	int y = 15;

	///	게임 루프
	while (1)
	{
		pInput->Update();	//	입력 갱신
		//	테스트 로직

		if (pInput->IsKeyDown('W'))	y--;	//	*** 콘솔 그래픽은 4사분면((0, 0)의 위치가 좌상단)이므로 y축이 반대로 동작해야합니다. ***
		if (pInput->IsKeyDown('A')) x--;
		if (pInput->IsKeyDown('S')) y++;
		if (pInput->IsKeyDown('D')) x++;
		if (pInput->IsKeyDown('G')) pGraphic->AddLog("G키를 눌렀습니다.");

		//	참고 https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		//← 좌측 방향키 : VK_LEFT
		//→ 우측 방향키 : VK_RIGTH
		//↑ 위 방향키: VK_UP
		//↓ 아래 방향키 : VK_DOWN
		//Enter키 : VK_RETURN

		//	그리기 시작
		//	각 액터의 Render를 이 함수 이후에 실행하세요
		pGraphic->StartDraw();



		//	테스트 그리기
		pGraphic->RenderToBuffer(x, y, pix, tex);


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
