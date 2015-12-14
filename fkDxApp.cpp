/**
* @file DxApp.cpp
* @brief DirectXの色々を実装する
* @author 紅狼
* @date 2014/04/23
*/

#include "fkDxApp.h"

namespace fk
{
	DxApp::DxApp(const tstring &windowName, const fk::Size<int> &size, const HICON &hIcon, const HCURSOR &hCursor) :DxBase(windowName, size, hIcon, hCursor)
	{

	}

	bool DxApp::Run()
	{
		++m_frame;
		Clear();
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		// 入力レイアウト
		m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

		std::vector<VertexData> vertex;
		vertex.push_back(VertexData(XMFLOAT3(-0.5f, -0.5f, 0.2f)));//左下 0
		vertex.push_back(VertexData(XMFLOAT3(0.5f, -0.5f, 0.2f)));//右下 1
		vertex.push_back(VertexData(XMFLOAT3(-0.5f, 0.5f, 0.2f)));//左上 2
		vertex.push_back(VertexData(XMFLOAT3(0.5f, 0.5f, 0.2f)));//右上 3
		std::vector<WORD> index = { 0,2,1,1,2,3 };
		tstring name = TEXT("v");
		AddVertexPrimitive(name, vertex,index);
		DrawVertex(name);

		m_pSwapChain->Present(0, 0);
		return DxBase::Run();
	}

	DxApp::~DxApp()
	{
		for (auto it = m_pTextureMap.begin(); it != m_pTextureMap.end(); ++it)
		{
			if (it->second) it->second->Release();
		}
		for (auto it = m_pVertexBufferMap.begin(); it != m_pVertexBufferMap.end(); ++it)
		{
			if (it->second.pbuffer) it->second.pbuffer->Release();
			if (it->second.pIndex) it->second.pIndex->Release();
		}
	}

	void DxApp::Clear(const float &red, const float &green ,const float &blue, const float &alpha)
	{
		float color[4] = { red, green, blue, alpha };
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
	}

	void DxApp::Present(const UINT &syncInterval, const UINT &flags)
	{
		m_pSwapChain->Present(syncInterval, flags);
	}

	void DxApp::AddTexture(tstring filename, tstring textureName)
	{
		if (textureName == TEXT("")) textureName = filename;
		if (m_pTextureMap.find(textureName) != m_pTextureMap.end()) return;
		CreateWICTextureFromFile(m_pd3dDevice, filename.c_str(), NULL, &m_pTextureMap[filename]);
	}

	void DxApp::SpriteBegin(SpriteSortMode sortMode)
	{
		if (!m_isSprite) m_Sprites->Begin(sortMode);
		m_isSprite = true;
	}

	void DxApp::SPriteDrawTexture(const tstring &name, const XMFLOAT2 &position, const RECT *rect, FXMVECTOR color,
		float rotation , const XMFLOAT2 &origin, float scale, SpriteEffects effects,
		float layerDepth)
	{
		m_Sprites->Draw(m_pTextureMap[name], position, rect, color, rotation, origin,scale , effects, layerDepth);
	}

	void DxApp::SpriteEnd()
	{
		if (m_isSprite) m_Sprites->End();
		m_isSprite = false;
	}

	void DxApp::AddVertexPrimitive(tstring bufferName, std::vector<VertexData> vertex, std::vector<WORD> index)
	{
		if (m_pVertexBufferMap.find(bufferName) != m_pVertexBufferMap.end()) return;
		//頂点バッファ
		m_pVertexBufferMap[bufferName].bufferCount = vertex.size();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VertexData) * vertex.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertex.data();
		m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferMap[bufferName].pbuffer);

		//インデックスバッファ
		m_pVertexBufferMap[bufferName].indexCount = index.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD)* index.size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = index.data();
		m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferMap[bufferName].pIndex);

	}

	void DxApp::DrawVertex(tstring bufferName)
	{
		m_pImmediateContext->IASetIndexBuffer(m_pVertexBufferMap[bufferName].pIndex, DXGI_FORMAT_R16_UINT, 0);
		// 頂点バッファ
		UINT stride = sizeof(VertexData);
		UINT offset = 0;
		m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBufferMap[bufferName].pbuffer, &stride, &offset);
		// シェーダ
		m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
		m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);

		// プリミティブ形状
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pImmediateContext->DrawIndexed(m_pVertexBufferMap[bufferName].indexCount, 0, 0);
	}
}