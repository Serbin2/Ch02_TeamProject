#include "Boss.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Utils/Utils.h"

#include "../Projectile/BossProjectile.h"

CBoss::CBoss(int Shape, int Color, FGridSize BossSize)
	: CCharacter(Shape, Color)
{
	m_cPosition = { 15, 15 };

	m_BossSize = BossSize;
}

CBoss::~CBoss()
{
};

void CBoss::Tick(double DeltaTime)
{
	m_fAccStateActionDelay += DeltaTime;
	m_iAccWaveAttackTriggerCooldown += DeltaTime;

	// 웨이브 어택을 위해서 사용
	if (m_iAccWaveAttackTriggerCooldown >= m_iWaveAttackTriggerCooldown)
	{
		m_iAccWaveAttackTriggerCooldown = 0;
		WaveAttack();
	}

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

	for (auto Projectile : m_vProjectiles)
	{
		Projectile->Tick(DeltaTime);
		//Projectile->Render();
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
	if (0 == rand() % 2)
	{
		FireProjectileToOutline();
	}
	else
	{
		WaveAttack();
	}
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

void CBoss::FireProjectileToOutline()
{
	// 공격 종료후 5초 후 이동 
	//static int sTestRange = 1;
	std::vector<FAttackPos> AttackPos = GetBossOutlineAttackRange(/*sTestRange++*/);
	std::string DebugMsg;
	for (const auto& Pos : AttackPos)
	{
		// 범위 체크
		if (Pos.Pos.X < 0 || TEMP_MAP_SIZE <= Pos.Pos.X
			|| Pos.Pos.Y < 0 || TEMP_MAP_SIZE <= Pos.Pos.Y)
		{
			continue;
		}

		COORD Dir;
		switch (Pos.Dir)
		{
		case EAttackDir::UP:
			Dir = { 0, -1 };
			break;
		case EAttackDir::UPRIGHT:
			Dir = { 1, -1 };
			break;
		case EAttackDir::RIGHT:
			Dir = { 1, 0 };
			break;
		case EAttackDir::RIGHTDOWN:
			Dir = { 1, 1 };
			break;
		case EAttackDir::DOWN:
			Dir = { 0, 1 };
			break;
		case EAttackDir::LEFTDOWN:
			Dir = { -1, 1 };
			break;
		case EAttackDir::LEFT:
			Dir = { -1, 0 };
			break;
		case EAttackDir::LEFTUP:
			Dir = { -1, -1 };
			break;
		}

		//auto pProjectile = make_shared<CBossProjectile>(Pixel::circle, TEXT_BACKGROUND_RED, Dir, 2.f);
		auto pProjectile = make_shared<CProjectile>(Pixel::circle, TEXT_BACKGROUND_RED);
		pProjectile->SetOwner(this);
		pProjectile->SetPosition(Pos.Pos);
		pProjectile->SetMoveDirection(Dir);
		pProjectile->SetSpeed(3.f);
		m_vProjectiles.push_back(pProjectile);

		DebugMsg += "(" + std::to_string(Pos.Pos.X) + ", " + std::to_string(Pos.Pos.Y) + "), ";
	}

	CGraphic* pGraphic = CGraphic::GetInstance();
	//pGraphic->AddLog(DebugMsg);
}

std::vector<FAttackPos> CBoss::GetBossOutlineAttackRange(int Range)
{
	const COORD pos = GetPosition();
	const int width = m_BossSize.m_iX;
	const int height = m_BossSize.m_iY;

	std::vector<FAttackPos> result;

	const int left = pos.X - Range;
	const int right = pos.X + width + Range - 1;
	const int top = pos.Y - Range;
	const int bottom = pos.Y + height + Range - 1;

	// Top Edge
	for (int x = left; x <= right; ++x)
	{
		result.push_back({ {(SHORT)x, (SHORT)top}, EAttackDir::UP });
	}

	// Bottom Edge
	for (int x = left; x <= right; ++x)
	{
		result.push_back({ {(SHORT)x, (SHORT)bottom}, EAttackDir::DOWN });
	}

	// Left Edge
	for (int y = top + 1; y < bottom; ++y)
	{
		result.push_back({ {(SHORT)left, (SHORT)y}, EAttackDir::LEFT });
	}

	// Right Edge
	for (int y = top + 1; y < bottom; ++y)
	{
		result.push_back({ {(SHORT)right, (SHORT)y}, EAttackDir::RIGHT });
	}

	return result;
}

void CBoss::WaveAttack()
{
	if (m_iCurrntWaveCount > m_iMaxWaveCount)
	{
		m_iCurrntWaveCount = 0;
		m_bIsActiveWaveAttack = false;
		return;
	}

	m_bIsActiveWaveAttack = true;

	std::vector<FAttackPos> AttackPos = GetBossOutlineAttackRange(m_iCurrntWaveCount++);
	for (const auto& AP : AttackPos)
	{
		if (AP.Pos.X < 0 || TEMP_MAP_SIZE <= AP.Pos.X
			|| AP.Pos.Y < 0 || TEMP_MAP_SIZE <= AP.Pos.Y)
		{
			continue;
		}

		auto pProjectile = make_shared<CProjectile>(Pixel::circle, TEXT_BACKGROUND_RED);
		pProjectile->SetOwner(this);
		pProjectile->SetPosition(AP.Pos);
		pProjectile->SetMoveDirection({ 0,0 });
		pProjectile->SetSpeed(0.f);
		m_vProjectiles.push_back(pProjectile);
	}
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
	SelectAttackPattern();
	ChangeState(EBossState::Move, 3);
}

void CBoss::MoveAction()
{
	// 텔포 -> 공격 스테이 후 
	Teleport();
	ChangeState(EBossState::Attack, 2);
}
