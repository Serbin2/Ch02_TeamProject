
#include "GameWorld.h"


CGameWorld::CGameWorld()
{

}

CGameWorld::~CGameWorld()
{

}

void CGameWorld::Update(double deltaTime)
{
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