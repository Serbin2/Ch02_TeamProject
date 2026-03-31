
#include "Boss.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Utils/Utils.h"
#include "../Graphics/Interface.h"
#include "../Projectile/BossProjectile.h"
#include "../Graphics/Effect.h"
#include "../Manager/SoundManager/SoundManager.h"
#include "../Time/Timer.h"

CBoss::CBoss() : CEnemy(0, 0)
{
	m_pShape = Pixel::square;
	m_tColor = TEXT_BACKGROUND_WHITE | TEXT_FOREGROUND_RED;

	m_cPosition = { 15, 15 };

	m_fHealth = 1000.f;
	m_fMaxHealth = 1000.f;

	m_sName = "[수집가 스크랩스]";
	m_eTag = ETag::actor | ETag::character | ETag::monster | ETag::boss;
	CInterface::GetInstance()->AddUI(24, m_sName);
	CInterface::GetInstance()->AddUI(25, "HP : ");
	CInterface::GetInstance()->SetValue(24, " ");
	CInterface::GetInstance()->SetValue(25, m_fHealth);
	GET_SINGLE(CSoundManager)->PlaySFX(L"Boss");
}

CBoss::~CBoss()
{
	CInterface::GetInstance()->RemoveUI(24);
	CInterface::GetInstance()->RemoveUI(25);
	GET_SINGLE(CSoundManager)->PlaySFX(L"FBossDead");
};

void CBoss::Tick(double DeltaTime)
{
	m_fAccStateActionDelay += DeltaTime;
	m_iAccWaveAttackTriggerCooldown += DeltaTime;
	if (m_dInvincibleTimer > 0.0) m_dInvincibleTimer -= DeltaTime;

	// 웨이브 어택을 위해서 사용
	if (m_bIsActiveWaveAttack )
	{
		if (m_iAccWaveAttackTriggerCooldown >= m_iWaveAttackTriggerCooldown)
		{
			m_iAccWaveAttackTriggerCooldown = 0;
			WaveAttack();
		}
	}

	if (m_bIsActiveLineAttack) 
	{
		m_iAccLineAttack += DeltaTime;
		if (m_iAccLineAttack >= m_iLineAttackTriggerCooldown)
		{
			m_iAccLineAttack = 0;
			LineAttack();
		}
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
		}
	}
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
	m_fHealth = MathUtil::Clamp<float>(m_fHealth - Damage,  0.f,  m_fMaxHealth);
	if (IsDead())
	{
		// [TODO-PJH] : 플레이어에게 몬스터가 소유한 아이템 or 경험치 정보 전달 
		GET_SINGLE(CSoundManager)->PlaySFX(L"FBossLastHit");
		CTimer::GetInstance()->Pause();
		Sleep(2000);
		CTimer::GetInstance()->Resume();
		m_bIsValid = false;
		return;
	}
	CInterface::GetInstance()->SetValue(25, m_fHealth);
	// 그로기 증가
	float AddGroggyAmount = Damage / 2.f;
	m_fAccGroggy += AddGroggyAmount;

	CInterface::GetInstance()->SetValue(25, std::to_string(m_fHealth));
}

