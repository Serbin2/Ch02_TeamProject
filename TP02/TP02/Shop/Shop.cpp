#include "Shop.h"
#include "../Character/Player.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "../Inventory/Inventory.h"
#include "../Graphics/Interface.h"

// 콘솔 가운데 정렬용 계산
static std::string GetPad(int contentWidth)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int startX = (width - contentWidth) / 2;
	if (startX < 0) startX = 0;
	return std::string(startX, ' ');
}

// 콘솔 세로 가운데 정렬용 빈 줄 출력
static void PrintTopPadding(int lineCount)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	int startY = (height - lineCount) / 2;
	if (startY < 0) startY = 0;
	for (int i = 0; i < startY; i++)
		std::cout << "\n";
}

// 메시지를 출력하고 일정 시간 대기 (자동으로 넘어감)
static void ShowMessageAndWait(const std::string& pad, const std::string& message, int waitMs = 2000)
{
	system("cls");
	PrintTopPadding(1);
	std::cout << pad << message << "\n";
	Sleep(waitMs);
}

CShop::CShop()
{
	InitShopItems();
}

void CShop::InitShopItems()
{
	m_ShopItems.push_back(std::make_shared<Potion>("체력 포션 (소)", "체력을 30 hp 회복한다.", 50, 1, 30));
	m_ShopItems.push_back(std::make_shared<Potion>("체력 포션 (중)", "체력을 80 hp 회복한다.", 120, 1, 80));
	m_ShopItems.push_back(std::make_shared<Potion>("체력 포션 (대)", "체력을 200 hp 회복한다.", 300, 1, 200));
}

// 상점 입장
void CShop::Enter(CPlayer* pPlayer)
{
	while (true)
	{
		system("cls");
		ShowShopUI(pPlayer);

		std::string pad = GetPad(30);
		std::cout << "\n";
		std::cout << pad << "[1] 구매  [2] 판매  [0] 나가기\n";
		std::cout << pad << "선택 : ";

		char key = _getch();

		// 1, 2, 0 외의 키는 무시 (잔상 없이)
		if (key != '1' && key != '2' && key != '0')
			continue;

		// 누른 키 표시 후 800ms 대기 → 보였다가 사라지는 느낌
		std::cout << key << "\n";
		Sleep(800);

		if (key == '1') BuyMenu(pPlayer);
		else if (key == '2') SellMenu(pPlayer);
		else if (key == '0') break;
	}

	// 상점 나갈 때 화면 지우고 인벤토리 빨간 박스 복구
	system("cls");
	pPlayer->GetInventory()->UpdateUI();
	CInterface::GetInstance()->Redraw();
}

// 상점 UI 출력
void CShop::ShowShopUI(CPlayer* pPlayer)
{
	int contentWidth = 32;
	std::string pad = GetPad(contentWidth);

	int lineCount = 6 + (int)m_ShopItems.size() * 2 + 3;
	PrintTopPadding(lineCount);

	std::cout << pad << "================================\n";
	std::cout << pad << "           상    점             \n";
	std::cout << pad << "================================\n";
	std::cout << pad << "  보유 골드 : " << pPlayer->GetGold() << " Gold\n";
	std::cout << pad << "================================\n";
	std::cout << pad << "  [판매 목록]\n";
	for (int i = 0; i < (int)m_ShopItems.size(); i++)
	{
		int sellPrice = (int)(m_ShopItems[i]->GetPrice() * 0.6f);
		std::cout << pad << "  " << (i + 1) << ". "
			<< m_ShopItems[i]->GetName()
			<< "  구매: " << m_ShopItems[i]->GetPrice() << "G"
			<< "  판매: " << sellPrice << "G\n";
		std::cout << pad << "      " << m_ShopItems[i]->GetDesc() << "\n";
	}
	std::cout << pad << "================================\n";
}

