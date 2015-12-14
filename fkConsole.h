/**
* @file fkconsole.h
* @brief �R���\�[����\��������\�����Ȃ������肷��
* @author �g�T
* @date 2014/04/23
*/

#pragma once
#include <Windows.h>
#include <stdio.h>

namespace fk
{
	/**
	* �R���\�[����\�������肵�Ȃ������肷��
	*/
	class Console
	{
	public:
		/**
		* �R���X�g���N�^�ŃR���\�[���̕\�����s��
		*/
		Console(bool isShow = true);

		/**
		* �f�X�g���N�^�ŉ��
		*/
		~Console(){ ::FreeConsole(); }

		/**
		* �R���\�[���̕\��
		*/
		void Show(){ if (!m_isShow) ::AllocConsole(); }

		/**
		* �R���\�[���̔�\��
		*/
		void Hide(){ if (m_isShow) ::FreeConsole(); }
	private:
		//�R���X�g���N�^���\�����ꂽ���ǂ���
		bool m_isShow;
	};
}

