#pragma once

#include <chrono>

using namespace std::chrono;

class CTimer
{
public:
	CTimer();
	~CTimer();

	void Start();
	void Pause();
	double Update();
	

private:
	
	steady_clock::time_point m_dPrev;

	bool m_bPaused;
};