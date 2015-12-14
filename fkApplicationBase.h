/**
* @file fkapplicationBase.h
* @brief アプリケーションの基礎となる部分を構築する
* @author 紅狼
* @date 2014/04/23
*/

#pragma once
#include "fkwindowbase.h"
#include "fkstring.h"

namespace fk
{
	/**
	* アプリケーションの基礎となる部分を構築する
	*/
	class ApplicationBase
	{
	public:
		/**
		* 初期化し、Windowを生成する。 完全にデフォルトの引数で渡すと100個が制作の限界に"してます"(無限ループ回避)
		* @param windowName Windowのタイトル
		* @param nCmdShow 表示状態 mainの引数から渡す
		* @param width Windowの横幅
		* @param height Windowの縦幅
		* @param position_x windowのx座標
		* @param position_y windowのy座標
		* @param style 表示スタイル
		* @param hIcon アイコン
		* @param hCursor カーソル
		* @param className そのまま
		* @param limit 同名Window作成数上限 無限ループ回避のため作りました 増やしたかったらいっぱいどうぞ
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
		* ウィンドウプロシージャを呼ぶ。ループさせる
		* @retval true 特になし
		* @retval false ウィンドウが閉じられた
		*/
		virtual bool Run();

		HWND GetHWND(){ return window.GetHWND(); }

	private:
		WindowBase window;
	};
}