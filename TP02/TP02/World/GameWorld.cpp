
#include "GameWorld.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Character/Player.h"
#include "../Boss/Boss.h"
#include "../Enemy/Slime.h"
#include "../Boss/SemiBoss.h"
#include "../Graphics/Interface.h"
#include <cstdlib>
#include "../Enemy/Skeleton/Skeleton.h"
#include "../Enemy/Golem/Golem.h"
#include "../Time/Timer.h"
#include "../InGameMenu/Reward.h"
#include "../Input/Input.h"

CGameWorld::CGameWorld()
{
	m_dWorldTime = 0;
	m_dMonsterSpawnInitialTime = 0;
	m_dMonsterSpawnTime = 0;
	m_bMonsterSpawn = 0;
	m_iWorldLevel = 0;
	m_iSpawnSlime = 0;
	m_iSpawnSkeleton = 0;
	m_iSpawnGolem = 0;
	m_pSemiBoss = nullptr;
	m_pPlayer = nullptr;
	m_pBoss = nullptr;
	m_bSemibossCreated = false;
	m_bBossCreated = false;
	m_bSemibossWarned = false;
	m_bBossWarned = false;
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

	for (auto it = m_pInstance->m_aActors.begin(); it != m_pInstance->m_aActors.end();)
	{
		if (!m_pInstance->EraseActorFromSort(it->first))
		{
			//error
			int a = 1;
		}
		it = m_pInstance->m_aActors.erase(it);
	}

	m_pInstance->m_pPlayer = nullptr;
	m_pInstance->m_pSemiBoss = nullptr;
	m_pInstance->m_pBoss = nullptr;

	delete m_pInstance;
	m_pInstance = nullptr;
}

void CGameWorld::Initialize()
{
	m_dWorldTime = 0;
	m_dMonsterSpawnInitialTime = 10.0;	//	10초마다 몬스터 스폰합니다
	m_dMonsterSpawnTime = 3.0;
	m_bMonsterSpawn = true;
	m_iSpawnSlime = 0;
	m_iSpawnSkeleton = 0;
	m_iSpawnGolem = 0;
	m_pSemiBoss = nullptr;
	m_pBoss = nullptr;
	m_pPlayer = nullptr;
	m_bSemibossCreated = false;
	m_bBossCreated = false;
	m_bSemibossWarned = false;
	m_bBossWarned = false;

	std::shared_ptr<CPlayer> pPlayer = make_shared<CPlayer>(Pixel::Gunman, TEXT_BACKGROUND_MAGENTA | TEXT_FOREGROUND_CYAN);
	m_pPlayer = pPlayer;
	AddActor(pPlayer);
}

