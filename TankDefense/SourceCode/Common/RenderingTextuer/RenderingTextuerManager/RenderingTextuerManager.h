#ifndef RENDERING_TEXTURE_MANAGER_H
#define RENDERING_TEXTURE_MANAGER_H

#include "..\RenderingTextuer.h"
#include <functional>

class CGBufferRender;
class CLightingRender;
class COutLineRender;
class CBloomRender;

/********************************************
*	�����_�����O�e�N�X�`���̊Ǘ��N���X.
**/
class CRenderingTexterManager : public CRenderTexture
{
	// �`��t���O.
	enum enRenderFlag : unsigned int
	{
		ERenderFlag_None,

		ERenderFlag_Light	= 1 << 0,
		ERenderFlag_OutLine	= 1 << 1,
		ERenderFlag_Bloom	= 1 << 2,

		ERenderFlag_OllOn	= 0xffffffff,
		ERenderFlag_Max		= ERenderFlag_OllOn,
	} typedef ERenderFlag;
public:
	CRenderingTexterManager();
	~CRenderingTexterManager();

	// ������.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;
	// ���.
	virtual void Release() override;

	// �`��֐�.
	void Render( std::function<void()>& func );

	// �T�C�Y�ύX.
	HRESULT ResizeTexture();

private:
	// �e�摜����������.
	void SynthesizeTexture( ID3D11ShaderResourceView* pSRV );
	// �ŏI�`��.
	void LastRender( ID3D11ShaderResourceView* pSRV );

	// �o�b�t�@�̐ݒ�.
	virtual void SetBuffer() override;
	// �e�N�X�`���̏�����.
	virtual HRESULT InitBufferTex() override;
	// �V�F�[�_�[���\�[�X�r���[�̐����擾.
	virtual int GetSRVCount()  override { return 0; }

	// �s�N�Z���V�F�[�_�[�̏�����.
	HRESULT InitLastPixelShader();

private:
	ID3D11PixelShader*					m_pLastPixelShader;	// �s�N�Z���V�F�[�_�[.
	std::unique_ptr<CGBufferRender>		m_pGBuffer;
	std::unique_ptr<CLightingRender>	m_pLighting;
	std::unique_ptr<COutLineRender>		m_pOutLine;
	std::unique_ptr<CBloomRender>		m_pBloom;
	unsigned int						m_RenderFlag;
};

#endif	// #ifndef RENDERING_TEXTURE_MANAGER_H.