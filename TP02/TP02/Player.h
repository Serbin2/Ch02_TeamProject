#pragma once

#include <windows.h>

// 가이드라인: 클래스명 C 접두사
class CPlayer
{
public:
	// 생성자: 객체를 만들 때 이름을 인자로 받습니다.
	// 가이드라인: string 매개변수 s 접두사
	CPlayer(std::string sName)
	{
		m_sName = sName; // 입력받은 이름을 멤버 변수에 저장
	}

	virtual	~CPlayer() = default;

	// 이름을 확인하기 위한 함수 (테스트용)
	void ShowName()
	{
		std::cout << "저장된 이름: " << m_sName << std::endl;
	}

private:
	// 가이드라인: 멤버 변수 m_ 접두사 + string s 접두사
	std::string m_sName;
};