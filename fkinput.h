/**
* @file fkinput.h
* @brief �L�[�{�[�h�ɂ����͂��󂯕t����
* @author �g�T
* @date 2014/04/25
*/

#include <map>
#include <Windows.h>

namespace fk
{
	/**
	* ���͂Ɋւ���I�u�W�F�N�g
	*/
	class Input
	{
	public:
		/**
		* map key �̐ݒ���s��
		*/
		Input();

		/**
		* �L�[�̏�Ԃ̎擾���s��
		*/
		BOOL Update(){ return GetKeyboardState(m_keyTable); }

		/**
		* �L�[�̏�Ԃ��擾����
		* @param key �m�肽���L�[�̃L�[ VK_ ��n��
		* @retbal true �L�[�̎擾�ɐ���
		* @retbal false �L�[�̎擾�Ɏ��s
		*/
		bool GetKeyState(const int &key){ return ((m_keyTable[key] & 0x80)? true : false ); }

		/**
		* �L�[�̏�Ԃ��擾����
		*/
		bool operator [] (const int &key){ return((m_keyTable[key] & 0x80) ? true : false); }

	private:
		std::map<std::string, int> m_key;
		typedef std::map<std::string, int> keymap;
		BYTE m_keyTable[256];
	};
}