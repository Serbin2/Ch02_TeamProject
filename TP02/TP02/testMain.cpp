
#include "Utils/Common.h"

#include "Manager/ResourceManager/ResourceManager.h"
#include "Manager/SoundManager/SoundManager.h"
#include "Resource/Sound/Sound.h"

#include "Inventory/Inventory.h"
#include "Item/Item.h"

int main()
{
	//HWND hHwnd = GetConsoleWindow();
	//GET_SINGLE(CResourceManager)->Init(hHwnd);
	//GET_SINGLE(CSoundManager)->Init(hHwnd);
	//// 로드 
	//GET_SINGLE(CResourceManager)->LoadSound(L"BGM1", L"DJ-Okawari-Flower-Dance-2010.wav");
	//GET_SINGLE(CResourceManager)->LoadSound(L"BGM2", L"OST-Second-Run.wav");
	//{
	//	//std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM1");
	//	std::shared_ptr<CSound> pSound = GET_SINGLE(CResourceManager)->GetSound(L"BGM2");
	//	pSound->Play(true);
	//}
	std::unique_ptr<CInventory> Inventory = std::make_unique<CInventory>(10);

	for (int i = 0; i < 5; ++i)
	{
		if (Inventory->AddItem(std::make_shared<Potion>(), 1))
		{
			Inventory->PrintItem();
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		Inventory->RemoveItem(0, 1);
	}

	Inventory->PrintItem();

	return 0;
}