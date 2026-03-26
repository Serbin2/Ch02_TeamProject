// [작성자] : 박재현
// [Desc] : 보스 기본 클래스 

#pragma once
#include "../Standard.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Utils/Utils.h"
#include "../Character/Character.h"
#include <functional>

constexpr int TEMP_MAP_SIZE = 30;

class CPlayer;


enum class EAttackDir
{
	UP,
	UPRIGHT,
	RIGHT,
	RIGHTDOWN,
	DOWN,
	LEFTDOWN,
	LEFT,
	LEFTUP
};

struct FAttackPos
{
	COORD Pos;
	EAttackDir Dir;
};

// 보스 FSM을 위한 상태값 
enum class EBossState
{
	Groggy,		// 그로기 상태
	Attack,		// 공격 
	Move,		// Teleport 
};

// 보스가 월드에서 차지하는 공간 
struct FGridSize
{
	FGridSize(uint8_t X, uint8_t Y)
	{
		// 오버플로우 방지
		m_iX = MathUtil::Clamp<int>(X, 1, UINT8_MAX);
		m_iY = MathUtil::Clamp<int>(X, 1, UINT8_MAX);
	}

	uint8_t m_iX; // X칸 
	uint8_t m_iY; // Y칸
};

class CBoss : public CCharacter
{
public:
	CBoss(int Shape, int Color, FGridSize BossSize);

	virtual ~CBoss();

public:
	// 액터 기본 기능 
	virtual void Tick(double DeltaTime) override;
	virtual void Move() {};
	virtual void Render() override;
public:
	virtual void Attack(COORD Direction) override
	{
		SelectAttackPattern();
	}

	virtual void OnHit(float Damage) override;


protected:
	// 랜덤 공격 패턴 선택 함수 / 어택해서 호출 
	virtual void SelectAttackPattern();

	// 플레이어 좌표기준 가장 멀리있는 부분 
	virtual COORD FindCanTelportPosition(/*CPlayer* Player*/);

	// 텔레포트 함수
	virtual void Teleport();

	// 플레이어 8방향 
	virtual void FireProjectileToOutline();

	// 사각형 형태의 보스의 Range  오프세만큼의 아웃라인 구하기
	std::vector<FAttackPos> GetBossOutlineAttackRange(int Range);

	// 플레이어 위치기준 한방향 최대 5번 단계저 공격 
	// ㅁ 모양이 커지는 식으로 공격
	// AttackInRagne의 범위를 여러번 호출해서 파도타기 같은 느낌으로 구현 
	// virtual void GetBossOutlineAttackRange();


	void WaveAttack();


protected:
	virtual void ChangeState(EBossState NewState, float Delay = 1.f);

	virtual void GroggyAction();
	virtual void AttackAction();
	virtual void MoveAction();

public:
	// 체력 유틸
	// 체력 상태 확인 
	bool IsDead() const
	{
		return 0 == m_fCurrentHealth;
	}
	 
	// [TODO-PJH] : 스탯관련은 러프하게 작성 -> 선재님 만드는거 기준으로 변경 필요
	bool IsGroggyState() const
	{
		return m_fCurrentGroggy == m_fMaxGroggy;
	}

protected:
	EBossState m_CurrentState = EBossState::Attack;

	float m_fCurrentHealth;				// 체력
	float m_fMaxHealth;					// 체력

	float m_fCurrentGroggy;				// 현재 그로기 
	float m_fMaxGroggy;					// 맥스 그로기 

	float m_fAccGroggy = 0.f;
	float m_fGroggyTime = 3.f;

	double m_fAccStateActionDelay = 0;
	double m_fStateActionDelay = 0;

	FGridSize m_BossSize = {2, 2};		// 보스가 월드에서 차지하는 크기 


	// 웨이브 스킬 쿨타임 
	bool bActive;
	int m_iWaveAttackTriggerCooldown = 1.5f;
	int m_iAccWaveAttackTriggerCooldown = 0.2f;
	int m_iCurrntWaveCount = 1;
	int m_iMaxWaveCount = 5;
	bool m_bIsActiveWaveAttack = false;

	std::vector<shared_ptr<class CProjectile>> m_vProjectiles;
};

