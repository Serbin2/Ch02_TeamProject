
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

CGameWorld* CGameWorld::m_pInstance = nullptr;

CGameWorld* CGameWorld::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameWorld();
	}

	return m_pInstance;
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
			it = m_aActors.erase(it);
		}
		else
		{	
			it->first->Tick(deltaTime);
			it++;
		}
	}
}

void CGameWorld::Render()
{
	for (int i = 0; i < 3; i++)
	{
		for (auto st : m_aSort[i])
		{
			st->Render();
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

bool CGameWorld::AddActor(shared_ptr<CActor> actor)
{
	//	액터 풀에 새로운 액터를 추가합니다.
	m_aActors[actor]++;

	if (m_aActors[actor] > 1)
	{	//	이미 추가되어 있던 액터임
		m_aActors[actor]--;
		return false;
	}

	int tag = actor->m_eTag;
	if (tag & ETag::environment)
	{//	환경 오브젝트
		m_aSort[0].push_back(actor);
	}
	else if (tag & ETag::effect)
	{
		m_aSort[2].push_back(actor);
	}
	else
	{
		m_aSort[1].push_back(actor);
	}

	return true;
}

shared_ptr<CActor> CGameWorld::FindActorFromPosition(COORD pos)
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

vector<shared_ptr<CActor>> CGameWorld::FindActorsByTag(ETag tag)
{
	vector<shared_ptr<CActor>> retVec;

	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;

		if (i.first->HasTag(tag))
		{
			retVec.push_back(i.first);
		}
	}

	return retVec;
}

vector<shared_ptr<CActor>> CGameWorld::FindActorsByRect(COORD LTPos, COORD RBPos)
{
	vector<shared_ptr<CActor>> retVec;

	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;

		COORD pos = i.first->GetPosition();

		if (LTPos.X <= pos.X && pos.X < RBPos.X && LTPos.Y <= pos.Y && pos.Y < LTPos.Y)
		{
			retVec.push_back(i.first);
		}
	}

	return retVec;
}

vector<shared_ptr<CActor>> CGameWorld::FindActorsByActorCustom(COORD pos)
{
	vector<shared_ptr<CActor>> retVec;

	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;

		if (i.first->ActorCustomCollisionTest(pos))
		{
			retVec.push_back(i.first);
		}
	}

	return retVec;
}