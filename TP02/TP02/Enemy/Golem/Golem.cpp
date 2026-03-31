#include "Golem.h"
#include "../../Graphics/Effect.h"

#define GOLEM_MOVE	5.0

CGolem::CGolem() : CEnemy(Pixel::Golem, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_GRAY)
{
	m_fSpeed = 0.5f;
	m_fAttackPower = 80.f;
	m_dMoveTimer = (float)rand() / RAND_MAX + 1.0f;
	m_eTag = ETag::actor | ETag::character | ETag::monster;

	//	체력 설정 해야함

	m_aMaterial.push_back(make_pair(Pixel::cross, TEXT_FOREGROUND_YELLOW_INT | TEXT_BACKGROUND_YELLOW));
	m_aMaterial.push_back(make_pair(Pixel::dust, TEXT_FOREGROUND_YELLOW_INT | TEXT_BACKGROUND_YELLOW));
	m_aMaterial.push_back(make_pair(Pixel::dust, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_BLACK));
	m_aDuration.push_back(0.1);
	m_aDuration.push_back(0.3);
	m_aDuration.push_back(0.6);
}

CGolem::~CGolem()
{

}

void CGolem::Tick(double deltaTime)
{
	m_dMoveTimer -= deltaTime;
	bool attack = m_dMoveTimer <= 0.0;
	CEnemy::Move();

	if (attack)	Attack();
}

void CGolem::Attack()
{	//	플레이어에게만 피해를 줍니다.
	auto player = CGameWorld::GetInstance()->GetPlayerActor();
	if (!player)	return;

	COORD pos = player.get()->GetPosition();

	//	5*5범위에 피해주기
	if (abs(m_cPosition.X - pos.X) < 3 && abs(m_cPosition.Y - pos.Y) < 3)
	{
		player.get()->OnHit(m_fAttackPower);
	}

	//	이펙트 출력하기
	shared_ptr<CEffect> FX = make_shared<CEffect>();
	vector<pair<COORD, COORD>> shape;
	shape.push_back(make_pair(COORD(m_cPosition.X - 1, m_cPosition.Y - 1), COORD(m_cPosition.X + 1, m_cPosition.Y + 1)));
	shape.push_back(make_pair(COORD(m_cPosition.X - 2, m_cPosition.Y - 2), COORD(m_cPosition.X + 2, m_cPosition.Y + 2)));
	shape.push_back(make_pair(COORD(m_cPosition.X - 2, m_cPosition.Y - 2), COORD(m_cPosition.X + 2, m_cPosition.Y + 2)));
	FX->CreateDynamicEffect(4, m_aMaterial, shape, m_aDuration);
	FX->RemoveTag(ETag::effect);
	FX->AddTag(ETag::environment);
	CGameWorld::GetInstance()->AddActor(FX);
}