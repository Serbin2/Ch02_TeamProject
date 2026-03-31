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
	static void Release();

	//	게임 생성
	//	초기 배치등 여기서 진행하시면 됩니다.
	//	타이밍이 필요한 이벤트 등 여기서 준비 작업 해주세요.
	void Initialize();

	//	업데이트
	int Update(double daltaTime);	

	void Render();

	//	액터 추가
	//	이미 추가된 액터를 추가하려고 하면 실패합니다.
	bool AddActor(shared_ptr<CActor> actor);

	//	플레이어를 가져옵니다
	shared_ptr<CActor> GetPlayerActor() { return m_pPlayer; };

	//	위치에서 액터를 찾습니다.
	shared_ptr<CActor> FindActorFromPosition(COORD pos, int tag = ETag::all);

	//	태그로 액터들을 찾습니다.
	vector<shared_ptr<CActor>> FindActorsByTag(ETag tag);

	//	범위에서 액터를 찾습니다.
	//	시작위치는 포함하지만 끝위치는 포함하지 않습니다.
	//	음수가 들어와도 검사 됩니다. (보정 X)
	//	사각형은 좌상단 위치와 우하단 위치로 넣어주셔야됩니다.	LeftTop / RightBottom
	vector<shared_ptr<CActor>> FindActorsByRect(COORD LTPos, COORD RBPos, int tag = ETag::all);

	//	액터에게 충돌 여부 판정을 처리하게합니다
	//	찾으려는 위치에 대해 true를 반환하는 액터를 찾습니다.
	shared_ptr<CActor> FindActorByActorCustom(COORD pos, int tag = ETag::all);

	//	RayTrace
	//	해당 방향으로 가장 먼저 적중하는 액터를 반환합니다.
	//	태그를 사용할 경우 해당 태그에서만 검색합니다.
	//	최적화 되어있지 않음 사용시 주의 할 것
	shared_ptr<CActor> RayTrace(COORD startPos, COORD direction, int tag = ETag::all);

	shared_ptr<CActor> RayTraceWithActorCustom(COORD startPos, COORD direction, int tag = ETag::all);

private:

	static CGameWorld* m_pInstance;

	//	월드 자체의 업데이트 입니다.
	//	각종 이벤트는 여기서 실행하세요.
	bool Tick(double deltaTime);	//	사망시 false 반환
	double m_dWorldTime;	//	월드 진행 시간입니다.
	int m_iWorldLevel;

	void MonsterSpawnEvent();
	bool EraseActorFromSort(shared_ptr<CActor> actor);

	double m_dMonsterSpawnInitialTime;
	double m_dMonsterSpawnTime;
	bool m_bMonsterSpawn;
	int m_iSpawnSlime;
	int m_iSpawnSkeleton;
	int m_iSpawnGolem;

	//	플레이어 액터는 특별 관리
	shared_ptr<CActor> m_pPlayer;

	bool m_bSemibossWarned;
	shared_ptr<CActor> m_pSemiBoss;
	bool m_bSemibossCreated;
	bool m_bBossWarned;
	shared_ptr<CActor> m_pBoss;
	bool m_bBossCreated;

	//	전체 액터 관리용 마스터 컨테이너
	unordered_map<shared_ptr<CActor>, int> m_aActors;


	//	렌더 소팅용 컨테이너
	unordered_map<shared_ptr<CActor>, int> m_aSort[3];
};

