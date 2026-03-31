#include "Character.h"
#include "../Utils/Utils.h"
#include "../Graphics/Interface.h"
#include <format>

void CCharacter::SetHealth(float NewHealth)
{
	float ClampedValue = MathUtil::Clamp<float>(NewHealth, 0, GetMaxHealth());
	m_fHealth = ClampedValue;

	// 플레이어의 경우 체력 변경시 업데이트
	if (HasTag(ETag::player))
	{
		string sHP = format("{:.1f}", m_fHealth) + " / " + format("{:.1f}", m_fMaxHealth);
		CInterface::GetInstance()->SetValue(2, sHP);
	}
}

void CCharacter::SetMaxHealth(float NewMaxHealth)
{
	m_fMaxHealth = NewMaxHealth; 
	if (HasTag(ETag::player))
	{
		string sHP = format("{:.1f}", m_fHealth) + " / " + format("{:.1f}", m_fMaxHealth);
		CInterface::GetInstance()->SetValue(2, sHP);
	}
}
