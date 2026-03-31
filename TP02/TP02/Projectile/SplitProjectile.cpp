#include "SplitProjectile.h"

CSplitProjectile::CSplitProjectile() : CProjectile(Pixel::circle, TEXT_FOREGROUND_CYAN) { }

void CSplitProjectile::CheckCollision()
{
	auto pActor = CGameWorld::GetInstance()->FindActorFromPosition(m_cPosition, ETag::monster);
	if (!pActor) pActor = CGameWorld::GetInstance()->FindActorByActorCustom(m_cPosition, ETag::monster);
	if (!pActor)	return;

	auto pCharacter = std::dynamic_pointer_cast<CCharacter>(pActor);
	if (!pCharacter)
		return;

	auto pOwner = m_pOwner.lock();
	if (pOwner && pOwner.get() != pCharacter.get())
	{
		// 대각선 4방향으로 분열 (좌상, 우상, 좌하, 우하)
		const COORD directions[4] = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };

		pCharacter->OnHit(pOwner->GetAttackPower());

		for (int i = 0; i < 4; ++i)
		{
			// 무한 분열을 막기 위해 파편은 기본 투사체(CProjectile)로 생성
			auto pFragment = std::make_shared<CProjectile>(Pixel::circle, TEXT_FOREGROUND_CYAN);
			pFragment->SetOwner(m_pOwner);
			pFragment->SetPosition(m_cPosition); // 적과 충돌한 현재 위치에서 생성
			pFragment->SetSpeed(m_fSpeed);       // 본체와 동일한 속도 유지
			pFragment->SetMoveDirection(directions[i]);
			CGameWorld::GetInstance()->AddActor(pFragment);
		}

		m_bIsValid = false; // 본체 투사체는 충돌 후 제거
	}
}
