
#include "Timer.h"


CTimer::CTimer()
{
	m_bPaused = false;
	m_iTargetFPS = 60;
	m_dFrameDuration = duration<double>(1.0 / m_iTargetFPS);

	m_dFpsTimer = 0.0f;
	m_iFpsCount = 0;
	m_iFps = 0;
}

CTimer::~CTimer()
{

}

void CTimer::Start()
{
	m_dPrev = steady_clock::now();
	m_dFpsTimer = 0.0f;
	m_iFpsCount = 0;
	m_iFps = 0;
}

void CTimer::Pause() 
{
	m_bPaused = true;
	m_dPausedTime = steady_clock::now();
}

void CTimer::Resume()
{
	if (!m_bPaused)	return;

	m_dPrev += steady_clock::now() - m_dPausedTime;
	m_bPaused = false;
}

double CTimer::Update()
{
	if (m_bPaused)	return 0.0;

	if (m_iTargetFPS > 0)
	{

	}

	auto now = steady_clock::now();
	auto elap = now - m_dPrev;
	double delta = duration<double>(elap).count();
	m_dPrev = now;
	return delta;
}

void CTimer::LimitFrame(duration<double> target)
{
	auto now = steady_clock::now();
	auto elap = steady_clock::now();
}