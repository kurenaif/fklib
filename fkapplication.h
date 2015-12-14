/**
//* @file fkapplication.h
//* @brief �Q�[���̊�b�ƂȂ镔�����\�z����
//* @author �g�T
//* @date 2014/04/25
*/

#pragma once
#include "fkApplicationBase.h"
#include "fkinput.h"
#include <memory>

namespace fk
{
	/**
	* WinAPI�ŏo���邱�Ƃ������Ŏ�������
	*/
	class Application : public ApplicationBase
	{
	public:
		/**
		* ���������AWindow�𐶐�����B
		* @param hInst �E�B���h�E�̃C���X�^���X�n���h�� main�̈�������n��
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
		Application(const tstring &windowName = TEXT("window"),
			const int &nCmdShow = SW_SHOWNORMAL,
			const UINT &width = CW_USEDEFAULT, const UINT &height = CW_USEDEFAULT,
			const int &position_x = CW_USEDEFAULT, const int &position_y = CW_USEDEFAULT,
			const DWORD &style = WS_OVERLAPPEDWINDOW,
			const HICON &hIcon = NULL,
			const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW),
			const tstring &className = TEXT("window"));

		/**
		* �E�B���h�E�𓮂����̂ɕK�v�Ȃ̂ɉ�����,����������
		*/
		virtual bool Run();

		/**
		* �L�[�̏�Ԃ��擾���� if(GetKeyState) �ŃL�[�������ꂽ��
		*/
		bool GetKeyState(const int &key){ return m_key->GetKeyState(key); }

		/**
		* �L�[�������ꂽ���̂ݔ�������
		* @param key ����ׂ�L�[
		*/
		bool IsKeyPush(const int &key);

		/**
		* �L�[�������ꂽ���̂ݔ�������
		* @param key ���ׂ�L�[
		*/
		bool IsKeyPull(const int &key);

		/**
		* �L�[�������ꂽ���̂ݔ�������
		* @param key ����ׂ�L�[
		*/
		bool IsKeyOn(const int &key){ return m_key->GetKeyState(key); }
	private:
		std::unique_ptr<Input> m_key;
		std::unique_ptr<Input> m_keyBefore;
	};
}