int CGameWorld::Update(double deltaTime)
{
	//	월드 이벤트 업데이트
	if (!this->Tick(deltaTime))
	{	//	플레이어 사망
		return MAIN_MENU;
	}

	if (m_pSemiBoss != nullptr)
	{	//	보스 사망 처리
		if (!m_pSemiBoss.get()->IsValid())
		{
			//	사망함
			//	보상지급
			CTimer::GetInstance()->Pause();
			CReward rew;
			int reward = rew.GetReward();
			CGraphic::GetInstance()->ReDraw();
			CTimer::GetInstance()->Resume();
			CInput::GetInstance()->Update();	//	입력 업데이트 돌려서 입력버퍼 비우기
			m_pSemiBoss = nullptr;
		}
	}

	if (m_pBoss != nullptr)
	{	//	보스 사망 처리
		if (!m_pBoss.get()->IsValid())
		{
			//	사망함
			//	게임 종료(CLEAR)
			m_pBoss = nullptr;
			return GAME_CLEARED;
		}
	}

	//	액터 풀 업데이트
	for (auto it = m_aActors.begin(); it != m_aActors.end();)
	{
		if (!it->first->IsValid())
		{	//	무효한 객체입니다.
			if (!EraseActorFromSort(it->first))
			{
				//error
				int a = 1;
			}
			it = m_aActors.erase(it);
			//CGraphic::GetInstance()->AddLog("Actor Erased");
			CInterface::GetInstance()->SetValue(29, (int)m_aActors.size());
		}
		else
		{
			it->first->Tick(deltaTime);
			it->first->Render();
			it++;
		}
	}
	return GOTO_GAME;
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

bool CGameWorld::Tick(double deltaTime)
{
	m_dWorldTime += deltaTime;

	if (m_pPlayer != nullptr)
	{	//	플레이어 사망확인
		if (dynamic_pointer_cast<CPlayer>(m_pPlayer).get()->GetHealth() <= 0.0)
		{	//	사망했습니다!
			CDead dead;
			dead.Dead();
			return false;
		}
	}


	if (m_dWorldTime > 295.0 && !m_bSemibossWarned)
	{
		m_bSemibossWarned = true;
		CTimer::GetInstance()->Pause();
		CWarning warn;
		int reward = warn.Warning();
		CGraphic::GetInstance()->ReDraw();
		CTimer::GetInstance()->Resume();
		CInput::GetInstance()->Update();	//	입력 업데이트 돌려서 입력버퍼 비우기
	}

	if (m_dWorldTime > 300.0 && !m_bSemibossCreated)				////////////////////////////	중간보스 생성
	{	//	시간으로 보스 생성
		
		m_bSemibossCreated = true;
		shared_ptr<CSemiBoss> sboss = make_shared<CSemiBoss>();
		AddActor(sboss);
		m_pSemiBoss = sboss;
	}

	if (m_dWorldTime > 595.0 && !m_bBossWarned)
	{
		m_bBossWarned = true;
		CTimer::GetInstance()->Pause();
		CWarning warn;
		int reward = warn.Warning();
		CGraphic::GetInstance()->ReDraw();
		CTimer::GetInstance()->Resume();
		CInput::GetInstance()->Update();	//	입력 업데이트 돌려서 입력버퍼 비우기
	}

	if (m_dWorldTime > 600.0 && !m_bBossCreated)				////////////////////////////	최종보스 생성
	{	//	시간으로 보스 생성
		m_bBossCreated = true;
		shared_ptr<CBoss> boss = make_shared<CBoss>();
		AddActor(boss);
		m_pBoss = boss;
	}

	if (FindActorsByTag(ETag::monster).size() == 0)
	{	//	적이 하나도 없으면
		m_iWorldLevel++;
		//	월드 레벨에 맞춰 몬스터 생성
		m_iSpawnSlime = min(6, m_iWorldLevel);	//	최대 6마리
		m_iSpawnSkeleton = min(4, m_iWorldLevel / 3);	//	3레벨부터 3레벨마다 추가 최대 4마리
		m_iSpawnGolem = min(3, m_iWorldLevel / 7);	//	7레벨부터 7레벨마다 추가 최대 3마리
		MonsterSpawnEvent();
	}
	return true;
}

void CGameWorld::MonsterSpawnEvent()
{
	for (int i = 0; i < m_iSpawnSlime; i++)
	{
		//	몬스터 생성
		shared_ptr<CActor> enemy = make_shared<CSlime>();
		COORD spawnPos = { 0, 0 };

		// 0: 위, 1: 아래, 2: 왼쪽, 3: 오른쪽
		switch (rand() % 4)
		{
		case 0: // 위쪽 테두리 (Y는 0 고정, X는 무작위)
			spawnPos.X = rand() % 30;
			spawnPos.Y = 0;
			break;

		case 1: // 아래쪽 테두리 (Y는 29 고정, X는 무작위)
			spawnPos.X = rand() % 30;
			spawnPos.Y = 29;
			break;

		case 2: // 왼쪽 테두리 (X는 0 고정, Y는 무작위)
			spawnPos.X = 0;
			spawnPos.Y = rand() % 30;
			break;

		case 3: // 오른쪽 테두리 (X는 29 고정, Y는 무작위)
			spawnPos.X = 29;
			spawnPos.Y = rand() % 30;
			break;
		}

		// 몬스터 생성 위치 설정
		enemy->SetPosition(spawnPos);

		//	몬스터 액터 풀에 넣기
		AddActor(enemy);
	}

	for (int i = 0; i < m_iSpawnSkeleton; i++)
	{
		//	몬스터 생성
		shared_ptr<CActor> enemy = make_shared<CSkeleton>();
		COORD spawnPos = { 10, 10 };	//	근거리 생성

		// 0: 위, 1: 아래, 2: 왼쪽, 3: 오른쪽
		switch (rand() % 4)
		{
		case 0: // 위쪽 테두리 (Y는 0 고정, X는 무작위)
			spawnPos.X = rand() % 10 + 10;
			spawnPos.Y = 10;
			break;

		case 1: // 아래쪽 테두리 (Y는 29 고정, X는 무작위)
			spawnPos.X = rand() % 10 + 10;
			spawnPos.Y = 19;
			break;

		case 2: // 왼쪽 테두리 (X는 0 고정, Y는 무작위)
			spawnPos.X = 10;
			spawnPos.Y = rand() % 10 + 10;
			break;

		case 3: // 오른쪽 테두리 (X는 29 고정, Y는 무작위)
			spawnPos.X = 19;
			spawnPos.Y = rand() % 10 + 10;
			break;
		}

		// 몬스터 생성 위치 설정
		enemy->SetPosition(spawnPos);

		//	몬스터 액터 풀에 넣기
		AddActor(enemy);
	}

	for (int i = 0; i < m_iSpawnGolem; i++)
	{
		//	몬스터 생성
		shared_ptr<CActor> enemy = make_shared<CGolem>();
		COORD spawnPos = { 5, 5 };	//	중거리 생성

		// 0: 위, 1: 아래, 2: 왼쪽, 3: 오른쪽
		switch (rand() % 4)
		{
		case 0: // 위쪽 테두리 (Y는 0 고정, X는 무작위)
			spawnPos.X = rand() % 20 + 5;
			spawnPos.Y = 5;
			break;

		case 1: // 아래쪽 테두리 (Y는 29 고정, X는 무작위)
			spawnPos.X = rand() % 20 + 5;
			spawnPos.Y = 24;
			break;

		case 2: // 왼쪽 테두리 (X는 0 고정, Y는 무작위)
			spawnPos.X = 5;
			spawnPos.Y = rand() % 20 + 5;
			break;

		case 3: // 오른쪽 테두리 (X는 29 고정, Y는 무작위)
			spawnPos.X = 24;
			spawnPos.Y = rand() % 20 + 5;
			break;
		}

		// 몬스터 생성 위치 설정
		enemy->SetPosition(spawnPos);

		//	몬스터 액터 풀에 넣기
		AddActor(enemy);
	}
}

bool CGameWorld::AddActor(shared_ptr<CActor> actor)
{
	//	액터 풀에 새로운 액터를 추가합니다.
	m_aActors[actor]++;

	if (m_aActors[actor] > 1)
	{	//	이미 추가되어 있던 액터임
		m_aActors[actor]--;
		//CGraphic::GetInstance()->AddLog("Error : Already Added Actor");
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

	//CGraphic::GetInstance()->AddLog("Actor Successfuly Added");
	CInterface::GetInstance()->SetValue(29, (int)m_aActors.size());
	return true;
}

shared_ptr<CActor> CGameWorld::FindActorFromPosition(COORD pos, int tag)
{
	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;
		if ((i.first->m_eTag & tag) == 0)	continue;

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

vector<shared_ptr<CActor>> CGameWorld::FindActorsByRect(COORD LTPos, COORD RBPos, int tag)
{
	vector<shared_ptr<CActor>> retVec;

	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;
		if ((i.first->m_eTag & tag) == 0)	continue;

		COORD pos = i.first->GetPosition();

		if (LTPos.X <= pos.X && pos.X < RBPos.X && LTPos.Y <= pos.Y && pos.Y < LTPos.Y)
		{
			retVec.push_back(i.first);
		}
	}

	return retVec;
}

shared_ptr<CActor> CGameWorld::FindActorByActorCustom(COORD pos, int tag)
{
	for (auto& i : m_aActors)
	{
		if (!i.first->m_bIsValid)	continue;
		if ((i.first->m_eTag & tag) == 0)	continue;

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

		result = FindActorFromPosition(COORD(findingPos.X, findingPos.Y), tag);
		if (result == nullptr)	continue;

		return result;
	}

	return nullptr;
}

shared_ptr<CActor> CGameWorld::RayTraceWithActorCustom(COORD startPos, COORD direction, int tag)
{
	shared_ptr<CActor> result = nullptr;
	if (direction.X == 0 && direction.Y == 0)	return nullptr;	//	유효하지 않은 방향

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

		result = FindActorByActorCustom(COORD(findingPos.X, findingPos.Y), tag);
		if (result == nullptr)	continue;

		return result;
	}

	return nullptr;
}