/**
* @file fkwindowsbase.h
* @brief Window生成クラスを定義する。
* @author 紅狼
* @date 2014/04/23
*/
#pragma once
#include <Windows.h>
#include "fkstring.h"

namespace fk
{
	/**
	* Windowを生成するクラス 一つのみのWindowに対応してます。
	*/
	class WindowBase
	{
	public:
		/**
		* WindowBaseオブジェクトを生成するだけ
		*/
		WindowBase();

		/**
		* 初期化し、Windowを生成する。 結果的にInitを呼ぶのと一緒
		* @param hInst ウィンドウのインスタンスハンドル mainの引数から渡す
		* @param className className
		* @param nCmdShow 表示状態 mainの引数から渡す
		* @param windowName Windowのタイトル
		* @param width Windowの横幅
		* @param height Windowの縦幅
		* @param position_x windowのx座標
		* @param position_y windowのy座標
		* @param style 表示スタイル
		* @param hIcon アイコン
		* @param hCursor カーソル
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
		* 初期化し、Windowを生成する。
		* @param hInst ウィンドウのインスタンスハンドル mainの引数から渡す
		* @param className className
		* @param nCmdShow 表示状態 mainの引数から渡す
		* @param windowName Windowのタイトル
		* @param width Windowの横幅
		* @param height Windowの縦幅
		* @param position_x windowのx座標
		* @param position_y windowのy座標
		* @param style 表示スタイル
		* @param hIcon アイコン
		* @param hCursor カーソル
		* @throw char* エラー文をthrowしてくれるよ!
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
		* 開放処理
		*/
		virtual ~WindowBase();

		/**
		* ウィンドウハンドルを取得する
		* @return ウィンドウのハンドル
		*/
		HWND GetHWND(){ return m_hWnd; }

		/**
		* WNDCLASSEXを取得する
		* @return ウィンドウのWNDCLASSEXの値
		*/
		WNDCLASSEX GetWndClassEx(){return m_wCEx;}

		/**
		* 追加でRunで行う処理を記述する 最後に継承元のRunを呼ぶ
		*/
		bool Run();

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//ウィンドウプロシージャ
		WNDCLASSEX m_wCEx;
		HWND m_hWnd;
	};

	int main(HINSTANCE);
}