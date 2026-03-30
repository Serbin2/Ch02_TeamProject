#pragma once
#include <string>
#include <iostream>

class CItem
{
protected:
	std::string sName;
	std::string sDesc;
	int iPrice;
	int iCount;

public:
	CItem(std::string itemName = "Default",
		std::string itemDesc = "default",
		int itemPrice = 0,
		int itemCount = 0);
	virtual ~CItem();

	std::string sGetName() const;
	std::string sGetDesc() const;
	int GetPrice() const;
	int GetCount() const;

	void SetPrice(int itemPrice);
	void GetItem(int amount = 1);
	void ConsumeItem(int amount = 1);

	virtual void UseItem();
};