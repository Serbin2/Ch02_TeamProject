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

	//	게임 생성
	//	초기 배치등 여기서 진행하시면 됩니다.
	//	타이밍이 필요한 이벤트 등 여기서 준비 작업 해주세요.
	void Initialize();

	//	업데이트
	void Update(double daltaTime);	

	//	액터 추가
	//	이미 추가된 액터를 추가하려고 하면 실패합니다.
	bool AddActor(CActor* actor);

	//	위치에서 액터를 찾습니다.
	CActor* FindActorFromPosition(COORD pos);

private:

	//	월드 자체의 업데이트 입니다.
	//	각종 이벤트는 여기서 실행하세요.
	void Tick(double deltaTime);

	void MonsterSpawnEvent(double deltaTime);

	double m_dMonsterSpawnInitialTime;
	double m_dMonsterSpawnTime;
	bool m_bMonsterSpawn;
	int m_iNumberOfMonsterSpawn;

	//vector<CActor*> m_aActors;
	unordered_map<CActor*, int> m_aActors;
};

