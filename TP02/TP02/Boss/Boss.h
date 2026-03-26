// [작성자] : 박재현
// [Desc] : 보스 기본 클래스 
// [TODO] : 선재님 파일 오면 해당 파일 참조로 변경

#pragma once
#include "../Standard.h"
#include "../Actor.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Utils/Utils.h"

class CPlayer;

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

class CBoss //: public CCharacter
{
public:
	CBoss();

	CBoss(CGraphic::Pixel Shape, CGraphic::TextColor Color, FGridSize BossSize)
		: CActor(Shape, Color)
	{
		m_BossSize = BossSize;
	};

	virtual ~CBoss();

public:
	// 액터 기본 기능 
	virtual void Tick(float DeltaTime) override;
	virtual void Move() override;					// 텔레포트 
	virtual void Render() override;


public:
	virtual void Attack(COORD Direction)
	{
		SelectAttackPattern();
	}

	virtual void OnHit(float Damage) = 0;

protected:
	virtual void ChangeState(EBossState NewState);

public:
	// 체력 유틸
	// 체력 상태 확인 
	bool IsDead() const
	{
		return 0 == m_fCurrentHealth;
	}
	 
	// [TOOD] : 스탯관련은 러프하게 작성 -> 선재님 만드는거 기준으로 변경 필요
	bool IsGroggyState() const
	{
		return m_fCurrentGroggy == m_fMaxGroggy;
	}

protected:
	// 랜덤 공격 패턴 선택 함수 / 어택해서 호출 
	virtual void SelectAttackPattern();

	// 텔레포트 가능한 위치 
	virtual void FindCanTelport(CPlayer* Player);

	// 텔레포트 함수
	virtual void Teleport(COORD Position);

protected:
	EBossState m_BossState;

	float m_fCurrentHealth;				// 체력
	float m_fMaxHealth;					// 체력

	float m_fCurrentGroggy;				// 현재 그로기 
	float m_fMaxGroggy;					// 맥스 그로기 

	float m_fMoveCoolTime;;				// 텔레포트 쿨타임

	FGridSize m_BossSize = {2, 2};		// 보스가 월드에서 차지하는 크기 
};

