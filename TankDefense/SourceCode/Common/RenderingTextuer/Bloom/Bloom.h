#ifndef BLOOM_RENDER_H
#define BLOOM_RENDER_H

#include "..\RenderingTextuer.h"

class CDownSamplingRender;

/***********************************************
*	�u���[���`��N���X.
*	�쐣���O���A�G�t�F�N�g���g�p.
*	�Q�l�T�C�g(http://project-asura.com/program/d3d11/d3d11_010.html).
**/
class CBloomRender : public CRenderTexture
{
	struct C_BUFFER_PER_FRAME
	{
		static const int NUM_OFFSET			= 16;
		static const int HALF_NUM_OFFSET	= 8;

		D3DXVECTOR4 Offset[NUM_OFFSET];	// �I�t�Z�b�g�l.
	};
public:
	CBloomRender();
	virtual ~CBloomRender();

	// ������.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;

	// �`��.
	void Render( const int& srvCount, const std::vector<ID3D11ShaderResourceView*>& srvList );

	// �o�b�t�@�̐ݒ�.
	virtual void SetBuffer() override;
	// �V�F�[�_�[���\�[�X�r���[�̐����擾.
	virtual int GetSRVCount() override { return m_pShaderResourceViewList.size(); }

protected:
	// �e�N�X�`���̏�����.
	virtual HRESULT InitBufferTex() override;

private:
	// ���_�o�b�t�@�[�̍쐬.
	virtual HRESULT InitVertexBuffer() override;
	// �R���X�^���g�o�b�t�@�̍쐬.
	virtual HRESULT InitConstantBuffer() override;

	// �`��.
	void Render( 
		const int& index,
		ID3D11ShaderResourceView* pSrv );

	// �K�E�X�̏d�݂��v�Z.
	float GaussianDistriBution( const D3DXVECTOR2& pos, const float& rho );
	// �u���[�̃p�����[�^���v�Z.
	C_BUFFER_PER_FRAME CalcBlurParam( 
		const D3DXVECTOR2& size, 
		const D3DXVECTOR2& dir,
		const float& deviation,
		const float& multiply );

private:
	std::unique_ptr<CDownSamplingRender>	m_pDownSampling;		// �_�E���T���v��.
	std::vector<ID3D11Buffer*>				m_pConstantBuffersInit;	// �R���X�^���g�o�b�t�@.
	std::vector<ID3D11Buffer*>				m_pVertexBuffers;		// ���_�o�b�t�@.
	ID3D11Buffer*							m_pConstantBufferFrame;	// �R���X�^���g�o�b�t�@.
};

#endif	// #ifndef BLOOM_RENDER_H.