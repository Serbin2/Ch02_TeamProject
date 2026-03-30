// [작성자] : 강민서
// [Desc] : 상점
#pragma once
#include <vector>
#include <memory>
#include "../Item/Item.h"

class CPlayer;

class CShop
{
public:
	CShop();
	void Enter(CPlayer* pPlayer);		// 상점 입장

private:
	void ShowShopUI(CPlayer* pPlayer);	// 상점 UI 출력
	void BuyMenu(CPlayer* pPlayer);		// 구매 메뉴
	void SellMenu(CPlayer* pPlayer);	// 판매 메뉴
	void BuyItem(CPlayer* pPlayer, int index);	// 구매 처리
	void SellItem(CPlayer* pPlayer, int index);	// 판매 처리

	void InitShopItems();				// 판매 목록 초기화

	std::vector<std::shared_ptr<CItem>> m_ShopItems;	// 상점 판매 목록
};