
#include "Timer.h"


CTimer::CTimer()
{
	m_bPaused = false;
	m_iTargetFPS = 120;
	m_dFrameDuration = duration<double>(1.0 / m_iTargetFPS);

	m_dFpsTimer = 0.0f;
	m_iFpsCount = 0;
	m_iFps = 0;
}

CTimer::~CTimer()
{

}

CTimer* CTimer::m_pInstance = nullptr;

CTimer* CTimer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CTimer();
	}

	return m_pInstance;
}

void CTimer::Release()
{
	if (m_pInstance == nullptr)	return;

	delete m_pInstance;
	m_pInstance = nullptr;
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

	//	프레임 제한
	if (m_iTargetFPS > 0)
	{
		LimitFrame(m_dFrameDuration);
	}

	//	델타 타임 계산
	auto now = steady_clock::now();
	double delta = duration<double>(now - m_dPrev).count();
	m_dPrev = now;

	//	FPS 계산
	m_dFpsTimer += delta;
	m_iFpsCount++;
	if (m_dFpsTimer >= 1.0)
	{
		m_iFps = m_iFpsCount;
		m_iFpsCount = 0;
		m_dFpsTimer = 0.0;
	}

	return delta;
}

void CTimer::SetTargetFps(int target)
{
	if (target < 0)	target = 0;
	m_iTargetFPS = target;

	if (target == 0)	return;	//	프레임 제한 없음
	m_dFrameDuration = duration<double>(1.0 / m_iTargetFPS);
}

void CTimer::LimitFrame(duration<double> target)
{
	auto now = steady_clock::now();
	while (1)
	{
		auto elap = steady_clock::now();
		auto t = elap - now;
		if (duration<double>(t) > target)
		{
			return;
		}
	}
}