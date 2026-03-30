// [작성자] : 박재현
// [Desc] : 월드에 배치되고

#pragma once
#include "Item.h"
#include "../World/GameWorld.h"
#include "../Character/Actor.h"


class CBomb : public CActor	
{
public:
	CBomb() = default;
	CBomb(int Shape, int Color, const COORD& Pos);
	~CBomb() = default;

public:
	virtual void Tick(double DeltaTime) override;
	virtual void Move() override {};
	//virtual void Render() override;

private:
	std::vector<COORD> GetBombOutlinePos(int Range);
	void Burst();

private:
	double m_dLifeTime = 2.5;

	std::vector<std::pair<COORD, COORD>> m_vAdge;
};

class CBombItem : public CItem
{

public:
	CBombItem();

public:
	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer) override;
};



