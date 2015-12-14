/**
* @file fkwindowsbase.h
* @brief Window�����N���X���`����B
* @author �g�T
* @date 2014/04/23
*/
#pragma once
#include <Windows.h>
#include "fkstring.h"

namespace fk
{
	/**
	* Window�𐶐�����N���X ��݂̂�Window�ɑΉ����Ă܂��B
	*/
	class WindowBase
	{
	public:
		/**
		* WindowBase�I�u�W�F�N�g�𐶐����邾��
		*/
		WindowBase();

		/**
		* ���������AWindow�𐶐�����B ���ʓI��Init���ĂԂ̂ƈꏏ
		* @param hInst �E�B���h�E�̃C���X�^���X�n���h�� main�̈�������n��
		* @param className className
		* @param nCmdShow �\����� main�̈�������n��
		* @param windowName Window�̃^�C�g��
		* @param width Window�̉���
		* @param height Window�̏c��
		* @param position_x window��x���W
		* @param position_y window��y���W
		* @param style �\���X�^�C��
		* @param hIcon �A�C�R��
		* @param hCursor �J�[�\��
		*/
		WindowBase(const HINSTANCE hInst, const tstring &className,
			const int &nCmdShow = SW_SHOWNORMAL,
			const tstring &windowName = TEXT("Window Name"),
			const UINT &width = CW_USEDEFAULT, const UINT &height = CW_USEDEFAULT,
			const int &position_x = CW_USEDEFAULT, const int &position_y = CW_USEDEFAULT,
			const DWORD &style = WS_OVERLAPPEDWINDOW,
			const HICON &hIcon = NULL,
			const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW));

		/**
		* ���������AWindow�𐶐�����B
		* @param hInst �E�B���h�E�̃C���X�^���X�n���h�� main�̈�������n��
		* @param className className
		* @param nCmdShow �\����� main�̈�������n��
		* @param windowName Window�̃^�C�g��
		* @param width Window�̉���
		* @param height Window�̏c��
		* @param position_x window��x���W
		* @param position_y window��y���W
		* @param style �\���X�^�C��
		* @param hIcon �A�C�R��
		* @param hCursor �J�[�\��
		* @throw char* �G���[����throw���Ă�����!
		*/
		void Init(const HINSTANCE hInst, const tstring &className,
			const int &nCmdShow = SW_SHOWNORMAL,
			const tstring &windowName = TEXT("Window Name"),
			const UINT &width = CW_USEDEFAULT, const UINT &height = CW_USEDEFAULT,
			const int &position_x = CW_USEDEFAULT, const int &position_y = CW_USEDEFAULT,
			const DWORD &style = WS_OVERLAPPEDWINDOW,
			const HICON &hIcon = NULL,
			const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW));

		/**
		* �J������
		*/
		virtual ~WindowBase();

		/**
		* �E�B���h�E�n���h�����擾����
		* @return �E�B���h�E�̃n���h��
		*/
		HWND GetHWND(){ return m_hWnd; }

		/**
		* WNDCLASSEX���擾����
		* @return �E�B���h�E��WNDCLASSEX�̒l
		*/
		WNDCLASSEX GetWndClassEx(){return m_wCEx;}

		/**
		* �ǉ���Run�ōs���������L�q���� �Ō�Ɍp������Run���Ă�
		*/
		bool Run();

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//�E�B���h�E�v���V�[�W��
		WNDCLASSEX m_wCEx;
		HWND m_hWnd;
	};

	int main(HINSTANCE);
}