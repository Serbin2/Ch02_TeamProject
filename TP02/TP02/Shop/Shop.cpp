#include "Shop.h"
#include "../Character/Player.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "../Inventory/Inventory.h"
#include "../Graphics/Interface.h"
#include "../Graphics/ConsoleGraphic.h"


//  콘솔 색상 
//  Windows API SetConsoleTextAttribute 를 사용


static void SetColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 모든 출력이 끝난 뒤 반드시 호출해서 다음 출력이 깨지지 않게 복원

static void ResetColor()
{
	SetColor(TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
}


//  레이아웃 
//  창 크기가 바뀌어도 자동으로 맞춰짐


// contentWidth : 출력할 내용의 실제 글자 너비(칸 수)
// 반환값       : 왼쪽에 붙여야 할 공백 문자열

static std::string GetPad(int contentWidth)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	int left = (consoleWidth - contentWidth) / 2;
	if (left < 0) left = 0;   

	return std::string(left, ' ');
}

// lineCount : 출력할 전체 줄 수
// 콘솔 세로 중앙에 맞추기 위해 위쪽에 빈 줄을 미리 출력
static void PrintTopPadding(int lineCount)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int top = (consoleHeight - lineCount) / 2;
	if (top < 0) top = 0;

	for (int i = 0; i < top; i++)
		std::cout << "\n";
}


//  한글 한 글자는 영문 2칸을 차지
//  UTF-8 인코딩에서 한글은 3바이트로 표현되므로 바이트 수가 아닌 "화면에 보이는 칸 수"를 별도로 계산하여 여백 계산

static int VisWidth(const std::string& s)
{
	int width = 0;
	int i = 0;
	while (i < (int)s.size())
	{
		unsigned char c = (unsigned char)s[i];
		if ((c & 0xF0) == 0xE0)
		{
			// 한글 (UTF-8 3바이트) → 콘솔 2칸
			width += 2;
			i += 3;
		}
		else
		{
			// ASCII, 숫자, 특수문자 → 콘솔 1칸
			width += 1;
			i += 1;
		}
	}
	return width;
}


//  박스 드로잉
//
//  상점 UI는 아래 구조의 고정 너비(BOX_W=54) 텍스트 박스
//
//  +----------------------------------------------------+   ← HLine
//  |  아이템 이름               구매 50G  판매 30G    |   ← BoxRowPrice
//  |    :체력을 30 hp 회복한다.                        |   ← BoxRowDesc
//  +----------------------------------------------------+   ← HLine

static constexpr int BOX_W = 54; // '+' 포함 전체 너비

// 박스 위/아래 가로선 한 줄 출력
static void HLine(const std::string& pad)
{
	ResetColor();
	std::cout << pad << "+";
	for (int i = 0; i < BOX_W - 2; i++) std::cout << "-";
	std::cout << "+\n";
}


// 박스 한 줄 출력 — 왼쪽/오른쪽 텍스트를 각각 다른 색으로 출력
//
// | leftText  ···(공백)···  rightText |
//
// 두 텍스트의 VisWidth 합산 후 나머지를 공백으로 채워
// 오른쪽 텍스트가 항상 '|' 바로 앞에 붙도록 맞춤

static void BoxRow(const std::string& pad,
	const std::string& leftText, WORD leftColor,
	const std::string& rightText, WORD rightColor)
{
	int spaces = (BOX_W - 2) - VisWidth(leftText) - VisWidth(rightText);
	if (spaces < 0) spaces = 0;

	ResetColor();
	std::cout << pad << "|";
	SetColor(leftColor);  std::cout << leftText;
	std::cout << std::string(spaces, ' ');
	SetColor(rightColor); std::cout << rightText;
	ResetColor();
	std::cout << "|\n";
}


// 아이템 한 줄 출력 — 이름(왼쪽) + 구매가/판매가(오른쪽)
//
// 출력 예:
//   |  1.  체력 포션 (소)      구매 50G  판매 30G  |
//
// 색상 규칙:
//   "구매" → 빨간색  
//   "판매" → 초록색  
//   숫자   → 노란색  (골드색 강조)
//
// priceWidth 계산 상세:
//   "구매 " = 한글2+한글2+공백1 = 5칸
//   buyStr  = 숫자 자릿수
//   "G"     = 1칸
//   "  "    = 두 가격 사이 구분 공백 2칸

