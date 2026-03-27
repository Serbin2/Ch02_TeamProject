// [작성자] : 박재현 
// [DESC] : 리소스 베이스

#pragma once
#include "../Utils/Common.h"

class CResourceBase
{
public:
	CResourceBase();
	virtual ~CResourceBase();

	virtual void LoadFile(const std::wstring& path);
	virtual void SaveFile(const std::wstring& path);
};