void CBoss::SelectAttackPattern()
{
	int iRandValue = rand() % 2;
	//if (iRandValue == 1)
	{
		FireProjectileToOutline();
	}

	if (m_bIsActiveWaveAttack || m_bIsActiveLineAttack)
	{
		return;
	}

	iRandValue = rand() % 2;
	if (iRandValue == 0)
	{
		WaveAttack();
	}
	else if (iRandValue == 1)
	{
		FindLineByPlayer();
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
	//	텔레포트 이후 공격
}

void CBoss::FireProjectileToOutline()
{
	// 공격 종료후 5초 후 이동 
	static int sTestRange = 1;
	if (sTestRange >= 4)
	{
		sTestRange = 1;
	}

	std::vector<FAttackPos> AttackPos = GetBossOutlineAttackRange(sTestRange++);
	//std::string DebugMsg;
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

		//auto pProjectile = make_shared<CBossProjectile>(Pixel::circle, TEXT_BACKGROUND_RED, m_eLinAttackDir, 2.f);
		auto pProjectile = make_shared<CProjectile>(Pixel::cross, TEXT_FOREGROUND_RED_INT);
		auto sharedOwner = std::static_pointer_cast<CCharacter>(shared_from_this());
		pProjectile->SetOwner(sharedOwner);
		pProjectile->SetPosition(Pos.Pos);
		pProjectile->SetMoveDirection(Dir);
		pProjectile->SetSpeed(15.f);
		pProjectile->SetLifeTime(3.0);
		CGameWorld::GetInstance()->AddActor(pProjectile);

		//DebugMsg += "(" + std::to_string(Pos.Pos.X) + ", " + std::to_string(Pos.Pos.Y) + "), ";
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

void CBoss::GetBossOutlineAttackRange(int Range, std::vector<COORD>& vUP, std::vector<COORD>& vDown, std::vector<COORD>& vLeft, std::vector<COORD>& vRight)
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
		vUP.push_back({(SHORT)x, (SHORT)top});
	}

	// Bottom Edge
	for (int x = left; x <= right; ++x)
	{
		vDown.push_back( {(SHORT)x, (SHORT)bottom});
	}

	// Left Edge
	for (int y = top + 1; y < bottom; ++y)
	{
		vLeft.push_back( {(SHORT)left, (SHORT)y});
	}

	// Right Edge
	for (int y = top + 1; y < bottom; ++y)
	{
		vRight.push_back({(SHORT)right, (SHORT)y});
	}
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


	std::vector<COORD> vUp;
	std::vector<COORD> vLeft;
	std::vector<COORD> vRight;
	std::vector<COORD> vDown;

	GetBossOutlineAttackRange(m_iCurrntWaveCount, vUp, vDown, vLeft, vRight);
	
	std::vector<FAttackPos> AttackPos;
	
	for (const auto& AP : vUp)
	{
		if (AP.X < 0 || TEMP_MAP_SIZE <= AP.X
			|| AP.Y < 0 || TEMP_MAP_SIZE <= AP.Y)
		{
			continue;
		}

		// 충돌 판정 필요
		ApplyDamage(AP, m_fWaveAttackDamage);
	}

	for (const auto& AP : vLeft)
	{
		if (AP.X < 0 || TEMP_MAP_SIZE <= AP.X
			|| AP.Y < 0 || TEMP_MAP_SIZE <= AP.Y)
		{
			continue;
		}

		// 충돌 판정 필요
		ApplyDamage(AP, m_fWaveAttackDamage);
	}

	for (const auto& AP : vRight)
	{
		if (AP.X < 0 || TEMP_MAP_SIZE <= AP.X
			|| AP.Y < 0 || TEMP_MAP_SIZE <= AP.Y)
		{
			continue;
		}

		// 충돌 판정 필요
		ApplyDamage(AP, m_fWaveAttackDamage);
	}

	for (const auto& AP : vDown)
	{
		if (AP.X < 0 || TEMP_MAP_SIZE <= AP.X
			|| AP.Y < 0 || TEMP_MAP_SIZE <= AP.Y)
		{
			continue;
		}

		// 충돌 판정 필요
		ApplyDamage(AP, m_fWaveAttackDamage);
	}

	// 이펙트
	{
		shared_ptr<CEffect> FX = make_shared<CEffect>();
		vector<pair<COORD, COORD>>shape;
		shape.push_back(make_pair(vUp[0], vUp[vUp.size() - 1]));
		vector<double> vShotDuration;
		vShotDuration.push_back(0.1);
		vector<pair<int, int>> vShotMaterial;
		vShotMaterial.push_back(make_pair(Pixel::blank, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
		FX->CreateDynamicEffect(1, vShotMaterial, shape, vShotDuration);
		CGameWorld::GetInstance()->AddActor(FX);
	}
	// 이펙트
	{
		shared_ptr<CEffect> FX = make_shared<CEffect>();
		vector<pair<COORD, COORD>>shape;
		shape.push_back(make_pair(vLeft[0], vLeft[vLeft.size() - 1]));
		vector<double> vShotDuration;
		vShotDuration.push_back(0.1);
		vector<pair<int, int>> vShotMaterial;
		vShotMaterial.push_back(make_pair(Pixel::blank, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
		FX->CreateDynamicEffect(1, vShotMaterial, shape, vShotDuration);
		CGameWorld::GetInstance()->AddActor(FX);
	}

	// 이펙트
	{
		shared_ptr<CEffect> FX = make_shared<CEffect>();
		vector<pair<COORD, COORD>>shape;
		shape.push_back(make_pair(vRight[0], vRight[vRight.size() - 1]));
		vector<double> vShotDuration;
		vShotDuration.push_back(0.1);
		vector<pair<int, int>> vShotMaterial;
		vShotMaterial.push_back(make_pair(Pixel::blank, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
		FX->CreateDynamicEffect(1, vShotMaterial, shape, vShotDuration);
		CGameWorld::GetInstance()->AddActor(FX);
	}
	// 이펙트
	{
		shared_ptr<CEffect> FX = make_shared<CEffect>();
		vector<pair<COORD, COORD>>shape;
		shape.push_back(make_pair(vDown[0], vDown[vDown.size() - 1]));
		vector<double> vShotDuration;
		vShotDuration.push_back(0.1);
		vector<pair<int, int>> vShotMaterial;
		vShotMaterial.push_back(make_pair(Pixel::blank, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
		FX->CreateDynamicEffect(1, vShotMaterial, shape, vShotDuration);
		CGameWorld::GetInstance()->AddActor(FX);
	}

	m_iCurrntWaveCount++;
}

void CBoss::LineAttack()
{
	// 1️⃣ 현재 라인이 맵 범위 벗어나면 종료
	if (m_cLineStart.X < 0 || m_cLineStart.X >= 30 ||
		m_cLineStart.Y < 0 || m_cLineStart.Y >= 30)
	{
		m_bIsActiveLineAttack = false;
		return;
	}

	COORD cEffectStartPos;
	COORD cEffectEndPos;

	// 왼쪽 아니면 오른쪽
	if (m_cLineDir.X != 0)
	{
		for (int y = 0; y < 30; ++y)
		{
			ApplyDamage(COORD(m_cLineStart.X, y), m_fLineAttackDamage);
		}
		cEffectStartPos = { m_cLineStart.X , 0 };
		cEffectEndPos = { m_cLineStart.X , 29 };
	}
	// 위 아니면 아래
	else
	{
		for (int x = 0; x < 30; ++x)
		{
			ApplyDamage(COORD(x, m_cLineStart.Y), m_fLineAttackDamage);
		}
		cEffectStartPos = { 0, m_cLineStart.Y };
		cEffectEndPos = { 29 , m_cLineStart.Y };
	}

	shared_ptr<CEffect> FX = make_shared<CEffect>();
	vector<pair<COORD, COORD>>shape;
	shape.push_back(make_pair(cEffectStartPos, cEffectEndPos));

	vector<double> vShotDuration; 
	vShotDuration.push_back(0.1);
	vector<pair<int, int>> vShotMaterial;
	vShotMaterial.push_back(make_pair(Pixel::blank, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));

	FX->CreateDynamicEffect(1, vShotMaterial, shape, vShotDuration);
	CGameWorld::GetInstance()->AddActor(FX);

	// 3️⃣ 다음 라인으로 이동 (다음 Tick에서 실행됨)
	m_cLineStart.X += m_cLineDir.X;
	m_cLineStart.Y += m_cLineDir.Y;
	GET_SINGLE(CSoundManager)->PlaySFX(L"LASER");
}


void CBoss::ChangeState(EBossState NewState, float Delay)
{
	// 동일하면 변경 x 
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
	//FindLineByPlayer();
	ChangeState(EBossState::Attack, 2);
}

void CBoss::MoveAction()
{
	// 텔포 -> 공격 스테이 후 
	Teleport();
	ChangeState(EBossState::Attack, 2);
}

void CBoss::ApplyDamage(const COORD& Pos, float Damage)
{
	std::shared_ptr<CActor> pFindedActor = CGameWorld::GetInstance()->FindActorFromPosition(Pos, ETag::player);
	if (pFindedActor)
	{
		std::shared_ptr<CCharacter> pChar = std::dynamic_pointer_cast<CCharacter>(pFindedActor);
		if (pChar)
		{
			// 데미지 부여하기
			pChar->OnHit(Damage);
		}
	}
}

bool CBoss::ActorCustomCollisionTest(COORD pos)
{
	if (m_cPosition.X <= pos.X && pos.X <= m_cPosition.X + m_BossSize.m_iX &&
		m_cPosition.Y <= pos.Y && pos.Y <= m_cPosition.Y + m_BossSize.m_iY)
	{
		return true;
	}
	return false;
}

void CBoss::FindLineByPlayer()
{
	const COORD MyPos = GetPosition();
	const COORD PlayerPos = CGameWorld::GetInstance()->GetPlayerActor()->GetPosition();

	int left = MyPos.X;
	int right = MyPos.X + m_BossSize.m_iX - 1;
	int top = MyPos.Y;
	int bottom = MyPos.Y + m_BossSize.m_iY - 1;

	int dx = PlayerPos.X - MyPos.X;
	int dy = PlayerPos.Y - MyPos.Y;


	if (abs(dx) > abs(dy))
	{
		if (dx > 0)
		{
			// 오른쪽
			m_cLineStart.X = right + 1;
			m_cLineStart.Y = MathUtil::Clamp<SHORT>(PlayerPos.Y, top, bottom);
			m_cLineDir = { 1, 0 };
		}
		else
		{
			// 왼쪽
			m_cLineStart.X = left - 1;
			m_cLineStart.Y = MathUtil::Clamp<SHORT>(PlayerPos.Y, top, bottom);
			m_cLineDir = { -1, 0 };
		}
	}
	else
	{
		if (dy > 0)
		{
			// 아래
			m_cLineStart.X = MathUtil::Clamp<SHORT>(PlayerPos.X, left, right);
			m_cLineStart.Y = bottom + 1;
			m_cLineDir = { 0, 1 };
		}
		else
		{
			// 위
			m_cLineStart.X = MathUtil::Clamp<SHORT>(PlayerPos.X, left, right);
			m_cLineStart.Y = top - 1;
			m_cLineDir = { 0, -1 };
		}
	}

	m_bIsActiveLineAttack = true;
	return;
}