/**
* @file fkfps.cpp
* @brief fkfps.h‚ÌŽÀ‘•
* @author g˜T
* @date 2014/04/23
*/

#include "fkfps.h"

namespace fk
{
	FPSController::FPSController(DWORD fps, bool isDropFrame) :m_fps(0),m_targetFps(fps),m_frameTime(1000.0/(double)fps), m_isDropFrame(isDropFrame),m_passFrameCount(0),m_isInit(true){}

	bool FPSController::Regure()
	{
		++m_passFrameCount;

		if (m_isInit)
		{
			m_beforeTime = timeGetTime();
			m_isInit = false;
			return true;
		}
		if (m_isDropFrame && timeGetTime() > (DWORD)((m_passFrameCount + 1) * m_frameTime + m_beforeTime))
		{
			++m_skipCount;
			return false;
		}
		else
		{
			DWORD time = (DWORD)((m_passFrameCount) * m_frameTime + m_beforeTime);
			while (timeGetTime() <= time)
				Sleep(1);
			DWORD nowTime = timeGetTime();
			if (nowTime - m_beforeTime >= 1000)
			{
				m_fps = m_passFrameCount;
				m_beforeTime = nowTime;
				m_passFrameCount = 0;
				m_skipRate = m_skipCount;
			}
		}
		return true;
	}
}