/**
* @file fkfps.h
* @brief fps�̒���
* @author �g�T
* @date 2014/04/23
*/

#include <Windows.h>
#include <mmsystem.h>

namespace fk
{
	/**
	* FPS�̒������s���B
	*/
	class FPSController
	{
	public:
		/**
		* �R���X�g���N�^
		* @param fps �ڕW��fps
		* @param isDropFrame �t���[���������邩�ǂ���
		*/
		FPSController(DWORD fps = 60, bool isDropFrame = false);

		/**
		* �ǂꂭ�炢�҂K�v�����邩�ǂ���
		* @retval true �\��������
		* @retval false �\�������Ȃ�
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