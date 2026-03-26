#pragma once

#include "../Character/Character.h"

class CProjectile : public CActor
{
public:
	CProjectile(int Shape, int Color);

	void Tick(double DeltaTime) override;
	void Move() override;

	void SetOwner(CCharacter* Owner) { m_pOwner = Owner; }

	const bool IsActive() const { return m_bIsActive; }

private:
	CCharacter* m_pOwner; // 투사체의 소유자 (발사한 캐릭터)
	bool m_bIsActive;     // 투사체 활성 상태

};
