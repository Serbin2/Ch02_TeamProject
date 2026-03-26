
#include "GameWorld.h"
#include "../Graphics/ConsoleGraphic.h"

CGameWorld::CGameWorld()
{
	m_dMonsterSpawnInitialTime = 0;
	m_dMonsterSpawnTime = 0;
	m_bMonsterSpawn = 0;
	m_iNumberOfMonsterSpawn = 0;
}

CGameWorld::~CGameWorld()
{

}

void CGameWorld::Initialize()
{
	m_dMonsterSpawnInitialTime = 10.0;	//	10초마다 몬스터 스폰합니다
	m_dMonsterSpawnTime = m_dMonsterSpawnInitialTime;
	m_bMonsterSpawn = true;
	m_iNumberOfMonsterSpawn = 3;	//	3마리씩 스폰합니다.
}

void CGameWorld::Update(double deltaTime)
{
	//	월드 이벤트 업데이트
	this->Tick(deltaTime);

	//	액터 풀 업데이트
	for (auto it = m_aActors.begin(); it != m_aActors.end();)
	{
		if (!it->first->m_bIsValid)
		{	//	무효한 객체입니다.
			delete it->first;	//	삭제
			it = m_aActors.erase(it);
		}
		else
		{	
			it->first->Tick(deltaTime);
			it++;
		}
	}
}

void CGameWorld::Tick(double deltaTime)
{
	//	몬스터 스폰 이벤트
	MonsterSpawnEvent(deltaTime);


}

void CGameWorld::MonsterSpawnEvent(double deltaTime)
{
	m_dMonsterSpawnTime -= deltaTime;

	if (m_dMonsterSpawnTime > 0)	return;

	for (int i = 0; i < m_iNumberOfMonsterSpawn; i++)
	{
		//	몬스터 생성

		//	몬스터 액터 풀에 넣기
	}

	CGraphic::GetInstance()->AddLog("몬스터를 세마리 생성했습니다.");

	m_dMonsterSpawnTime = m_dMonsterSpawnInitialTime;
}

bool CGameWorld::AddActor(CActor* actor)
{
	//	액터 풀에 새로운 액터를 추가합니다.
	m_aActors[actor]++;

	if (m_aActors[actor] > 1)
	{	//	이미 추가되어 있던 액터임
		m_aActors[actor]--;
		return false;
	}

	return true;
}

CActor* CGameWorld::FindActorFromPosition(COORD pos)
{
	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;

		if (i.first->m_cPosition.X == pos.X && i.first->m_cPosition.Y == pos.Y)
		{
			return i.first;
		}
	}

	return nullptr;
}