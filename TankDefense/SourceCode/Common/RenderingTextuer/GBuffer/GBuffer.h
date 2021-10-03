#ifndef G_BUFFER_RENDER_H
#define G_BUFFER_RENDER_H

#include "..\RenderingTextuer.h"

class CCascadedShadowMap;

/*********************************************
*	G-Buffer�̕`��N���X.
**/
class CGBufferRender : public CRenderTexture
{
public:
	// G-Buffer�̎��.
	enum enGBufferNo
	{
		EGBufferNo_BACK,		// �F���.
		EGBufferNo_COLOR,		// �F���.
		EGBufferNo_NORMAL,		// �@�����.
		EGBufferNo_WORLDPOS,	// ���[���h���W.
		EGBufferNo_Z_DEPTH,		// �[�x�l���.
		EGBufferNo_DIFFUSE,		// �F���.

		EGBufferNo_MAX,	// �ő�l.

	} typedef EGBufferNo;
public:
	CGBufferRender();
	virtual ~CGBufferRender();

	// ������.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;

	// �o�b�t�@�̐ݒ�.
	virtual void SetBuffer() override;
	// �V�F�[�_�[���\�[�X�r���[�̐����擾.
	virtual int GetSRVCount() override { return EGBufferNo_MAX; }

protected:
	// �e�N�X�`���̏�����.
	virtual HRESULT InitBufferTex() override;

private:
	// �X�V.
	void Update();

	// �t���[�����n���l�̍X�V.
	void UpdateFrame();
	// �e����n���l�̍X�V.
	void UpdateShadow();

	// �V���h�E�p�T���v���[�̍쐬.
	HRESULT InitShadowSampler();

	// �f�B�U�[�e�N�X�`���쐬.
	HRESULT CreateDitherTexture();

private:
	ID3D11Buffer*				m_pConstantBufferFrame;		// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*				m_pConstantBufferShadow;	// �R���X�^���g�o�b�t�@.
	ID3D11SamplerState*			m_pShadowMapSampler;		// �V���h�E�}�b�v�p�T���v���[.
	CCascadedShadowMap*			m_pShadowMap;				// �V���O���g��(�O������擾).
	ID3D11ShaderResourceView*	m_pDitherTexture;			// �ʏ�e�N�X�`��.
};

#endif	// #ifndef G_BUFFER_RENDER_H.