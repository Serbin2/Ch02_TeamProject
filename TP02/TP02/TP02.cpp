//	Window 11 이상의 콘솔창은 콘솔 창 크기 강제 조절이 안됩니다.
//	콘솔 창의 설정에서 기본 시작 크기를 조정해서 게임플레이의 적당한 크기를 미리 설정하고 다시 실행해주세요.
//	260325 지호나

#include "Graphics/ConsoleGraphic.h"

// [2026-03-25, 박재현] 권한 테스트

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

	if (pGraphic == nullptr)
	{	//	엔진이 없습니다
		return 0;
	}

	//	그리기 테스트용
	CGraphic::Pixel pix = CGraphic::Pixel::circle;
	CGraphic::TextColor tex = static_cast<CGraphic::TextColor>(CGraphic::TextColor::TEXT_BACKGROUND_MAGENTA | CGraphic::TextColor::TEXT_FOREGROUND_CYAN);
	int x = 15;
	int y = 15;
	
	///	게임 루프
	while (1)
	{
		//	테스트 로직
		if (_kbhit())	//	이번 프레임에 입력이 있었는지 감지합니다.
		{	//	입력이 있었다면
			char input = _getch();	//	입력을 가져옵니다.
			
			switch (input)
			{	//	입력 내용을 확인하고 처리합니다.
			case 'w':
				y--;	//	*** 콘솔 그래픽은 4사분면((0, 0)의 위치가 좌상단)이므로 y축이 반대로 동작해야합니다. ***
				break;
			case 'a':
				x--;
				break;
			case 's':
				y++;
				break;
			case 'd':
				x++;
				break;
			}

			//	참고
			//← 좌측 방향키 : 75
			//
			//→ 우측 방향키 : 77
			//
			//↑ 위 방향키 : 72
			//
			//↓ 아래 방향키 : 80
			//
			//Enter키 : 13
		}

		pGraphic->StartDraw();


		//	테스트 그리기
		pGraphic->RenderToBuffer(x, y, pix, tex);


		pGraphic->EndDraw();
	}


	///////////////////////////////////////////////
	//	게임이 종료되었습니다. 종료처리를 작성해주세요
	///////////////////////////////////////////////
	
	CGraphic::Release();

	return 0;
}
