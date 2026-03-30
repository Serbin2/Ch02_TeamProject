#pragma once

#include "../Character/Character.h"

class CProjectile : public CActor
{
public:
	CProjectile() : CProjectile(Pixel::circle, TEXT_FOREGROUND_YELLOW) { }
	CProjectile(int Shape, int Color);
	~CProjectile() = default;

	virtual std::shared_ptr<CProjectile> Clone() { return std::make_shared<CProjectile>(*this); }

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void CheckCollision() override;

	std::weak_ptr<CCharacter> GetOwner() const { return m_pOwner; }
	void SetOwner(std::weak_ptr<CCharacter> Owner) { m_pOwner = Owner; }

protected:
	std::weak_ptr<CCharacter> m_pOwner; // 투사체의 소유자 (발사한 캐릭터)

};
