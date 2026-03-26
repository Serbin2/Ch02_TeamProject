#include "Player.h"
#include "../Input/Input.h"

CPlayer::CPlayer(int Shape, int Color) : CCharacter(Shape, Color)
{
	m_cPosition = { 15, 15 };
	m_fHealth = 100.0f;
	m_fAttackPower = 15.0f;
	m_fDefense = 5.0f;
	m_iAttackRange = 1;
	m_fAttackCooldown = 0.0f;
	m_iLevel = 1;
	m_iExp = 0;
}

void CPlayer::Tick(double DeltaTime)
{
	Input();
	Render();
}

void CPlayer::Move()
{
	// TODO: 플레이어 위치 변경, 맵 경계 체크
}

void CPlayer::Attack(COORD Direction)
{
	// TODO: 공격 로직 구현, 공격 쿨타임 체크, 공격 범위 내 적 탐색 및 피해 적용
}

void CPlayer::OnHit(float Damage)
{
	// TODO: 피해 계산, 체력 감소, 사망 처리, 피격 효과 등 구현
}

void CPlayer::Input()
{
	CInput* pInput = CInput::GetInstance();
	pInput->Update();
	
	// 콘솔 그래픽은 4사분면((0, 0)의 위치가 좌상단)이므로 y축이 반대로 동작해야합니다.
	// Key Code: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

	// Move input
	if (pInput->IsKeyDown('W'))	m_cMoveDirection = { 0, -1 };
	if (pInput->IsKeyDown('S')) m_cMoveDirection = { 0,  1 };
	if (pInput->IsKeyDown('A')) m_cMoveDirection = { -1, 0 };
	if (pInput->IsKeyDown('D')) m_cMoveDirection = { 1,  0 };

	// Attack input
	if (pInput->IsKeyDown(VK_UP))    Attack({ 0, -1 }); // [↑]
	if (pInput->IsKeyDown(VK_DOWN))  Attack({ 0,  1 }); // [↓]
	if (pInput->IsKeyDown(VK_LEFT))	 Attack({ -1, 0 }); // [←]
	if (pInput->IsKeyDown(VK_RIGHT)) Attack({ 1,  0 }); // [→]
}
