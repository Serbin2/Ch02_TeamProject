#include "Boss.h"

#include "../Graphics/ConsoleGraphic.h"

void CBoss::Tick(float DeltaTime)
{
	m_fAccStateActionDelay += DeltaTime;


	if (m_fAccStateActionDelay >= m_fStateActionDelay)
	{
		m_fStateActionDelay = 0.f;

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
	ChangeState(EBossState::Move, 5);
}

void CBoss::MoveAction()
{
	// 텔포 -> 공격 스테이 후 
	ChangeState(EBossState::Attack, 2);
}
