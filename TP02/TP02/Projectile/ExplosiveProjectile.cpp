#include "ExplosiveProjectile.h"
#include "../Graphics/Effect.h"

CExplosiveProjectile::CExplosiveProjectile() : CProjectile(Pixel::circle, TEXT_FOREGROUND_RED) { }

void CExplosiveProjectile::CheckCollision()
{
	auto pGameWorld = CGameWorld::GetInstance();
	auto pActor = pGameWorld->FindActorFromPosition(m_cPosition);
	if (!pActor || !pActor->HasTag(ETag::character))
		return;

	auto pCharacter = std::dynamic_pointer_cast<CCharacter>(pActor);
	if (!pCharacter)
		return;

	auto pOwner = m_pOwner.lock();
	if (pOwner && pOwner.get() != pCharacter.get())
	{
		float baseDamage = pOwner->GetAttackPower();
		float splashDamage = baseDamage * 0.5f;
		pCharacter->OnHit(baseDamage);

		// 주변 3x3 영역 탐색 및 스플래시 데미지 적용
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				// 정중앙(0, 0)은 이미 데미지를 주었으므로 제외
				if (x == 0 && y == 0)
					continue;

				COORD splashPos(m_cPosition.X + x, m_cPosition.Y + y);

				if (splashPos.X >= 0 && splashPos.X < 30 && splashPos.Y >= 0 && splashPos.Y < 30)
				{
					auto pSplashActor = pGameWorld->FindActorFromPosition(splashPos);
					if (pSplashActor == nullptr) pSplashActor = pGameWorld->FindActorByActorCustom(splashPos);
					if (pSplashActor == nullptr || !pSplashActor->HasTag(ETag::character))
						continue;

					auto pSplashTarget = std::dynamic_pointer_cast<CCharacter>(pSplashActor);
					if (pSplashTarget == nullptr || pOwner.get() == pSplashTarget.get())
						continue;
					
					pSplashTarget->OnHit(splashDamage);
				}
			}
		}

		auto pFX = std::make_shared<CEffect>();
		COORD LT = { m_cPosition.X - 1, m_cPosition.Y - 1 };
		COORD RB = { m_cPosition.X + 1, m_cPosition.Y + 1 };

		pFX->CreateStaticEffect(Pixel::circle, TEXT_FOREGROUND_RED | TEXT_FOREGROUND_RED_INT, LT, RB, 0.5);
		pGameWorld->AddActor(pFX);
		m_bIsValid = false;
	}
}
