/**
* @file fkconsole.h
* @brief コンソールを表示したり表示しなかったりする
* @author 紅狼
* @date 2014/04/23
*/

#pragma once
#include <Windows.h>
#include <stdio.h>

namespace fk
{
	/**
	* コンソールを表示したりしなかったりする
	*/
	class Console
	{
	public:
		/**
		* コンストラクタでコンソールの表示を行う
		*/
		Console(bool isShow = true);

		/**
		* デストラクタで解放
		*/
		~Console(){ ::FreeConsole(); }

		/**
		* コンソールの表示
		*/
		void Show(){ if (!m_isShow) ::AllocConsole(); }

		/**
		* コンソールの非表示
		*/
		void Hide(){ if (m_isShow) ::FreeConsole(); }
	private:
		//コンストラクタが表示されたかどうか
		bool m_isShow;
	};
}

