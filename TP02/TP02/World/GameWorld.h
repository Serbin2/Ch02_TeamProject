#pragma once

#include "../Standard.h"
#include "../Character/Actor.h"
#include <unordered_map>

//	게임에 존재하는 액터들을 관리하는 매니저 클래스입니다.
//	각 객체의 생성과 업데이트를 담당하도록 합니다.

class CGameWorld
{
protected:
	CGameWorld();
	~CGameWorld();
	
public:

	static CGameWorld* GetInstance();

	//	게임 생성
	//	초기 배치등 여기서 진행하시면 됩니다.
	//	타이밍이 필요한 이벤트 등 여기서 준비 작업 해주세요.
	void Initialize();

	//	업데이트
	void Update(double daltaTime);	

	void Render();

	//	액터 추가
	//	이미 추가된 액터를 추가하려고 하면 실패합니다.
	bool AddActor(shared_ptr<CActor> actor);

	//	위치에서 액터를 찾습니다.
	shared_ptr<CActor> FindActorFromPosition(COORD pos);

	//	태그로 액터들을 찾습니다.
	vector<shared_ptr<CActor>> FindActorsByTag(ETag tag);

	//	범위에서 액터를 찾습니다.
	//	시작위치는 포함하지만 끝위치는 포함하지 않습니다.
	vector<shared_ptr<CActor>> FindActorsByRect(COORD statrPos, COORD endPos);

private:

	static CGameWorld* m_pInstance;

	//	월드 자체의 업데이트 입니다.
	//	각종 이벤트는 여기서 실행하세요.
	void Tick(double deltaTime);

	void MonsterSpawnEvent(double deltaTime);

	double m_dMonsterSpawnInitialTime;
	double m_dMonsterSpawnTime;
	bool m_bMonsterSpawn;
	int m_iNumberOfMonsterSpawn;

	//	전체 액터 관리용 마스터 컨테이너
	unordered_map<shared_ptr<CActor>, int> m_aActors;


	//	렌더 소팅용 컨테이너
	vector<shared_ptr<CActor>> m_aSort[3];
};

