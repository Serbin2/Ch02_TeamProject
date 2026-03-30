#include "Item.h"
#include <iostream>

CItem::CItem(std::string itemName, std::string itemDesc, int itemPrice, int itemCount)
	: sName(itemName), sDesc(itemDesc), iPrice(itemPrice), iCount(itemCount)
{
}

CItem::~CItem() = default;

std::string CItem::sGetName() const { return sName; }
std::string CItem::sGetDesc() const { return sDesc; }
int CItem::GetPrice() const { return iPrice; }
int CItem::GetCount() const { return iCount; }

void CItem::SetPrice(int itemPrice)
{
	if (itemPrice >= 0) iPrice = itemPrice;
}

void CItem::GetItem(int amount)
{
	iCount += amount;
}

void CItem::ConsumeItem(int amount)
{
	if (iCount >= amount) iCount -= amount;
}

void CItem::UseItem()
{
	if (iCount <= 0) return;

	std::cout << sName << " 사용!\n";
	ConsumeItem();
}