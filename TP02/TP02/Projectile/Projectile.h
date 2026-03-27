#pragma once

#include "../Character/Character.h"

class CProjectile : public CActor
{
public:
	CProjectile(int Shape, int Color);
	~CProjectile() = default;

	void Tick(double DeltaTime) override;
	void Move() override;
	void CheckCollision();

	void SetOwner(std::weak_ptr<CCharacter> Owner) { m_pOwner = Owner; }

protected:
	std::weak_ptr<CCharacter> m_pOwner; // 투사체의 소유자 (발사한 캐릭터)

};
