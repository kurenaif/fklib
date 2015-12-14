/**
* @file DxApp.h
* @brief DirectXの色々を実装する
* @author 紅狼
* @date 2014/04/23
*/

#pragma once
#include "fkDxBase.h"
#include "WICTextureLoader.h"
#include "fkfps.h"
#include "vs.h"
#include "ps.h"
#include <map>
#include <string>
#include <Shlwapi.h>
#include <vector>

namespace fk
{
	struct VertexData
	{
		VertexData(XMFLOAT3 pos_) :pos(pos_){}
		VertexData(){}
		XMFLOAT3 pos;
	};


	class DxApp : public DxBase
	{
	public:
		/**
		* 固定ウィンドウの作成
		*/
		DxApp(const tstring &windowName, const fk::Size<int> &size, const HICON &hIcon = NULL, const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW));

		/**
		* いろいろな開放処理
		*/
		~DxApp();

		/**
		* ループ
		*/
		bool Run();

		/**
		* 指定色で画面をクリア
		*/
		void Clear(const float &red = 0.0f, const float &green = 0.0f, const float &blue = 0.0f, const float &alpha = 1.0f);

		/**
		* 結果をウィンドウに反映
		*/
		void Present(const UINT &SyncInterval = 0, const UINT &flags = 0);

		/**
		* Spriteの使う準備
		*/
		void SpriteBegin(SpriteSortMode sortMode = SpriteSortMode_Deferred);

		/**
		* Spriteでテクスチャを描画する
		* @param 登録したテクスチャの名前
		* @param 表示位置
		* @param 表示する四角
		* @param 色を合成してるみたい
		* @param 回転(rad)
		* @param 原点(位置, スケーリング, 回転)
		* @param レイヤー深度
		*/
		void SPriteDrawTexture(const tstring &name, const XMFLOAT2 &position, const RECT *rect, FXMVECTOR color = Colors::White,
			float rotation = 0.0f, const XMFLOAT2 &origin = XMFLOAT2(0,0), float scale = 1.0f, SpriteEffects effet = SpriteEffects_None,
			float layerDepth = 0.0f);

		/**
		* Spriteの終了
		*/
		void SpriteEnd();

		/**
		* テクスチャを登録する
		* @param filename 読み込むファイルの名前
		* @param textureName そのテクスチャの名前
		*/
		void AddTexture(tstring filename, tstring textureName = TEXT(""));

		/**
		* フレーム数の取得
		*/
		unsigned int GetFrame(){ return m_frame; }

		/**
		* FPSの取得
		*/
		int GetFPS(){ return m_fpsController.GetFPS(); }

		/** 
		* 頂点バッファの登録を行う
		* @param bufferName 頂点バッファの名前
		* @param vertex 頂点郡
		インデックスは必ず右回りじゃないとダメみたいです
		*/
		void AddVertexPrimitive(tstring bufferName, std::vector<VertexData> vertex, std::vector<WORD> index);

		/**
		* 頂点を描画する
		* @param bufferName 頂点バッファの名前
		*/
		void DrawVertex(tstring bufferName);


	private:
		std::map<tstring, ID3D11ShaderResourceView*> m_pTextureMap;
		struct VertexPair
		{
			int bufferCount;
			ID3D11Buffer* pbuffer;
			int indexCount;
			ID3D11Buffer* pIndex;
		};
		std::map<tstring, VertexPair > m_pVertexBufferMap; //頂点バッファ格納用

		//SpriteがBegin()されているかどうか
		bool m_isSprite = false;
		unsigned int m_frame = 0;
		FPSController m_fpsController;
	};
}