// 구매 메뉴
void CShop::BuyMenu(CPlayer* pPlayer)
{
	system("cls");
	ShowShopUI(pPlayer);
	std::string pad = GetPad(32);
	std::cout << "\n" << pad << "구매할 아이템 번호 입력 (0: 취소) : ";

	char key = _getch();

	// 유효한 번호 또는 0만 허용
	if (key != '0' && (key - '1') >= (int)m_ShopItems.size())
	{
		std::cout << key << "\n";
		Sleep(800);
		ShowMessageAndWait(pad, "잘못된 번호입니다.");
		return;
	}

	std::cout << key << "\n";
	Sleep(800);

	if (key == '0') return;

	int index = key - '1';
	BuyItem(pPlayer, index);
}

// 판매 메뉴
void CShop::SellMenu(CPlayer* pPlayer)
{
	system("cls");
	int contentWidth = 32;
	std::string pad = GetPad(contentWidth);

	int lineCount = 5 + (int)pPlayer->GetInventory()->GetSize() + 3;
	PrintTopPadding(lineCount);

	std::cout << pad << "================================\n";
	std::cout << pad << "        보유 아이템 목록         \n";
	std::cout << pad << "================================\n";
	std::cout << pad << "  보유 골드 : " << pPlayer->GetGold() << " Gold\n";
	std::cout << pad << "================================\n";

	auto inv = pPlayer->GetInventory();
	if (inv->IsEmpty())
	{
		std::cout << pad << "  보유 중인 아이템이 없습니다.\n";
		std::cout << pad << "================================\n";
		Sleep(2000);
		return;
	}

	for (int i = 0; i < (int)inv->GetSize(); i++)
	{
		if (inv->GetItem(i))
		{
			int sellPrice = (int)(inv->GetItem(i)->GetPrice() * 0.6f);
			std::cout << pad << "  " << (i + 1) << ". "
				<< inv->GetItem(i)->GetName()
				<< "  판매가: " << sellPrice << "G\n";
		}
	}
	std::cout << pad << "================================\n";
	std::cout << "\n" << pad << "판매할 아이템 번호 입력 (0: 취소) : ";

	char key = _getch();
	std::cout << key << "\n";
	Sleep(800);

	if (key == '0') return;

	int index = key - '1';
	SellItem(pPlayer, index);
}

// 실제 구매 처리
void CShop::BuyItem(CPlayer* pPlayer, int index)
{
	std::string pad = GetPad(32);

	if (index < 0 || index >= (int)m_ShopItems.size())
	{
		ShowMessageAndWait(pad, "잘못된 번호입니다.");
		return;
	}

	int price = m_ShopItems[index]->GetPrice();

	if (!pPlayer->SpendGold(price))
	{
		std::string msg = "골드가 부족합니다! (필요: "
			+ std::to_string(price)
			+ "G, 보유: "
			+ std::to_string(pPlayer->GetGold()) + "G)";
		ShowMessageAndWait(pad, msg);
		return;
	}

	Potion* original = dynamic_cast<Potion*>(m_ShopItems[index].get());
	if (original)
	{
		auto newPotion = std::make_shared<Potion>(*original);
		pPlayer->GetInventory()->AddItem(newPotion, 1);
	}

	std::string msg = "[" + m_ShopItems[index]->GetName() + "] 을(를) 구매했습니다!";
	ShowMessageAndWait(pad, msg);  // 2초 표시
}

// 실제 판매 처리
void CShop::SellItem(CPlayer* pPlayer, int index)
{
	std::string pad = GetPad(32);
	auto inv = pPlayer->GetInventory();

	if (index < 0 || index >= (int)inv->GetSize() || !inv->GetItem(index))
	{
		ShowMessageAndWait(pad, "잘못된 번호입니다.");
		return;
	}

	int sellPrice = (int)(inv->GetItem(index)->GetPrice() * 0.6f);
	std::string name = inv->GetItem(index)->GetName();

	inv->RemoveItem(index, 1);
	pPlayer->AddGold(sellPrice);

	std::string msg = "[" + name + "] 을(를) " + std::to_string(sellPrice) + "G에 판매했습니다!";
	ShowMessageAndWait(pad, msg);  // 2초 표시
}