#pragma once

#include "../Character/Character.h"

class CProjectile : public CActor
{
public:
	CProjectile(int Shape, int Color);
	~CProjectile() { };

	void Tick(double DeltaTime) override;
	void Move() override;

	void CheckCollision();

	std::weak_ptr<CCharacter> GetOwner() const { return m_pOwner; }
	void SetOwner(std::weak_ptr<CCharacter> Owner) { m_pOwner = Owner; }

protected:
	std::weak_ptr<CCharacter> m_pOwner; // 투사체의 소유자 (발사한 캐릭터)

};
