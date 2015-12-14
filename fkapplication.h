/**
//* @file fkapplication.h
//* @brief ゲームの基礎となる部分を構築する
//* @author 紅狼
//* @date 2014/04/25
*/

#pragma once
#include "fkApplicationBase.h"
#include "fkinput.h"
#include <memory>

namespace fk
{
	/**
	* WinAPIで出来ることをここで実装する
	*/
	class Application : public ApplicationBase
	{
	public:
		/**
		* 初期化し、Windowを生成する。
		* @param hInst ウィンドウのインスタンスハンドル mainの引数から渡す
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
		Application(const tstring &windowName = TEXT("window"),
			const int &nCmdShow = SW_SHOWNORMAL,
			const UINT &width = CW_USEDEFAULT, const UINT &height = CW_USEDEFAULT,
			const int &position_x = CW_USEDEFAULT, const int &position_y = CW_USEDEFAULT,
			const DWORD &style = WS_OVERLAPPEDWINDOW,
			const HICON &hIcon = NULL,
			const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW),
			const tstring &className = TEXT("window"));

		/**
		* ウィンドウを動かすのに必要なのに加えて,処理をする
		*/
		virtual bool Run();

		/**
		* キーの状態を取得する if(GetKeyState) でキーが押された時
		*/
		bool GetKeyState(const int &key){ return m_key->GetKeyState(key); }

		/**
		* キーが押された時のみ反応する
		* @param key しらべるキー
		*/
		bool IsKeyPush(const int &key);

		/**
		* キーが離された時のみ反応する
		* @param key 調べるキー
		*/
		bool IsKeyPull(const int &key);

		/**
		* キーが押された時のみ反応する
		* @param key しらべるキー
		*/
		bool IsKeyOn(const int &key){ return m_key->GetKeyState(key); }
	private:
		std::unique_ptr<Input> m_key;
		std::unique_ptr<Input> m_keyBefore;
	};
}