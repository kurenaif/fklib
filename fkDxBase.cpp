/**
* @file fkDxBase.cpp
* @brief DirectXのベースとなる部分を構築する
* @author 紅狼
* @date 2014/04/23
*/

#include "fkDxBase.h"
namespace fk
{
	// シェーダのコンパイル
	HRESULT CompileShaderFromFile(WCHAR* fname, LPCSTR entry_point, LPCSTR shd_model, ID3DBlob** pp_blob_out)
	{
		HRESULT hr = S_OK;

		DWORD shd_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		shd_flags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* err_blob;
		hr = D3DCompileFromFile(fname, NULL, NULL, entry_point, shd_model,
			shd_flags, 0, pp_blob_out, &err_blob);
		if (FAILED(hr)){
			if (err_blob != NULL){
				OutputDebugStringA((char*)err_blob->GetBufferPointer());
			}
			if (err_blob) err_blob->Release();
			return hr;
		}
		if (err_blob){
			err_blob->Release();
		}
		return S_OK;
	}

	DxBase::DxBase(const tstring &windowName, const fk::Size<int> &size, const HICON &hIcon, const HCURSOR &hCursor) :Application(windowName, SW_SHOWNORMAL, size.width, size.height, CW_USEDEFAULT, CW_USEDEFAULT, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, hIcon, hCursor)
	{
		InitDevice();
	}

	HRESULT DxBase::InitDevice()
	{
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(GetHWND(), &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		//DXGI_SWAP_CHAIN_DESC:http://msdn.microsoft.com/ja-jp/library/bb173075(v=vs.85).aspx
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		//バックバッファの解像度の幅
		sd.BufferDesc.Width = width;
		//バックバッファの解像度の高さ
		sd.BufferDesc.Height = height;
		//バックバッファの色のフォーマット 符号なし32bit4方向フォーマット
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//バックバッファのリフレッシュレート
		//以下２つは有理数を表す構造体 60/1(Hz)
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		//サーフェスまたはリソースを出力レンダーターゲットとして使用する
		//サーフェス：画像を保存しておく領域　フロントバッファ、バックバッファ、オフスクリーンサーフェスがこれ
		//リソース：DirectXがアクセスできるメモリの領域
		//出力レンダーターゲット：出力するためのサーフェス（テクスチャ）
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//出力するウィンドウ
		sd.OutputWindow = GetHWND();
		//マルチサンプリングパラメータを記述する
		//マルチサンプリング：http://ja.wikipedia.org/wiki/%E3%82%A2%E3%83%B3%E3%83%81%E3%82%A8%E3%82%A4%E3%83%AA%E3%82%A2%E3%82%B9
		//ピクセル単位のマルチサンプリング数
		sd.SampleDesc.Count = 1;
		//イメージの品質レベル0~ID3D11Device::CheckMultisampleQualityLevels()-1まで
		sd.SampleDesc.Quality = 0;
		//出力がウィンドウモードかどうか
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			m_driverType = driverTypes[driverTypeIndex];
			//http://msdn.microsoft.com/ja-jp/library/ee416033(v=vs.85).aspx
			//3つの方法でデバイスの生成を行う
			hr = D3D11CreateDeviceAndSwapChain(nullptr, m_driverType, NULL, createDeviceFlags, featureLevels, 3,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		//バックバッファへのアドレスを取得
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		//取得したバックバッファを指定し、レンダーターゲットを指定、生成
		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		//スワップチェイン→レンダーターゲット→描画デバイス
		//レンダーターゲットを作るために必要なdepth stencil texture(マルチサンプリング対象)
		//テクスチャ（バックバッファ）を読み書きする方法とかを設定
		// Create depth stencil texture
		// D3D11_TEXTURE2D_DESC:2Dテクスチャについての記述
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		//マルチサンプリングされたテクスチャには1を使用する
		descDepth.MipLevels = 1;
		//テクスチャ配列内のテクスチャの数
		descDepth.ArraySize = 1;
		//深度チャンネルに 24 ビット、ステンシル チャンネルに 8 ビットを使用する 32 ビット Z バッファー フォーマット
		//深度：http://www.adobe.com/jp/joc/focusin/tips/ae_recipe02.html
		//ステンシル：http://rudora7.blog81.fc2.com/blog-entry-443.html
		//Zバッファ：http://ja.wikipedia.org/wiki/Z%E3%83%90%E3%83%83%E3%83%95%E3%82%A
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//ピクセル単位のマルチサンプリングパラメータ（SwapChainで説明済み
		descDepth.SampleDesc.Count = 1;
		//イメージの品質レベル（SwapChainで説明済み
		descDepth.SampleDesc.Quality = 0;
		//テクスチャの読み込み及び描きこみ方法を指定
		//GPU による読み取りおよび書き込みアクセスを必要とするリソースです。これは最も一般的な使用法の選択です。
		//D3D11_USAGE:http://msdn.microsoft.com/ja-jp/library/ee416352(v=vs.85).aspx
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		//パイプラインステージへのバインドのフラグの指定
		//D3D11_BIND:http://msdn.microsoft.com/ja-jp/library/ee416041(v=vs.85).aspx
		//パイプラインステージ:http://ja.wikipedia.org/wiki/Direct3D#.E3.83.91.E3.82.A4.E3.83.97.E3.83.A9.E3.82.A4.E3.83.B3
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//CPUアクセスフラグ
		//必要でないときは0
		//D3D11_CPU_ACCESS_FLAG:http://msdn.microsoft.com/ja-jp/library/ee416074(v=vs.85).aspx
		descDepth.CPUAccessFlags = 0;
		//他の一般性の低いリソース オプションを識別するフラグ
		//D3D11_RESOURCE_MISC_FLAG:http://msdn.microsoft.com/ja-jp/library/ee416267(v=vs.85).aspx
		descDepth.MiscFlags = 0;
		//第３引数；作成されるテクスチャへのポインタのアドレス
		//深度/ステンシル　リソースの作成
		hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
		if (FAILED(hr))
			return hr;

		// Create the depth stencil view
		//深度/ステンシル　ビューの作成(深度/ステンシルのリソースをバインドするのに必要
		//出力結合ステートで深度/ステンシル テストするかどうかの決定
		//深度/ステンシルテスト:（特定のピクセルを描画するかどうかを決定する）（ピクセルを破棄したりするのを考える）
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
		if (FAILED(hr))
			return hr;

		//1 つ以上のレンダー ターゲットをアトミックにバインドし、出力結合ステージに深度ステンシル バッファーをバインドします
		//アトミックに：完全に実行するor全く実行しない　ように見えるみたいな感じのやつ？
		//デバイスコンテキスト（描画する媒体のようなもの）にバックバッファとデプスバッファを関連付けさせる
		//レンダーターゲット→描画先＝バックバッファ？
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		// Setup the viewport
		//ビューポート：３次元空間を描画する２次元の空間　のsetup
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pImmediateContext->RSSetViewports(1, &vp);

		// Create DirectXTK objects
		m_States.reset(new CommonStates(m_pd3dDevice));
		m_Sprites.reset(new SpriteBatch(m_pImmediateContext));
		m_FXFactory.reset(new EffectFactory(m_pd3dDevice));
		m_Batch.reset(new PrimitiveBatch<VertexPositionColor>(m_pImmediateContext));

		//BasicEffect:テクスチャマッピング、頂点色、指向性証明、霧をサポートする
		m_BatchEffect.reset(new BasicEffect(m_pd3dDevice));
		//頂点色を有効にする
		m_BatchEffect->SetVertexColorEnabled(true);

		{
			void const* shaderByteCode;
			size_t byteCodeLength;

			m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

			//頂点レイアウトを作成する
			//第１引数：頂点情報（作成したもの）
			//第２引数：その要素数
			//第３引数：コンパイル済みのシェーダ―コードを渡す
			//第４引数：シェーダ―コードの長さ
			//第５引数：作成された頂点レイアウト
			hr = m_pd3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
				VertexPositionColor::InputElementCount,
				shaderByteCode, byteCodeLength,
				&m_pBatchInputLayout);
			if (FAILED(hr))
				return hr;
		}

		// Initialize the world matrices
		m_World = XMMatrixIdentity();

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_View = XMMatrixLookAtLH(Eye, At, Up);

		m_BatchEffect->SetView(m_View);

		//// Initialize the projection matrix
		////視野に基づいて、左手座標系パースペクティブ射影行列を生成
		////第1:トップダウン視野角度
		////第2:アス比
		////第3:近くの面の奥行き
		////第4:遠くの面の奥行き
		//m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

		//m_BatchEffect->SetProjection(m_Projection);

		// 頂点シェーダのコンパイル
		ID3DBlob* vs_blob = NULL;
		hr = CompileShaderFromFile(L"dx11Poly2D.fx", "vsMain", "vs_4_0", &vs_blob);
		if (FAILED(hr)){
			MessageBox(NULL, L"シェーダのコンパイル失敗", L"Error", MB_OK);
		}

		// 頂点シェーダ作成
		hr = m_pd3dDevice->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), NULL, &m_pVertexShader);
		if (FAILED(hr)){
			vs_blob->Release();
		}

