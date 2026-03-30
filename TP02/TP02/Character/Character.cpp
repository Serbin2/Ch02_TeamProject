#include "Character.h"
#include "../Utils/Utils.h"
#include "../Graphics/Interface.h"

void CCharacter::SetHealth(float NewHealth)
{
	float ClampedValue = MathUtil::Clamp<float>(NewHealth, 0, GetMaxHealth());
	m_fHealth = ClampedValue;

	// 플레이어의 경우 체력 변경시 업데이트
	if (HasTag(ETag::player)) CInterface::GetInstance()->SetValue(2, m_fHealth);
}
