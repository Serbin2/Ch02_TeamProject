#pragma once

class CInput
{
private:
	CInput();
	~CInput();

	static CInput* m_pInstance;
public:
	static CInput* GetInstance();
	static void Release();

	void Update();

	bool IsKeyDown(int Key);
	bool IsKeyUp(int key);
	bool IsKeyPressed(int key);

private:
	bool m_aPrev[256] = { false, };
	bool m_aCurr[256] = { false, };
};