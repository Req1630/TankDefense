#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "..\Common.h"

/************************************************
*	�����_�����O�e�N�X�`���̃x�[�X�N���X.
**/
class CRenderTexture : public CCommon
{
protected:
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2 Tex;	// �e�N�X�`�����W.
	};
	// �R���X�^���g�o�b�t�@.
	struct C_BUFFER_PER_INIT
	{
		ALIGN16 D3DXMATRIX	mW;				// ���[���h�s��.
		ALIGN16 D3DXVECTOR2	vViewPort;		// �E�B���h�E�T�C�Y.
		ALIGN16 D3DXVECTOR2	vPixelSize;		// �s�N�Z���̃T�C�Y.
		ALIGN16 D3DXVECTOR2	vWndRatio;		// �E�B���h�E�̉����̔䗦.
	};

	const float CLEAR_BACK_COLOR[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
public:
	CRenderTexture();
	virtual ~CRenderTexture();

	// ������.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) = 0;

	// ���.
	virtual void Release();

	// �o�b�t�@�̐ݒ�.
	virtual void SetBuffer() = 0;

	// �V�F�[�_�[���\�[�X�r���[�̎擾.
	inline std::vector<ID3D11ShaderResourceView*> GetShaderResourceViewList() const { return m_pShaderResourceViewList; }
	// �V�F�[�_�[���\�[�X�r���[�̐����擾.
	virtual int GetSRVCount() = 0;

	// �T�C�Y�̕ύX.
	HRESULT Resize();

protected:
	// ������.
	HRESULT InitBase( ID3D11DeviceContext* pContext11 );
	// �o�b�t�@�̐ݒ�.
	void SetBuffer( const int& numViews );

	// �e�N�X�`���̏�����.
	virtual HRESULT InitBufferTex() = 0;

	// ���_�o�b�t�@�[�̍쐬.
	virtual HRESULT InitVertexBuffer();
	// �R���X�^���g�o�b�t�@�̍쐬.
	virtual HRESULT InitConstantBuffer();
	// ���_�V�F�[�_�[�쐬.
	HRESULT InitVertexShader( const char* filePath, const char* entryName );
	// �s�N�Z���V�F�[�_�[�쐬.
	HRESULT InitPixelShader( const char* filePath, const char* entryName );

	// �T���v���[�̍쐬.
	HRESULT InitSampleLinear();

	// �o�b�t�@�̍쐬.
	HRESULT CreateBufferTex(
		const D3D11_TEXTURE2D_DESC	texDesc,
		ID3D11RenderTargetView**	ppRTV,
		ID3D11ShaderResourceView**	ppSRV,
		ID3D11Texture2D**			ppTex );

protected:
	std::vector<ID3D11RenderTargetView*>	m_pRenderTargetViewList;	// �����_�[�^�[�Q�b�g�r���[.
	std::vector<ID3D11ShaderResourceView*>	m_pShaderResourceViewList;	// �V�F�[�_�[���\�[�X�r���[.
	std::vector<ID3D11Texture2D*>			m_pTexture2DList;			// �e�N�X�`��2D.

	ID3D11VertexShader*		m_pVertexShader;		// ���_�V�F�[�_�[.
	ID3D11PixelShader*		m_pPixelShader;			// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*		m_pVertexLayout;		// ���_���C�A�E�g.
	ID3D11SamplerState*		m_pSampleLinear;		// �T���v��.

	ID3D11Buffer*			m_pConstantBufferInit;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*			m_pVertexBuffer;		// ���_�o�b�t�@.

	UINT					m_WndWidth;			// �E�B���h�E��.
	UINT					m_WndHeight;		// �E�B���h�E����.
};

#endif	// #ifndef RENDER_TEXTURE_H.