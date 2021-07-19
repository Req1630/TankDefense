#ifndef CASCADED_SHADOW_MAP_H
#define CASCADED_SHADOW_MAP_H

#include "..\RenderingTextuer.h"

#include <vector>
#include <functional>

/*********************************************
*	�����V���h�E�}�b�v�`��N���X.
**/
class CCascadedShadowMap : public CRenderTexture
{
	// �o�E���f�B���O�{�b�N�X(�J�X�P�[�h�V���h�E�Ɏg�p).
	struct stBBox
	{
		D3DXVECTOR3 Min;
		D3DXVECTOR3 Max;

		stBBox( D3DXVECTOR3 min, D3DXVECTOR3 max )
			: Min ( min )
			, Max ( max )
		{}

	} typedef SBBox;

	// �R���X�^���g�o�b�t�@.
	struct C_CBUFFER
	{
		D3DXMATRIX ShadowVP;
	};

public:
	CCascadedShadowMap();
	virtual ~CCascadedShadowMap();

	// �C���X�^���X�̎擾.
	static CCascadedShadowMap* GetInstance();

	// ������.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;

	// �X�V.
	void Update();
	// �`��.
	bool Render( const bool& isSkin, const D3DXMATRIX& mW, std::function<void()>& func );

	// �`�悪�I������.
	inline void SetEndRender(){ m_IsEndRender = true; }

	// �o�b�t�@�̐ݒ�.
	virtual void SetBuffer() override;
	// �V�F�[�_�[���\�[�X�r���[�̐����擾.
	virtual int GetSRVCount() override { return m_CascadedNum; }

protected:
	// �e�N�X�`���̏�����.
	virtual HRESULT InitBufferTex() override;

private:
	// ���_�V�F�[�_�[�̍쐬.
	HRESULT InitVertexShader();

	// ���s��������.
	void ComputeSplitPositions(
		const int splitCount, const float lamda, 
		const float nearClip, const float farClip );
	// ���������������8�p�����Ƃ߂āC���C�g�̃r���[�ˉe��Ԃ�AABB�����߂�.
	SBBox CalculateFrustum( float nearClip, float farClip, D3DXMATRIX& viewProj );
	// �N���b�v�s������߂�.
	D3DXMATRIX CreateCropMatrix( SBBox& box );

private:
	ID3D11VertexShader*		m_pSkinVertexShader;	// ���_�V�F�[�_�[.
	ID3D11Buffer*			m_pConstantBufferFrame;	// �R���X�^���g�o�b�t�@.
	std::vector<D3DXMATRIX>	m_ShadowMatrix;			// �V���h�E�p�s��(�J�X�P�[�h���p��).
	std::vector<float>		m_SplitPos;				// �J�X�P�[�h�̋�؂���W.
	D3DXMATRIX				m_ProjMatrix;			// �v���W�F�N�V����.
	D3DXMATRIX				m_ViewMatrix;			// �r���[.
	float					m_MaxClipDistance;		// �ő�N���b�s���O����.
	float					m_MinClipDistance;		// �ŏ��N���b�s���O����.
	int						m_CascadedNum;			// ������.
	bool					m_IsEndRender;

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CCascadedShadowMap( const CCascadedShadowMap & )				= delete;
	CCascadedShadowMap& operator = ( const CCascadedShadowMap & )	= delete;
	CCascadedShadowMap( CCascadedShadowMap && )						= delete;
	CCascadedShadowMap& operator = ( CCascadedShadowMap && )		= delete;
};

#endif	// #ifndef CASCADED_SHADOW_MAP_H.