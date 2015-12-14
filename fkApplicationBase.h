/**
* @file fkapplicationBase.h
* @brief �A�v���P�[�V�����̊�b�ƂȂ镔�����\�z����
* @author �g�T
* @date 2014/04/23
*/

#pragma once
#include "fkwindowbase.h"
#include "fkstring.h"

namespace fk
{
	/**
	* �A�v���P�[�V�����̊�b�ƂȂ镔�����\�z����
	*/
	class ApplicationBase
	{
	public:
		/**
		* ���������AWindow�𐶐�����B ���S�Ƀf�t�H���g�̈����œn����100������̌��E��"���Ă܂�"(�������[�v���)
		* @param windowName Window�̃^�C�g��
		* @param nCmdShow �\����� main�̈�������n��
		* @param width Window�̉���
		* @param height Window�̏c��
		* @param position_x window��x���W
		* @param position_y window��y���W
		* @param style �\���X�^�C��
		* @param hIcon �A�C�R��
		* @param hCursor �J�[�\��
		* @param className ���̂܂�
		* @param limit ����Window�쐬����� �������[�v����̂��ߍ��܂��� ���₵���������炢���ς��ǂ���
		*/
		ApplicationBase(const tstring &windowName = TEXT("window"),
			const int &nCmdShow = SW_SHOWNORMAL,
			const UINT &width = CW_USEDEFAULT, const UINT &height = CW_USEDEFAULT,
			const int &position_x = CW_USEDEFAULT, const int &position_y = CW_USEDEFAULT,
			const DWORD &style = WS_OVERLAPPEDWINDOW,
			const HICON &hIcon = NULL,
			const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW),
			const tstring &className = TEXT("window"),
			const int &limit = 100);

		/**
		* �E�B���h�E�v���V�[�W�����ĂԁB���[�v������
		* @retval true ���ɂȂ�
		* @retval false �E�B���h�E������ꂽ
		*/
		virtual bool Run();

		HWND GetHWND(){ return window.GetHWND(); }

	private:
		WindowBase window;
	};
}