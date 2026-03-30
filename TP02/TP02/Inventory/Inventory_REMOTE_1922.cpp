#include "Inventory.h"
#include "../Item/Item.h"
#include "../Utils/Utils.h"

#include "../Graphics/ConsoleGraphic.h"
#include "../Graphics/Interface.h"
#include "../Item/Bomb.h"

CInventory::CInventory()
{
	m_vSlots.resize(m_ciMaxSlot);

	CInterface::GetInstance()->AddUI(10, "1.");
	CInterface::GetInstance()->AddUI(11, "2.");
	CInterface::GetInstance()->AddUI(12, "3.");
	CInterface::GetInstance()->AddUI(13, "4.");
	CInterface::GetInstance()->AddUI(14, "5.");

	//UpdateUI();

#ifdef _DEBUG
		AddItem(std::make_shared<Potion>(), 1);
		AddItem(std::make_shared<CElixir>(), 1);
		AddItem(std::make_shared<CBombItem>(), 1);
		AddItem(std::make_shared<CInvinciblePotion>(), 1);
#endif
}

CInventory::CInventory(int iMaxSlot)
{
	m_vSlots.resize(iMaxSlot);
}

CInventory::~CInventory()
{
	m_vSlots.clear();
}

bool CInventory::CanAddItem(std::shared_ptr<CItem> pItem, int iAmount)
{
	int Remaining = iAmount;

	for (const auto& Slot : m_vSlots)
	{
		// 아이템 겹처서 보관가능한지 판단
		if (!Slot.IsEmpty() && Slot.m_pItem->GetName() == pItem->GetName())
		{
			int CanStack = Slot.m_pItem->GetMaxAmount() - Slot.m_iOwningAmount;

			Remaining -= CanStack;

			if (Remaining <= 0)
				return true;
		}
	}

	// 빈 슬롯 확인
	for (const auto& Slot : m_vSlots)
	{
		if (Slot.IsEmpty())
		{
			Remaining -= pItem->GetMaxAmount();

			if (Remaining <= 0)
				return true;
		}
	}

	return false;
}

void CInventory::UseItem(int iItemIdx, std::weak_ptr<class CPlayer> pPlayer)
{
	// 플레이어가 유효한 값인지 확인
	if (!pPlayer.lock())
	{
		return;
	}


	int iSize = static_cast<int>(m_vSlots.size());

	// 잘못된 인덱스 접근
	if (iItemIdx < 0 ||  iSize <= iItemIdx)
	{
		return;
	}

	//CGraphic::GetInstance()->AddLog(to_string(iItemIdx) + " 번 슬롯 키 입력");

	FInventorySlot& Slot = m_vSlots.at(iItemIdx);
	if (Slot.m_iOwningAmount >= 1)
	{
		Slot.m_pItem->UseItem(pPlayer);
		RemoveItem(iItemIdx, 1);
	}
}

bool CInventory::AddItem(std::shared_ptr<CItem> pItem, int iAmount)
{
	if (!CanAddItem(pItem, iAmount))
	{
		std::cout << "아이템을 더 이상 추가할수 없음" << std::endl;
		return false;
	}

	int Remaining = iAmount;

	// 기존 슬롯 채우기
	for (auto& Slot : m_vSlots)
	{
		// 같은 이름의 아이템 찾기
		if (!Slot.IsEmpty() && Slot.m_pItem->GetName() == pItem->GetName())
		{
			// 현재 슬롯에 추가로 넣을수 있는 아이템 개수
			int CanAddAmount = Slot.m_pItem->GetMaxAmount() - Slot.m_iOwningAmount;

			// 더 작은값을 현재 슬롯에 넣음
			int AddAmount = min(Remaining, CanAddAmount);

			// 실제 슬롯 아이템 개수 증가
			Slot.m_iOwningAmount += AddAmount;

			// 남은 아이템 개수 감소
			Remaining -= AddAmount;

			// 남은 아이템 개수가 0보다 작거나 같으면, 존재하는 슬롯에 아이템을 다 넣은 상태
			if (Remaining <= 0)
			{
				UpdateUI();
				return true;
			}
		}
	}

	// 위에서 모든 슬롯
	for (auto& Slot : m_vSlots)
	{
		if (Slot.IsEmpty())
		{
			// 아이템 클래스 넣기 
			Slot.m_pItem = pItem;

			Slot.m_bEmpty = false;

			// 더 작은값을 현재 슬롯에 넣음
			int AddAmount = min(Remaining, pItem->GetMaxAmount());

			// 현재 슬롯 개수 설정 
			Slot.m_iOwningAmount = AddAmount;

			// 남은 아이템 감소
			Remaining -= AddAmount;

			// 0보다 작으면 모두 수납완료
			if (Remaining <= 0)
			{
				UpdateUI();
				return true;
			}
		}
	}

	// 여기까지 도달하면, 아이템이 남아있다는 뜻으로 잘못된 상황이다.
	// 실제로 여기까지 도달했다면, 로직의 수정이 있으니 작성에게 말을 해야한다.
	return false;
}

std::vector<int> CInventory::FindItem(const std::string& sItemName)
{
	std::vector<int> vFindIdx;

	for (int i = 0; i < m_vSlots.size(); i++)
	{
		const FInventorySlot& Slot = m_vSlots[i];

		if (Slot.m_pItem && Slot.m_pItem->GetName() == sItemName)
		{
			vFindIdx.push_back(i);
		}
	}

	return vFindIdx;
}

void CInventory::RemoveItem(int iItemIdx, int iRemoveAmount)
{
	FInventorySlot& Slot = m_vSlots.at(iItemIdx);

	Slot.m_iOwningAmount -= iRemoveAmount;
	if (Slot.m_iOwningAmount <= 0)
	{
		Slot.Clear();
	}

	UpdateUI();
}

void CInventory::UpdateUI()
{
	int iSize = (int)m_vSlots.size();

	for (int i = 0; i < iSize; ++i)
	{
		if (m_vSlots[i].IsEmpty())
		{
			CInterface::GetInstance()->SetValue(UI_IVNENTORY__START_LINE + i, "empty");
		}
		else
		{
			CInterface::GetInstance()->SetValue(UI_IVNENTORY__START_LINE + i, m_vSlots[i].PrintItemDataMinial());
		}
	}
}

bool CInventory::IsEmpty() const
{
	for (const auto& Slot : m_vSlots)
	{
		if (!Slot.IsEmpty())
		{
			return false;
		}
	}

	return true;
}

std::shared_ptr<CItem> CInventory::GetItem(int Index)
{
	if (Index < 0 || Index >= (int)GetSize())
	{
		return nullptr;
	}

	if (m_vSlots[Index].IsEmpty())
	{
		return nullptr;
	}

	return m_vSlots[Index].m_pItem;
}
