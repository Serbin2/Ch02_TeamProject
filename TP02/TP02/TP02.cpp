//	Window 11 이상의 콘솔창은 콘솔 창 크기 강제 조절이 안됩니다.
//	콘솔 창의 설정에서 기본 시작 크기를 조정해서 게임플레이의 적당한 크기를 미리 설정하고 다시 실행해주세요.

#include "Graphics/ConsoleGraphic.h"

// [2026-03-25, 박재현] 권한 테스트

int main()
{
    std::cout << "Hello World!\n";

	CGraphic* graphinc = CGraphic::GetInstance();

	if (graphinc == nullptr)
	{
		return 0;
	}

	std::cout << "Engine roaded" << std::endl;

	return 0;
}
