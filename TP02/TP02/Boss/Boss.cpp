#include "Boss.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Utils/Utils.h"
#include "../Graphics/ConsoleGraphic.h"

CBoss::CBoss(int Shape, int Color, FGridSize BossSize)
	: CCharacter(Shape, Color)
{
	m_cPosition = { 20, 20 };

	m_BossSize = BossSize;
}
CBoss::~CBoss()
{
};

void CBoss::Tick(double DeltaTime)
{
	m_fAccStateActionDelay += DeltaTime;

	// 행동 변화 쿨타임 
	if (m_fAccStateActionDelay >= m_fStateActionDelay)
	{
		m_fAccStateActionDelay = 0.f;

		switch (m_CurrentState)
		{
		case EBossState::Groggy:
			GroggyAction();
			break;
		case EBossState::Attack:
			AttackAction();
			break;
		case EBossState::Move:
			MoveAction();
			break;
		}
	}

	Render();
}

void CBoss::Render()
{
	const COORD CurrentPosition = GetPosition();

	CGraphic* pGraphic = CGraphic::GetInstance();
	if (pGraphic)
	{
		for (int i = 0; i < m_BossSize.m_iX; ++i)
		{
			for (int j = 0; j < m_BossSize.m_iY; ++j)
			{
				pGraphic->RenderToBuffer(CurrentPosition.X + i, CurrentPosition.Y + j, m_pShape, m_tColor);
			}
		}
	}
}

void CBoss::OnHit(float Damage)
{
	m_fCurrentHealth = MathUtil::Clamp<float>(m_fCurrentHealth - Damage,  0.f, m_fCurrentHealth);
	if (IsDead())
	{
		// [TODO-PJH] : 플레이어에게 몬스터가 소유한 아이템 or 경험치 정보 전달 
		return;
	}

	// 그로기 증가
	float AddGroggyAmount = Damage / 2.f;
	m_fAccGroggy += AddGroggyAmount;
}

void CBoss::SelectAttackPattern()
{
}

COORD CBoss::FindCanTelportPosition(/*CPlayer* Player*/)
{
	SHORT iRandX = MathUtil::Clamp(rand() % TEMP_MAP_SIZE - m_BossSize.m_iX, 0, TEMP_MAP_SIZE - m_BossSize.m_iX);
	SHORT iRandY = MathUtil::Clamp(rand() % TEMP_MAP_SIZE - m_BossSize.m_iY, 0, TEMP_MAP_SIZE - m_BossSize.m_iY);
	return COORD(iRandX, iRandY);
}

void CBoss::Teleport()
{
	SetPosition(FindCanTelportPosition());
}

void CBoss::FireProjectileToCircle()
{
	
}

std::vector<COORD> CBoss::GetBossOutlineAttackRange(int Range)
{
	const COORD cCurrentPos = GetPosition();
	const int iWidth = m_BossSize.m_iX;
	const int iHeight = m_BossSize.m_iY;

	std::vector<COORD> Outline;

	int iMinX = cCurrentPos.X - Range;
	int iMinY = cCurrentPos.Y - Range;
	int iMaxX = cCurrentPos.X + iWidth + Range - 1;
	int iMaxY = cCurrentPos.Y + iHeight + Range - 1;

	// 오른쪽 
	for (int iX = iMinX; iX <= iMaxX; ++iX)
	{
		Outline.push_back({ (SHORT)iX, (SHORT)iMinY });
	}

	// 오른쪽 아래
	for (int iY = iMinY + 1; iY <= iMaxY - 1; ++iY)
	{
		Outline.push_back({ (SHORT)iMaxX, (SHORT)iY });
	}

	// 왼쪽
	for (int iX = iMaxX; iX >= iMinX; --iX)
	{
		Outline.push_back({ (SHORT)iX, (SHORT)iMaxY });
	}

	// 왼쪽 위
	for (int iY = iMaxY - 1; iY >= iMinY + 1; --iY)
	{
		Outline.push_back({ (SHORT)iMinX, (SHORT)iY });
	}

	return Outline;
}

void CBoss::ChangeState(EBossState NewState, float Delay)
{
	// 동일하면 변경 x 
	if (NewState == m_CurrentState)
	{
		return;
	}
	
	// 누적값 초기화
	m_fAccStateActionDelay = 0;
	// 딜레이 설정 
	m_fStateActionDelay = Delay;
	m_CurrentState = NewState;
}

void CBoss::GroggyAction()
{
	// [TODO-PJH] : 그로기 상태임을 표시할 수 있게
	ChangeState(EBossState::Attack, 0);
}

void CBoss::AttackAction()
{
	// 공격 종료후 5초 후 이동 
	std::vector<COORD> AttackPos = GetBossOutlineAttackRange(1);
	std::string DebugMsg;
	for (const auto& Pos : AttackPos)
	{
		// 범위 체크
		if (Pos.X < 0 || TEMP_MAP_SIZE <= Pos.X
			|| Pos.Y < 0 || TEMP_MAP_SIZE <= Pos.Y)
		{
			continue;
		}

		DebugMsg += "(" + std::to_string(Pos.X) + ", " + std::to_string(Pos.Y) + "), ";
	}

	CGraphic* pGraphic = CGraphic::GetInstance();
	pGraphic->AddLog(DebugMsg);

	ChangeState(EBossState::Move, 3);
}

void CBoss::MoveAction()
{
	// 텔포 -> 공격 스테이 후 
	Teleport();
	ChangeState(EBossState::Attack, 2);
}
