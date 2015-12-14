/**
* @file DxApp.h
* @brief DirectX�̐F�X����������
* @author �g�T
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
		* �Œ�E�B���h�E�̍쐬
		*/
		DxApp(const tstring &windowName, const fk::Size<int> &size, const HICON &hIcon = NULL, const HCURSOR &hCursor = LoadCursor(NULL, IDC_ARROW));

		/**
		* ���낢��ȊJ������
		*/
		~DxApp();

		/**
		* ���[�v
		*/
		bool Run();

		/**
		* �w��F�ŉ�ʂ��N���A
		*/
		void Clear(const float &red = 0.0f, const float &green = 0.0f, const float &blue = 0.0f, const float &alpha = 1.0f);

		/**
		* ���ʂ��E�B���h�E�ɔ��f
		*/
		void Present(const UINT &SyncInterval = 0, const UINT &flags = 0);

		/**
		* Sprite�̎g������
		*/
		void SpriteBegin(SpriteSortMode sortMode = SpriteSortMode_Deferred);

		/**
		* Sprite�Ńe�N�X�`����`�悷��
		* @param �o�^�����e�N�X�`���̖��O
		* @param �\���ʒu
		* @param �\������l�p
		* @param �F���������Ă�݂���
		* @param ��](rad)
		* @param ���_(�ʒu, �X�P�[�����O, ��])
		* @param ���C���[�[�x
		*/
		void SPriteDrawTexture(const tstring &name, const XMFLOAT2 &position, const RECT *rect, FXMVECTOR color = Colors::White,
			float rotation = 0.0f, const XMFLOAT2 &origin = XMFLOAT2(0,0), float scale = 1.0f, SpriteEffects effet = SpriteEffects_None,
			float layerDepth = 0.0f);

		/**
		* Sprite�̏I��
		*/
		void SpriteEnd();

		/**
		* �e�N�X�`����o�^����
		* @param filename �ǂݍ��ރt�@�C���̖��O
		* @param textureName ���̃e�N�X�`���̖��O
		*/
		void AddTexture(tstring filename, tstring textureName = TEXT(""));

		/**
		* �t���[�����̎擾
		*/
		unsigned int GetFrame(){ return m_frame; }

		/**
		* FPS�̎擾
		*/
		int GetFPS(){ return m_fpsController.GetFPS(); }

		/** 
		* ���_�o�b�t�@�̓o�^���s��
		* @param bufferName ���_�o�b�t�@�̖��O
		* @param vertex ���_�S
		�C���f�b�N�X�͕K���E��肶��Ȃ��ƃ_���݂����ł�
		*/
		void AddVertexPrimitive(tstring bufferName, std::vector<VertexData> vertex, std::vector<WORD> index);

		/**
		* ���_��`�悷��
		* @param bufferName ���_�o�b�t�@�̖��O
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
		std::map<tstring, VertexPair > m_pVertexBufferMap; //���_�o�b�t�@�i�[�p

		//Sprite��Begin()����Ă��邩�ǂ���
		bool m_isSprite = false;
		unsigned int m_frame = 0;
		FPSController m_fpsController;
	};
}