		// 入力レイアウト定義
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elem_num = ARRAYSIZE(layout);

		// 入力レイアウト作成
		hr = m_pd3dDevice->CreateInputLayout(layout, elem_num, vs_blob->GetBufferPointer(),
			vs_blob->GetBufferSize(), &m_pVertexLayout);
		vs_blob->Release();
		if (FAILED(hr)){
		}
		// ピクセルシェーダのコンパイル
		ID3DBlob* ps_blob = NULL;
		hr = CompileShaderFromFile(L"dx11Poly2D.fx", "psMain", "ps_4_0", &ps_blob);
		if (FAILED(hr)){
			MessageBox(NULL,
				L"シェーダのコンパイル失敗", L"Error", MB_OK);
		}

		// ピクセルシェーダ作成
		hr = m_pd3dDevice->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), NULL, &m_pPixelShader);
		ps_blob->Release();
		if (FAILED(hr)){
		}

		return S_OK;
	}
}

fk::DxBase::~DxBase()
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pBatchInputLayout) m_pBatchInputLayout->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	if (m_pDepthStencil) m_pDepthStencil->Release();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);



	ID3D11PixelShader*      m_pPixelShader = nullptr;
	ID3D11InputLayout*      m_pVertexLayout = nullptr;
	ID3D11VertexShader*     m_pVertexShader = nullptr;

#ifdef DXTK_AUDIO
	m_audEngine.reset();
#endif
}

bool fk::DxBase::Run()
{
	return Application::Run();
}
