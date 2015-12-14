/**
* @file fkDxBase.cpp
* @brief DirectX�̃x�[�X�ƂȂ镔�����\�z����
* @author �g�T
* @date 2014/04/23
*/

#include "fkDxBase.h"
namespace fk
{
	// �V�F�[�_�̃R���p�C��
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
		//�o�b�N�o�b�t�@�̉𑜓x�̕�
		sd.BufferDesc.Width = width;
		//�o�b�N�o�b�t�@�̉𑜓x�̍���
		sd.BufferDesc.Height = height;
		//�o�b�N�o�b�t�@�̐F�̃t�H�[�}�b�g �����Ȃ�32bit4�����t�H�[�}�b�g
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//�o�b�N�o�b�t�@�̃��t���b�V�����[�g
		//�ȉ��Q�͗L������\���\���� 60/1(Hz)
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		//�T�[�t�F�X�܂��̓��\�[�X���o�̓����_�[�^�[�Q�b�g�Ƃ��Ďg�p����
		//�T�[�t�F�X�F�摜��ۑ����Ă����̈�@�t�����g�o�b�t�@�A�o�b�N�o�b�t�@�A�I�t�X�N���[���T�[�t�F�X������
		//���\�[�X�FDirectX���A�N�Z�X�ł��郁�����̗̈�
		//�o�̓����_�[�^�[�Q�b�g�F�o�͂��邽�߂̃T�[�t�F�X�i�e�N�X�`���j
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//�o�͂���E�B���h�E
		sd.OutputWindow = GetHWND();
		//�}���`�T���v�����O�p�����[�^���L�q����
		//�}���`�T���v�����O�Fhttp://ja.wikipedia.org/wiki/%E3%82%A2%E3%83%B3%E3%83%81%E3%82%A8%E3%82%A4%E3%83%AA%E3%82%A2%E3%82%B9
		//�s�N�Z���P�ʂ̃}���`�T���v�����O��
		sd.SampleDesc.Count = 1;
		//�C���[�W�̕i�����x��0~ID3D11Device::CheckMultisampleQualityLevels()-1�܂�
		sd.SampleDesc.Quality = 0;
		//�o�͂��E�B���h�E���[�h���ǂ���
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			m_driverType = driverTypes[driverTypeIndex];
			//http://msdn.microsoft.com/ja-jp/library/ee416033(v=vs.85).aspx
			//3�̕��@�Ńf�o�C�X�̐������s��
			hr = D3D11CreateDeviceAndSwapChain(nullptr, m_driverType, NULL, createDeviceFlags, featureLevels, 3,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		//�o�b�N�o�b�t�@�ւ̃A�h���X���擾
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		//�擾�����o�b�N�o�b�t�@���w�肵�A�����_�[�^�[�Q�b�g���w��A����
		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		//�X���b�v�`�F�C���������_�[�^�[�Q�b�g���`��f�o�C�X
		//�����_�[�^�[�Q�b�g����邽�߂ɕK�v��depth stencil texture(�}���`�T���v�����O�Ώ�)
		//�e�N�X�`���i�o�b�N�o�b�t�@�j��ǂݏ���������@�Ƃ���ݒ�
		// Create depth stencil texture
		// D3D11_TEXTURE2D_DESC:2D�e�N�X�`���ɂ��Ă̋L�q
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		//�}���`�T���v�����O���ꂽ�e�N�X�`���ɂ�1���g�p����
		descDepth.MipLevels = 1;
		//�e�N�X�`���z����̃e�N�X�`���̐�
		descDepth.ArraySize = 1;
		//�[�x�`�����l���� 24 �r�b�g�A�X�e���V�� �`�����l���� 8 �r�b�g���g�p���� 32 �r�b�g Z �o�b�t�@�[ �t�H�[�}�b�g
		//�[�x�Fhttp://www.adobe.com/jp/joc/focusin/tips/ae_recipe02.html
		//�X�e���V���Fhttp://rudora7.blog81.fc2.com/blog-entry-443.html
		//Z�o�b�t�@�Fhttp://ja.wikipedia.org/wiki/Z%E3%83%90%E3%83%83%E3%83%95%E3%82%A
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//�s�N�Z���P�ʂ̃}���`�T���v�����O�p�����[�^�iSwapChain�Ő����ς�
		descDepth.SampleDesc.Count = 1;
		//�C���[�W�̕i�����x���iSwapChain�Ő����ς�
		descDepth.SampleDesc.Quality = 0;
		//�e�N�X�`���̓ǂݍ��݋y�ѕ`�����ݕ��@���w��
		//GPU �ɂ��ǂݎ�肨��я������݃A�N�Z�X��K�v�Ƃ��郊�\�[�X�ł��B����͍ł���ʓI�Ȏg�p�@�̑I���ł��B
		//D3D11_USAGE:http://msdn.microsoft.com/ja-jp/library/ee416352(v=vs.85).aspx
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		//�p�C�v���C���X�e�[�W�ւ̃o�C���h�̃t���O�̎w��
		//D3D11_BIND:http://msdn.microsoft.com/ja-jp/library/ee416041(v=vs.85).aspx
		//�p�C�v���C���X�e�[�W:http://ja.wikipedia.org/wiki/Direct3D#.E3.83.91.E3.82.A4.E3.83.97.E3.83.A9.E3.82.A4.E3.83.B3
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//CPU�A�N�Z�X�t���O
		//�K�v�łȂ��Ƃ���0
		//D3D11_CPU_ACCESS_FLAG:http://msdn.microsoft.com/ja-jp/library/ee416074(v=vs.85).aspx
		descDepth.CPUAccessFlags = 0;
		//���̈�ʐ��̒Ⴂ���\�[�X �I�v�V���������ʂ���t���O
		//D3D11_RESOURCE_MISC_FLAG:http://msdn.microsoft.com/ja-jp/library/ee416267(v=vs.85).aspx
		descDepth.MiscFlags = 0;
		//��R�����G�쐬�����e�N�X�`���ւ̃|�C���^�̃A�h���X
		//�[�x/�X�e���V���@���\�[�X�̍쐬
		hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
		if (FAILED(hr))
			return hr;

		// Create the depth stencil view
		//�[�x/�X�e���V���@�r���[�̍쐬(�[�x/�X�e���V���̃��\�[�X���o�C���h����̂ɕK�v
		//�o�͌����X�e�[�g�Ő[�x/�X�e���V�� �e�X�g���邩�ǂ����̌���
		//�[�x/�X�e���V���e�X�g:�i����̃s�N�Z����`�悷�邩�ǂ��������肷��j�i�s�N�Z����j�������肷��̂��l����j
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
		if (FAILED(hr))
			return hr;

		//1 �ȏ�̃����_�[ �^�[�Q�b�g���A�g�~�b�N�Ƀo�C���h���A�o�͌����X�e�[�W�ɐ[�x�X�e���V�� �o�b�t�@�[���o�C���h���܂�
		//�A�g�~�b�N�ɁF���S�Ɏ��s����or�S�����s���Ȃ��@�悤�Ɍ�����݂����Ȋ����̂�H
		//�f�o�C�X�R���e�L�X�g�i�`�悷��}�̂̂悤�Ȃ��́j�Ƀo�b�N�o�b�t�@�ƃf�v�X�o�b�t�@���֘A�t��������
		//�����_�[�^�[�Q�b�g���`��恁�o�b�N�o�b�t�@�H
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		// Setup the viewport
		//�r���[�|�[�g�F�R������Ԃ�`�悷��Q�����̋�ԁ@��setup
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

		//BasicEffect:�e�N�X�`���}�b�s���O�A���_�F�A�w�����ؖ��A�����T�|�[�g����
		m_BatchEffect.reset(new BasicEffect(m_pd3dDevice));
		//���_�F��L���ɂ���
		m_BatchEffect->SetVertexColorEnabled(true);

		{
			void const* shaderByteCode;
			size_t byteCodeLength;

			m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

			//���_���C�A�E�g���쐬����
			//��P�����F���_���i�쐬�������́j
			//��Q�����F���̗v�f��
			//��R�����F�R���p�C���ς݂̃V�F�[�_�\�R�[�h��n��
			//��S�����F�V�F�[�_�\�R�[�h�̒���
			//��T�����F�쐬���ꂽ���_���C�A�E�g
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
		////����Ɋ�Â��āA������W�n�p�[�X�y�N�e�B�u�ˉe�s��𐶐�
		////��1:�g�b�v�_�E������p�x
		////��2:�A�X��
		////��3:�߂��̖ʂ̉��s��
		////��4:�����̖ʂ̉��s��
		//m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

		//m_BatchEffect->SetProjection(m_Projection);

		// ���_�V�F�[�_�̃R���p�C��
		ID3DBlob* vs_blob = NULL;
		hr = CompileShaderFromFile(L"dx11Poly2D.fx", "vsMain", "vs_4_0", &vs_blob);
		if (FAILED(hr)){
			MessageBox(NULL, L"�V�F�[�_�̃R���p�C�����s", L"Error", MB_OK);
		}

		// ���_�V�F�[�_�쐬
		hr = m_pd3dDevice->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), NULL, &m_pVertexShader);
		if (FAILED(hr)){
			vs_blob->Release();
		}

		// ���̓��C�A�E�g��`
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elem_num = ARRAYSIZE(layout);

		// ���̓��C�A�E�g�쐬
		hr = m_pd3dDevice->CreateInputLayout(layout, elem_num, vs_blob->GetBufferPointer(),
			vs_blob->GetBufferSize(), &m_pVertexLayout);
		vs_blob->Release();
		if (FAILED(hr)){
		}
		// �s�N�Z���V�F�[�_�̃R���p�C��
		ID3DBlob* ps_blob = NULL;
		hr = CompileShaderFromFile(L"dx11Poly2D.fx", "psMain", "ps_4_0", &ps_blob);
		if (FAILED(hr)){
			MessageBox(NULL,
				L"�V�F�[�_�̃R���p�C�����s", L"Error", MB_OK);
		}

		// �s�N�Z���V�F�[�_�쐬
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
