/**
* @file fkinput.h
* @brief キーボードによる入力を受け付ける
* @author 紅狼
* @date 2014/04/25
*/

#include <map>
#include <Windows.h>

namespace fk
{
	/**
	* 入力に関するオブジェクト
	*/
	class Input
	{
	public:
		/**
		* map key の設定を行う
		*/
		Input();

		/**
		* キーの状態の取得を行う
		*/
		BOOL Update(){ return GetKeyboardState(m_keyTable); }

		/**
		* キーの状態を取得する
		* @param key 知りたいキーのキー VK_ を渡す
		* @retbal true キーの取得に成功
		* @retbal false キーの取得に失敗
		*/
		bool GetKeyState(const int &key){ return ((m_keyTable[key] & 0x80)? true : false ); }

		/**
		* キーの状態を取得する
		*/
		bool operator [] (const int &key){ return((m_keyTable[key] & 0x80) ? true : false); }

	private:
		std::map<std::string, int> m_key;
		typedef std::map<std::string, int> keymap;
		BYTE m_keyTable[256];
	};
}