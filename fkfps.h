/**
* @file fkfps.h
* @brief fpsの調整
* @author 紅狼
* @date 2014/04/23
*/

#include <Windows.h>
#include <mmsystem.h>

namespace fk
{
	/**
	* FPSの調整を行う。
	*/
	class FPSController
	{
	public:
		/**
		* コンストラクタ
		* @param fps 目標のfps
		* @param isDropFrame フレーム落ちするかどうか
		*/
		FPSController(DWORD fps = 60, bool isDropFrame = false);

		/**
		* どれくらい待つ必要があるかどうか
		* @retval true 表示させる
		* @retval false 表示させない
		*/
		bool Regure();

		int GetFPS(){ return m_fps; }
		int GetSkipRate(){ return m_skipRate; }
	private:
		bool m_isInit;
		int m_fps;
		int m_targetFps;
		double m_frameTime;
		bool m_isDropFrame;
		DWORD m_beforeTime = timeGetTime();
		int m_beforeFrame;
		int m_passFrameCount;
		int m_skipCount;
		int m_skipRate;
	};
}