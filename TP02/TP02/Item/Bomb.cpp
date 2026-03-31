#include "Bomb.h"
#include "../Graphics/Effect.h"

#include "../Manager/SoundManager/SoundManager.h"

CBomb::CBomb(int Shape, int Color, const COORD& Pos)
	: CActor(Pixel::star, TEXT_BACKGROUND_MAGENTA | TEXT_FOREGROUND_CYAN)
{
	m_eTag = ETag::actor; 
	m_sName = "폭탄";

	m_cPosition =  Pos;

	for (int i = 1; i <= 3; ++i)
	{
		std::pair<COORD, COORD> Adge;

		Adge.first = { (short)(m_cPosition.X - 1 * i), (short)(m_cPosition.Y - 1 * i) };
		Adge.second = { (short)(m_cPosition.X + 1 * i), (short)(m_cPosition.Y + 1 * i) };

		m_vAdge.push_back(Adge);
	}

	shared_ptr<CEffect> FX = make_shared<CEffect>();
	vector<pair<int, int >> material;
	//material.push_back(make_pair(Pixel::cross, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
	material.push_back(make_pair(Pixel::cross, TEXT_FOREGROUND_RED | TEXT_BACKGROUND_YELLOW));
	material.push_back(make_pair(Pixel::cross, TEXT_FOREGROUND_RED | TEXT_BACKGROUND_YELLOW));
	material.push_back(make_pair(Pixel::square, TEXT_FOREGROUND_RED_INT | TEXT_BACKGROUND_RED));
	//material.push_back(make_pair(Pixel::dust, TEXT_FOREGROUND_YELLOW_INT | TEXT_BACKGROUND_YELLOW));
	vector<double> duration;
	duration.push_back(1.0);
	duration.push_back(1.0);
	duration.push_back(1.0);
	FX->CreateDynamicEffect(3, material, m_vAdge, duration);
	CGameWorld::GetInstance()->AddActor(FX);
}

void CBomb::Tick(double DeltaTime)
{
	m_dLifeTime -= DeltaTime;
	if (m_dLifeTime <= 0)
	{
		// 데미지 입히는 로직 
		Burst();
		m_bIsValid = false;
	}
}

std::vector<COORD> CBomb::GetBombOutlinePos(int Range)
{
	const COORD pos = GetPosition();
	const int width = 1;
	const int height = 1;

	std::vector<COORD> result;

	const int left = pos.X - Range;
	const int right = pos.X + width + Range - 1;
	const int top = pos.Y - Range;
	const int bottom = pos.Y + height + Range - 1;

	// Top Edge
	for (int x = left; x <= right; ++x)
	{
		result.push_back({(SHORT)x, (SHORT)top});
	}

	// Bottom Edge
	for (int x = left; x <= right; ++x)
	{
		result.push_back({(SHORT)x, (SHORT)bottom});
	}

	// Left Edge
	for (int y = top + 1; y < bottom; ++y)
	{
		result.push_back({(SHORT)left, (SHORT)y});
	}

	// Right Edge
	for (int y = top + 1; y < bottom; ++y)
	{
		result.push_back({(SHORT)right, (SHORT)y});
	}

	return result;
}

void CBomb::Burst()
{
	std::vector<COORD> CheckPoint;
	for (int i = 1; i <= 3; ++i)
	{
		std::vector<COORD> Outline = GetBombOutlinePos(i);
		CheckPoint.insert(CheckPoint.end(), Outline.begin(), Outline.end());
	}
	CheckPoint.push_back(GetPosition());

	for (const auto& Point : CheckPoint)
	{
		if (Point.X < 0 || 30 <= Point.X
			|| Point.Y < 0 || 30 <= Point.Y)
		{
			continue;
		}

		auto pActor = CGameWorld::GetInstance()->FindActorFromPosition(Point);
		if (!pActor)
		{
			continue;
		}

		auto pCharacter = std::static_pointer_cast<CCharacter>(pActor);
		if (!pCharacter)
		{
			continue;
		}

		pCharacter->OnHit(100.f);
	}

	GET_SINGLE(CSoundManager)->PlaySFX(L"Boom");

}

CBombItem::CBombItem()
{
	m_sName = "폭탄";
	m_sDesc = "폭탄기준 3의 추가영역에 데미지";
	m_iPrice = 400;
	m_iMaxAmount = 10;
}

void CBombItem::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	// 유효성 확인
	std::shared_ptr<CPlayer> Player = pPlayer.lock();
	if (!Player)
	{
		return;
	}

	const COORD& SpawnPosition = Player->GetPosition();
	std::shared_ptr<CActor> Bomb = std::make_shared<CBomb>(Pixel::star, TEXT_BACKGROUND_MAGENTA | TEXT_FOREGROUND_CYAN, SpawnPosition);
	if (!Bomb)
	{
		return;
	}

	Bomb->SetPosition(SpawnPosition);
	CGameWorld::GetInstance()->AddActor(Bomb);
}