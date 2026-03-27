// [작성자] : 박다현
// [Desc] : 아이템 베이스 클래스


#pragma once
#include <string>
#include <iostream>
#include <memory>

#include "../Character/Character.h"


class cItem
{
protected: // 이름, 설명, 가격
	std::string sName;
	std::string sDesc;
	int iPrice;
	int iCount;

public:
	cItem(std::string n = "Default", std::string d = "default", int p = 0, int c = 0)
		: sName(n), sDesc(d), iPrice(p), iCount(c) {
	}
	virtual ~cItem() = default;

	//getter
	std::string sGetName() const { return sName; }
	std::string sGetDesc() const { return sDesc; }
	int GetPrice() const { return iPrice; }
	int GetCount() const { return iCount; }


	//만약을 위한 setter (가격 변경)
	void SetPrice(int p) { if (p >= 0) iPrice = p; }


	// 개수 처리
	void GetItem(int amount = 1) { iCount += amount; } // 아이템획득
	void ConsumeItem(int amount = 1) { //아이템 사용
		if (iCount >= amount) iCount -= amount;
	}
	//아이템 사용시 효과
	virtual void UseItem() {
		if (iCount <= 0) return;
		std::cout << sName << " 사용!\n";

		ConsumeItem();
	}


};

//예시 체력 회복포션
class Potion : public cItem
{
private:
	int iHeal;

public:
	Potion(std::string n = "체력 포션",
		std::string d = "체력을 50 hp 회복한다.",
		int p = 100, int c = 1, int h = 50)
		: cItem(n, d, p, c), iHeal(h) {
	}

	void UseItem(CCharacter& player) {
		if (iCount <= 0) return;

		float health = player.GetHealth();
		health += iHeal;
		player.SetHealth(health);

		std::cout << sName << " 사용! 체력을 "
			<< iHeal << "hp 회복했습니다.\n";

		ConsumeItem();
	}
};