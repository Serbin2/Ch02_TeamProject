
#include "GameWorld.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Character/Player.h"
#include "../Boss/Boss.h"
#include "../Character/Sniper/Sniper.h"
#include "../Enemy/Slime.h"

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

void CGameWorld::Release()
{
	if (m_pInstance == nullptr)	return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

void CGameWorld::Initialize()
{
	m_dMonsterSpawnInitialTime = 10.0;	//	10초마다 몬스터 스폰합니다
	m_dMonsterSpawnTime = 3.0;
	m_bMonsterSpawn = true;
	m_iNumberOfMonsterSpawn = 3;	//	3마리씩 스폰합니다.

	//std::shared_ptr<CActor> pBoss = make_shared<CBoss>(Pixel::triangle, TEXT_BACKGROUND_BLACK | TEXT_BACKGROUND_BLUE_INT, FGridSize(2, 2));
	//std::shared_ptr<CActor> pPlayer = make_shared<CPlayer>(Pixel::square, TEXT_BACKGROUND_MAGENTA | TEXT_FOREGROUND_CYAN);
	std::shared_ptr<CActor> pPlayer = make_shared<CSniper>();
	AddActor(pPlayer);
	//AddActor(pBoss);

	m_pPlayer = pPlayer;
}

void CGameWorld::Update(double deltaTime)
{
	//	월드 이벤트 업데이트
	this->Tick(deltaTime);

	//	액터 풀 업데이트
	for (auto it = m_aActors.begin(); it != m_aActors.end();)
	{
		if (!it->first->IsValid())
		{	//	무효한 객체입니다.
			if (EraseActorFromSort(it->first))
			{
				CGraphic::GetInstance()->AddLog("Actor Successfuly Erased From Sort Container");
			}
			it = m_aActors.erase(it);
			CGraphic::GetInstance()->AddLog("Actor Erased");
		}
		else
		{
			it->first->Tick(deltaTime);
			it->first->Render();
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
			st.first->Render();
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
		shared_ptr<CActor> slime = make_shared<CSlime>();
		//	몬스터 액터 풀에 넣기
		AddActor(slime);
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
		CGraphic::GetInstance()->AddLog("Error : Already Added Actor");
		return false;
	}

	int tag = actor->m_eTag;
	if (tag & ETag::environment)
	{//	환경 오브젝트
		m_aSort[0][actor]++;
	}
	else if (tag & ETag::effect || tag & ETag::projectile)
	{//	발사체와 이펙트
		m_aSort[2][actor]++;
	}
	else
	{
		m_aSort[1][actor]++;
	}

	CGraphic::GetInstance()->AddLog("Actor Successfuly Added");
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

shared_ptr<CActor> CGameWorld::FindActorByActorCustom(COORD pos)
{
	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;

		if (i.first->ActorCustomCollisionTest(pos))
		{
			return i.first;
		}
	}

	return nullptr;
}

bool CGameWorld::EraseActorFromSort(shared_ptr<CActor> actor)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_aSort[i].erase(actor) > 0)	return true;
	}

	return false;
}

shared_ptr<CActor> CGameWorld::RayTrace(COORD startPos, COORD direction, int tag) 
{
	shared_ptr<CActor> result = nullptr;
	if (direction.X == 0 && direction.Y == 0)	return nullptr;	//	유효하지 않은 방향

	if (tag == ETag::none)
	{
		tag = ~tag;
	}
	COORD findingPos = startPos;
	while (1)
	{
		findingPos.X += direction.X;
		findingPos.Y += direction.Y;
		if (findingPos.X < 0 || findingPos.X > 29 || findingPos.Y < 0 || findingPos.Y > 29)
		{	//	맵을 벗어남
			//	유효한 결과를 찾지 못함
			return nullptr;
		}

		result = FindActorFromPosition(COORD(findingPos.X, findingPos.Y));
		if (result == nullptr)	continue;
		if (result.get()->m_eTag & tag)
		{
			return result;
		}
		//	아직 유효한 결과를 찾지 못함
		result = nullptr;
	}

	return nullptr;
}

shared_ptr<CActor> CGameWorld::RayTraceWithActorCustom(COORD startPos, COORD direction, int tag)
{
	shared_ptr<CActor> result = nullptr;
	if (direction.X == 0 && direction.Y == 0)	return nullptr;	//	유효하지 않은 방향

	if (tag == ETag::none)
	{
		tag = ~tag;
	}
	COORD findingPos = startPos;
	while (1)
	{
		findingPos.X += direction.X;
		findingPos.Y += direction.Y;
		if (findingPos.X < 0 || findingPos.X > 29 || findingPos.Y < 0 || findingPos.Y > 29)
		{	//	맵을 벗어남
			//	유효한 결과를 찾지 못함
			return nullptr;
		}

		result = FindActorByActorCustom(COORD(findingPos.X, findingPos.Y));
		if (result == nullptr)	continue;
		if (result.get()->m_eTag & tag)
		{
			return result;
		}
		//	아직 유효한 결과를 찾지 못함
		result = nullptr;
	}

	return nullptr;
}
//플레이어 가져오기
CPlayer* CGameWorld::GetPlayer() const
{
	return dynamic_cast<CPlayer*>(m_pPlayer.get());
}