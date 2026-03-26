#pragma once

#include "../Standard.h"
#include "../Character/Actor.h"
#include <unordered_map>

//	게임에 존재하는 액터들을 관리하는 매니저 클래스입니다.
//	각 객체의 생성과 업데이트를 담당하도록 합니다.

class CGameWorld
{
public:
	CGameWorld();
	~CGameWorld();

	//	업데이트
	void Update(double daltaTime);

	//	액터 추가
	//	이미 추가된 액터를 추가하려고 하면 실패합니다.
	bool AddActor(CActor* actor);

	//	위치에서 액터를 찾습니다.
	CActor* FindActorFromPosition(COORD pos);

private:

	//vector<CActor*> m_aActors;
	unordered_map<CActor*, int> m_aActors;
};

