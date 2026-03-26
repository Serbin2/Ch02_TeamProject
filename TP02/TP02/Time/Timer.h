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
	void Resume();
	double Update();
	

private:
	
	void LimitFrame(duration<double> target);

	steady_clock::time_point m_dPrev;
	steady_clock::time_point m_dPausedTime;

	duration<double> m_dFrameDuration;

	int m_iTargetFPS;

	double m_dFpsTimer;
	int m_iFpsCount;
	int m_iFps;

	bool m_bPaused;
};