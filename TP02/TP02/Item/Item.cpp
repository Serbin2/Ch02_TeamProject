#include "Item.h"
#include <iostream>

CItem::CItem(const std::string& sName, const std::string& sDesc, int iPrice, int iMaxAmount)
	: m_sName(sName), m_sDesc(sDesc), m_iPrice(iPrice), m_iMaxAmount(iMaxAmount)
{

}

void CItem::SetPrice(int itemPrice)
{
	if (itemPrice >= 0) m_iPrice = itemPrice;
}