static void BoxRowPrice(const std::string& pad,
	const std::string& nameText,
	int buyPrice,
	int sellPrice)
{
	std::string buyStr = std::to_string(buyPrice);
	std::string sellStr = std::to_string(sellPrice);

	int priceWidth = 5 + (int)buyStr.size() + 1   // "구매 50G"
		+ 2                             // "  " 구분 공백
		+ 5 + (int)sellStr.size() + 1  // "판매 30G"
		+ 2;                            // '|' 앞 여백

	int spaces = (BOX_W - 2) - VisWidth(nameText) - priceWidth;
	if (spaces < 0) spaces = 0;

	ResetColor();
	std::cout << pad << "|";

	SetColor(TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
	std::cout << nameText;
	std::cout << std::string(spaces, ' ');

	SetColor(TEXT_FOREGROUND_RED | TEXT_BACKGROUND_BLACK); std::cout << "구매 ";
	SetColor(TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_BLACK); std::cout << buyStr << "G";

	ResetColor(); std::cout << "  ";

	SetColor(TEXT_FOREGROUND_GREEN | TEXT_BACKGROUND_BLACK); std::cout << "판매 ";
	SetColor(TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_BLACK); std::cout << sellStr << "G";

	ResetColor();
	std::cout << "  |\n";
}


// 아이템 설명 줄 출력
//
// indent(들여쓰기 칸 수)를 이름 줄의 텍스트 시작 위치와 맞춰서
// 설명이 번호/이름 아래 정렬되어 보이도록함
//
// 예:
//   |  1.  체력 포션 (소)   ...   |
//   |      :체력을 30 hp 회복한다.|  ← indent=6 으로 "  1.  " 너비에 맞춤

static void BoxRowDesc(const std::string& pad,
	const std::string& desc,
	int indent)
{
	std::string indentStr(indent, ' ');

	int usedWidth = indent + VisWidth(desc);
	int spaces = (BOX_W - 2) - usedWidth;
	if (spaces < 0) spaces = 0;

	ResetColor();
	std::cout << pad << "|";
	SetColor(TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
	std::cout << indentStr << desc;
	std::cout << std::string(spaces, ' ');
	ResetColor();
	std::cout << "|\n";
}


//  PrintDialogue — 상점 주인 대사 출력
//
//  화면을 지운 뒤 타이핑 효과(글자당 80ms)로 대사를 출력하고, 2초 후 자동으로 반환
//  플레이어가 읽는 동안 다른 입력을 받지 않으므로 Sleep(1500) 뒤 다음 루프로 진행

static void PrintDialogue(const std::string& text)
{
	system("cls");

	std::string pad = GetPad(46); // 대사 박스 너비 46에 맞춰 계산
	PrintTopPadding(4);           // 세로 중앙 정렬용 상단 여백

	ResetColor();
	std::cout << pad << " +--[ ";
	SetColor(TEXT_FOREGROUND_CYAN | TEXT_BACKGROUND_BLACK);
	std::cout << "상점 주인";
	ResetColor();
	std::cout << " ]------------------------+\n";
	std::cout << pad << " | ";

	// 타이핑 효과: 한글은 3바이트를 한 단위로 출력해야 글씨 안깨짐

	SetColor(TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
	int i = 0;
	while (i < (int)text.size())
	{
		unsigned char c = (unsigned char)text[i];
		if ((c & 0xF0) == 0xE0) // 한글 시작 바이트 판별
		{
			std::cout << text[i] << text[i + 1] << text[i + 2] << std::flush;
			i += 3;
		}
		else
		{
			std::cout << text[i] << std::flush;
			i += 1;
		}
		Sleep(80);
	}

	std::cout << "\n";
	ResetColor();
	std::cout << pad << " +------------------------------------------+\n";

	Sleep(1500); // 플레이어가 대사를 읽을 시간
	ResetColor();
}


//  CShop 생성자
CShop::CShop()
{
	InitShopItems();
}


// 상점 판매 목록 초기화
//
// 구매 시 원본을 직접 주지 않고 복사본을 만들어 인벤토리에 넣음
// 원본은 여기서 딱 한 번만 생성하면됨
//

void CShop::InitShopItems()
{
	// Potion(이름, 설명, 구매가, 종류, 회복량)
	m_ShopItems.push_back(std::make_shared<Potion>("체력 포션 (소)", ":체력을 30 hp 회복한다.", 50, 1, 30));
	m_ShopItems.push_back(std::make_shared<CElixir>());
	m_ShopItems.push_back(std::make_shared<CInvinciblePotion>());
}


//  Enter — 상점 메인 루프
//
//  플레이어가 '0'(나가기)을 누를 때까지 아래 흐름을 반복
//
//    화면 초기화 → 상점 UI 출력 → 키 입력 대기
//      '1' → BuyMenu (구매)
//      '2' → SellMenu (판매)
//      '0' → 루프 탈출
//
//  퇴장 시 대사 출력 후 인벤토리 UI와 메인 화면을 복원

void CShop::Enter(CPlayer* pPlayer)
{
	PrintDialogue("어서오세요! 뭘 찾으세요?");

	while (true)
	{
		system("cls");
		ShowShopUI(pPlayer);

		std::string pad = GetPad(BOX_W);
		ResetColor();

		std::cout << "\n" << pad << "  [1] 구매  [2] 판매  [0] 나가기\n";
		std::cout << "\n";
		std::cout << pad << "  선택 : ";

		char key = _getch(); // 엔터 없이 즉시 입력 받기 (conio.h)
		if (key != '1' && key != '2' && key != '0') continue; // 유효하지 않은 키 무시

		std::cout << key << "\n";
		Sleep(1500); // 키 에코 후 잠깐 대기 (UI 전환 전 기다림)

		if (key == '1') BuyMenu(pPlayer);
		else if (key == '2') SellMenu(pPlayer);
		else if (key == '0') break;
	}

	PrintDialogue("또 오세요~ 기다리고 있을게요!");

	// 상점 종료 후 원래 게임 화면 복원
	ResetColor();
	system("cls");
	pPlayer->GetInventory()->UpdateUI();
	CInterface::GetInstance()->Redraw();
}


// ShowShopUI — 상점 전체 정보를 박스 형태로 출력
//
// 레이아웃:
//   +----+
//   | SHOP          |
//   +----+
//   | 보유 골드  N G|
//   +----+
//   | 1. 아이템A  구매 XG  판매 YG|
//   |    :설명                     |
//   | 2. ...                       |
//   +----+
//
// 판매가는 구매가의 60%로 고정 -> 조건

void CShop::ShowShopUI(CPlayer* pPlayer)
{
	std::string pad = GetPad(BOX_W);

	// 헤더(3줄) + 아이템 줄(이름+설명 = 2줄) + 하단선(1줄)
	int lineCount = 5 + (int)m_ShopItems.size() * 2;
	PrintTopPadding(lineCount);

	HLine(pad);
	BoxRow(pad, "  S H O P", TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK,
		"", TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
	HLine(pad);

	// 보유 골드는 노란색으로 강조해서 현재 구매력을 한눈에 확인
	BoxRow(pad,
		"  보유 골드", TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK,
		std::to_string(pPlayer->GetGold()) + " G  ", TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_BLACK);
	HLine(pad);

	for (int i = 0; i < (int)m_ShopItems.size(); i++)
	{
		int buyP = m_ShopItems[i]->GetPrice();
		int sellP = (int)(buyP * 0.6f); // 판매가 = 구매가의 60%

		// "  1.  " 앞 여백+번호+점+공백 = 6칸 → descIndent 와 통일
		std::string nameStr = "  " + std::to_string(i + 1) + ".  " + m_ShopItems[i]->GetName();
		int descIndent = 6;

		BoxRowPrice(pad, nameStr, buyP, sellP);
		BoxRowDesc(pad, m_ShopItems[i]->GetDesc(), descIndent);
	}

	HLine(pad);
}


//  BuyMenu — 구매할 아이템 번호 입력 받기
//
//  상점 UI를 다시 그린 뒤 번호 한 글자를 입력받음

void CShop::BuyMenu(CPlayer* pPlayer)
{
	system("cls");
	ShowShopUI(pPlayer);

	std::string pad = GetPad(BOX_W);
	ResetColor();

	std::cout << "\n" << pad << "  구매할 아이템 번호 입력 (0: 취소)\n";
	std::cout << "\n";
	std::cout << pad << "  선택 : ";

	char key = _getch();
	std::cout << key << "\n";
	Sleep(1500);

	if (key == '0') return; // 취소

	int index = key - '1'; // '1'→0, '2'→1, '3'→2 ...

	// 범위 초과 또는 미구현 번호 처리
	if (index < 0 || index >= (int)m_ShopItems.size())
	{
		PrintDialogue("그런 물건은 없는데요..?");
		return;
	}

	BuyItem(pPlayer, index);
}


//  SellMenu — 인벤토리 목록을 박스로 출력하고 판매할 번호 입력 받기
//
//  인벤토리가 비어 있으면 안내 대사를 출력하고 즉시 반환
//  판매가는 아이템의 원래 가격(GetPrice)의 60%

void CShop::SellMenu(CPlayer* pPlayer)
{
	system("cls");

	std::string pad = GetPad(BOX_W);
	auto inv = pPlayer->GetInventory();

	// 헤더(3줄) + 아이템 줄 + 하단선(1줄) + 입력 안내(1줄)
	int lineCount = 5 + (int)inv->GetSize() + 1;
	PrintTopPadding(lineCount);

	HLine(pad);
	BoxRow(pad, "  판 매", TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK,
		"", TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
	HLine(pad);
	BoxRow(pad,
		"  보유 골드", TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK,
		std::to_string(pPlayer->GetGold()) + " G  ", TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_BLACK);
	HLine(pad);

	// 인벤토리가 비어 있으면 UI 출력 없이 대사로 안내 후 복귀
	if (inv->IsEmpty())
	{
		BoxRow(pad, "  보유 중인 아이템이 없습니다.",
			TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK,
			"", TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
		HLine(pad);
		Sleep(500);
		PrintDialogue("팔 물건이 없으신 것 같은데요?");
		return;
	}

	// 인벤토리 목록 출력
	// 판매(돈이 들어옴) → "판매" 텍스트를 초록색으로 표시
	for (int i = 0; i < (int)inv->GetSize(); i++)
	{
		if (!inv->GetItem(i)) continue; // 슬롯이 비어 있으면 건너뜀

		int sellP = (int)(inv->GetItem(i)->GetPrice() * 0.6f);
		std::string sellStr = std::to_string(sellP);
		std::string nameStr = "  " + std::to_string(i + 1) + ".  " + inv->GetItem(i)->GetName();

		// 오른쪽 가격 영역 너비: "판매 " + 숫자 + "G" + "  " 여백
		int priceWidth = 5 + (int)sellStr.size() + 1 + 2;

		int spaces = (BOX_W - 2) - VisWidth(nameStr) - priceWidth;
		if (spaces < 0) spaces = 0;

		ResetColor();
		std::cout << pad << "|";
		SetColor(TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK); std::cout << nameStr;
		std::cout << std::string(spaces, ' ');
		SetColor(TEXT_FOREGROUND_GREEN | TEXT_BACKGROUND_BLACK); std::cout << "판매 ";
		SetColor(TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_BLACK); std::cout << sellStr << "G";
		ResetColor();
		std::cout << "  |\n";
	}

	HLine(pad);
	ResetColor();

	std::cout << "\n" << pad << "  판매할 아이템 번호 입력 (0: 취소)\n";
	std::cout << "\n";
	std::cout << pad << "  선택 : ";

	char key = _getch();
	std::cout << key << "\n";
	Sleep(1500);

	if (key == '0') return; // 취소

	SellItem(pPlayer, key - '1'); // 1-based 입력 → 0-based 인덱스 변환
}


//  BuyItem — 실제 구매 처리
//
//  1. SpendGold 로 골드 차감을 시도
//     골드가 부족하면 SpendGold 내부에서 차감하지 않고 false 를 반환
//  2. 성공 시 m_ShopItems 의 원본을 복사해서 인벤토리에 추가
//     (원본을 그대로 넘기면 상점 아이템이 소모되므로 반드시 복사)
//  3. 현재는 Potion 계열만 복사 지원

void CShop::BuyItem(CPlayer* pPlayer, int index)
{
	int price = m_ShopItems[index]->GetPrice();

	if (!pPlayer->SpendGold(price))
	{
		PrintDialogue("골드가 부족하시네요! 필요한 금액은 "
			+ std::to_string(price) + "G 입니다.");
		return;
	}

	// Potion 계열이면 복사 생성자로 새 인스턴스를 만들어 인벤토리에 추가
	Potion* original = dynamic_cast<Potion*>(m_ShopItems[index].get());
	if (original)
	{
		auto newPotion = std::make_shared<Potion>(*original); // 복사
		pPlayer->GetInventory()->AddItem(newPotion, 1);
	}

	PrintDialogue("[" + m_ShopItems[index]->GetName() + "] 좋은 선택이에요! ");
}


//  SellItem — 실제 판매 처리
//
//  1. 아이템 이름과 판매가를 미리 저장
//     (RemoveItem 호출 뒤에는 포인터가 무효화될 수 있으므로 먼저 읽어야 함)
//  2. RemoveItem 으로 인벤토리에서 1개를 제거
//  3. AddGold 로 판매가만큼 골드를 지급

void CShop::SellItem(CPlayer* pPlayer, int index)
{
	auto inv = pPlayer->GetInventory();

	if (index < 0 || index >= (int)inv->GetSize() || !inv->GetItem(index))
	{
		PrintDialogue("그런 물건은 없는데요..?");
		return;
	}

	int         sellPrice = (int)(inv->GetItem(index)->GetPrice() * 0.6f);
	std::string name = inv->GetItem(index)->GetName(); // 제거 전에 이름 저장

	inv->RemoveItem(index, 1);   // 인벤토리에서 1개 제거
	pPlayer->AddGold(sellPrice); // 판매 대금 지급

	PrintDialogue("[" + name + "] " + std::to_string(sellPrice) + "G에 잘 샀습니다. 고마워요!");
}