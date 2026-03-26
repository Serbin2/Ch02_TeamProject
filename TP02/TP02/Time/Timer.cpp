
#include "Timer.h"


CTimer::CTimer()
{
	m_bPaused = false;
}

CTimer::~CTimer()
{

}

void CTimer::Start()
{
	m_dPrev = high_resolution_clock::now();
}

void CTimer::Pause() 
{

}

double CTimer::Update()
{
	auto now = high_resolution_clock::now();
	auto elap = now - m_dPrev;
	double delta = duration<double>(elap).count();
	m_dPrev = now;
	return delta;
}