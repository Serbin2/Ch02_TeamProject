#include "Shop.h"
#include "../Character/Player.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "../Inventory/Inventory.h"

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

CShop::CShop()
{
	InitShopItems();
}

void CShop::InitShopItems()
{
	// 상점 판매 아이템 목록
	// 아이템 추가는 여기에 작성 가능
	m_ShopItems.push_back(std::make_shared<Potion>("체력 포션 (소)", "체력을 30 hp 회복한다.", 50, 1, 30));
	m_ShopItems.push_back(std::make_shared<Potion>("체력 포션 (중)", "체력을 80 hp 회복한다.", 120, 1, 80));
	m_ShopItems.push_back(std::make_shared<Potion>("체력 포션 (대)", "체력을 200 hp 회복한다.", 300, 1, 200));
}

//상점 입장 : 플레이어가 상점에 들어왔을 때 무한 루프를 돌며 메뉴
void CShop::Enter(CPlayer* pPlayer)
{
	while (true) //나가기전까지 계속 상점 상태 유지
	{
		system("cls");
		ShowShopUI(pPlayer); // 상점화면을 그려줌

		std::string pad = GetPad(30); // 가운대 정렬용 여백 계산
		std::cout << "\n";
		std::cout << pad << "[1] 구매  [2] 판매  [0] 나가기\n";
		std::cout << pad << "선택 : ";

		char key = _getch(); // 키보드 입력을 받음
		if (key == '1') BuyMenu(pPlayer); //1번 누르면 구매 메뉴
		else if (key == '2') SellMenu(pPlayer); //2번 누르면 판매 메뉴
		else if (key == '0') break; //0번 누르면 상점 나감
	}
}

//구매 메뉴: 어떤 물건을 살지 번호를 입력받는 곳
void CShop::ShowShopUI(CPlayer* pPlayer) // 구매 메뉴 어떤 물건을 살지
{
	int contentWidth = 32;
	std::string pad = GetPad(contentWidth);

	// 아이템 줄 수 계산 (헤더 5줄 + 아이템 2줄씩 + 하단 1줄 + 메뉴 2줄)
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

// 실제 구매 처리: 돈이 있는지 확인하고 가방에 넣음
void CShop::BuyMenu(CPlayer* pPlayer)
{
	system("cls");
	ShowShopUI(pPlayer);
	std::string pad = GetPad(32);
	std::cout << "\n" << pad << "구매할 아이템 번호 입력 (0: 취소) : ";
	int index;
	std::cin >> index;
	if (index == 0) return;
	BuyItem(pPlayer, index - 1);
}

// 실제 판매 처리: 플레이어 가방에서 빼고 돈을 뻄
void CShop::SellMenu(CPlayer* pPlayer)
{
	system("cls");
	int contentWidth = 32;
	std::string pad = GetPad(contentWidth);

	int lineCount = 5 + (int)pPlayer->GetInventory()->GetSize() + 3;
	PrintTopPadding(lineCount);

	std::cout << pad << "================================\n";
	std::cout << pad << "       보유 아이템 목록         \n";
	std::cout << pad << "================================\n";
	std::cout << pad << "  보유 골드 : " << pPlayer->GetGold() << " G\n";
	std::cout << pad << "================================\n";

	auto inv = pPlayer->GetInventory();
	if (inv->IsEmpty())
	{
		std::cout << pad << "  보유 중인 아이템이 없습니다.\n";
		std::cout << pad << "================================\n";
		std::cin.get();
		return;
	}

	for (int i = 0; i < (int)inv->GetSize(); i++)
	{
		if (inv->GetItem(i))
		{
			int sellPrice = (int)(inv->GetItem(i)->GetPrice() * 0.6f);

			std::string Msg = pad + "  " + std::to_string(i + 1) + ". "
				+ inv->GetItem(i)->GetName()
				+ "  판매가: " + std::to_string(sellPrice) + "G\n";

			std::cout << pad << "  " << (i + 1) << ". "
				<< inv->GetItem(i)->GetName()
				<< "  판매가: " << sellPrice << "G\n";
		}
	}
	std::cout << pad << "================================\n";
	std::cout << "\n" << pad << "판매할 아이템 번호 입력 (0: 취소) : ";
	int index;
	std::cin >> index;
	if (index == 0) return;
	SellItem(pPlayer, index - 1);
}

// 잘못된 구매할려고할때
void CShop::BuyItem(CPlayer* pPlayer, int index)
{
	std::string pad = GetPad(32);
	if (index < 0 || index >= (int)m_ShopItems.size())
	{
		std::cout << pad << "잘못된 번호입니다.\n";
		std::cin.get();
		return;
	}

	int price = m_ShopItems[index]->GetPrice();

	if (!pPlayer->SpendGold(price))
	{
		std::cout << pad << "골드가 부족합니다! (필요: " << price
			<< "G, 보유: " << pPlayer->GetGold() << "G)\n";
		std::cin.get();
		return;
	}

	// 구매할 때마다 새 포션 복사본 생성
	Potion* original = dynamic_cast<Potion*>(m_ShopItems[index].get());
	if (original)
	{
		auto newPotion = std::make_shared<Potion>(*original); // 새 포션 생성
		pPlayer->GetInventory()->AddItem(newPotion, 1);// 플레이어 가방에 넣음
	}


	std::cout << pad << "[" << m_ShopItems[index]->GetName() << "] 을(를) 구매했습니다!\n";
	std::cin.get();
}

void CShop::SellItem(CPlayer* pPlayer, int index)
{
	std::string pad = GetPad(32);
	auto inv = pPlayer->GetInventory();
	
	if (!inv->GetItem(index))
	{
		return;
	}
	
	if (index < 0 || index >= (int)inv->GetSize())
	{
		std::cout << pad << "잘못된 번호입니다.\n";
		std::cin.get();
		return;
	}

	int sellPrice = (int)(inv->GetItem(index)->GetPrice() * 0.6f); // 판매가는 원래 가격의 60%
	std::string name = inv->GetItem(index)->GetName();

	inv->RemoveItem(index, 1);// 플레이어 가방에서 해당 아이템 삭제
	pPlayer->AddGold(sellPrice);// 깎인 만큼 돈을 추가

	std::cout << pad << "[" << name << "] 을(를) " << sellPrice << "G에 판매했습니다!\n";
	std::cin.get();
}
