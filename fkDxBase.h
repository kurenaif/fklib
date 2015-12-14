/**
* @file fDxBase.h
* @brief DirectXのベースとなる部分を構築する
* @author 紅狼
* @date 2014/04/23
*/

#pragma once
#include "fkapplication.h"
#include "fkstring.h"
#include "fkMath.h"
#include <DirectXTex\DirectXTex.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <DDSTextureLoader.h>
#include <CommonStates.h>
#include <Effects.h>
#include <GeometricPrimitive.h>
#include <Model.h>
#include <PrimitiveBatch.h>
#include <ScreenGrab.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <VertexTypes.h>
#include <d3dcompiler.h>

using namespace DirectX;

#pragma comment(lib, "d3dCompiler.lib")

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

namespace fk
{
	HRESULT CompileShaderFromFile(WCHAR* fname, LPCSTR entry_point, LPCSTR shd_model, ID3DBlob** pp_blob_out);
	/**
	* DirectXのベースを構築する
	*/
	class DxBase : public Application
	{
	public:
		/**
		* コンストラクタ、指定したサイズのウィンドウを固定で出す
		*/
		DxBase(const tstring &WindowName, const fk::Size<int> &size, const HICON &hIcon = NULL, const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW));

		/**
		* デバイスの解放
		*/
		~DxBase();

		/**
		* デバイスの初期化
		*/
		HRESULT InitDevice();

		/**
		* フレーム調整
		*/
		virtual bool Run();
	protected:
		D3D_DRIVER_TYPE			m_driverType;
		D3D_FEATURE_LEVEL		m_featureLevel;
		ID3D11Device*			m_pd3dDevice;
		ID3D11DeviceContext*	m_pImmediateContext;
		IDXGISwapChain*			m_pSwapChain;
		ID3D11RenderTargetView*	m_pRenderTargetView;
		ID3D11Texture2D*		m_pDepthStencil;
		ID3D11DepthStencilView*	m_pDepthStencilView;
		std::unique_ptr<DirectX::CommonStates>	m_States;
		std::unique_ptr<DirectX::BasicEffect>	m_BatchEffect;
		std::unique_ptr<DirectX::EffectFactory>		m_FXFactory;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor> >    m_Batch;
		std::unique_ptr<DirectX::SpriteBatch>	m_Sprites;
		std::unique_ptr<DirectX::SpriteFont>	m_Font;
		ID3D11InputLayout*                  m_pBatchInputLayout = nullptr;


		ID3D11PixelShader*      m_pPixelShader = nullptr;
		ID3D11InputLayout*      m_pVertexLayout = nullptr;
		ID3D11VertexShader*     m_pVertexShader = nullptr;

		DirectX::XMMATRIX    m_World;
		DirectX::XMMATRIX	m_View;
		DirectX::XMMATRIX	m_Projection;
	};
}
