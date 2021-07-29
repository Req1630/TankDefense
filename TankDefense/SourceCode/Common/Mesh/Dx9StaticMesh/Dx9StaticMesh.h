#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"
#include "..\MeshStruct.h"

#include <functional>

class CCascadedShadowMap;

/*************************************************
*	�X�^�e�B�b�N���b�V��.
**/
class CDX9StaticMesh : public CCommon
{
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;		// ���_���W.
		D3DXVECTOR3	Normal;		// �@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2	Tex;		// �e�N�X�`�����W.
		D3DXVECTOR3	Tangent;	// .
		D3DXVECTOR3	Binormal;	// �@��(�A�e�v�Z�ɕK�{).
	};
	// �}�e���A���\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;					// �A���r�G���g.
		D3DXVECTOR4	Diffuse;					// �f�B�t���[�Y.
		D3DXVECTOR4	Specular;					// �X�y�L����.
		char szTextureName[64];					// �e�N�X�`���t�@�C����.
		ID3D11ShaderResourceView* pTexture;		// �e�N�X�`��.
		ID3D11ShaderResourceView* pNormTexture;	// �@���e�N�X�`��.
		DWORD dwNumFace;						// ���̃}�e���A���̃|���S����.

		MY_MATERIAL()
			: Ambient		()
			, Diffuse		()
			, Specular		()
			, szTextureName	()
			, pTexture		( nullptr )
			, pNormTexture	( nullptr )
			, dwNumFace		( 0)
		{
		}

		~MY_MATERIAL() 
		{
			SAFE_RELEASE(pNormTexture);
			SAFE_RELEASE(pTexture);
		}
	};
public:
	CDX9StaticMesh( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName )
		: CDX9StaticMesh()
	{
		Init( hWnd, pDevice11, pContext11, pDevice9, fileName );
	}

	CDX9StaticMesh();
	~CDX9StaticMesh();

	// �������֐�.
	HRESULT Init(
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );

	// �`��֐�.
	void Render();

	// ���b�V���̎擾.
	inline LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

	// ���g�ɂ�����e�̔Z��.
	//	�L�����N�^�[�͔����A�n�ʂ͔Z���Ȃ�.
	//	�͈� 0 ~ 1.
	inline void SetShadowDepth( const float& depth ){ m_ShadowDepth = depth; }

private:
	// ���b�V���ǂݍ���.
	HRESULT LoadXMesh(const char* fileName);
	// ����֐�.
	void Release();
	// �V�F�[�_�[�쐬.
	HRESULT InitShader();

	// ���b�V���̃����_�����O.
	void RenderMesh(
		D3DXMATRIX& mWorld, 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj );
	
private:
	HWND				m_hWnd;					// �E�B���h�E�n���h��.
	LPDIRECT3DDEVICE9	m_pDevice9;				// Dx9�f�o�C�X.
	ID3D11VertexShader*	m_pVertexShader;		// ���_�V�F�[�_�[.
	ID3D11InputLayout*	m_pVertexLayout;		// ���_���C�A�E�g.
	ID3D11PixelShader*	m_pPixelShader;			// �s�N�Z���V�F�[�_�[.
	ID3D11Buffer*		m_pCBufferPerMesh;		// �R���X�^���g�o�b�t�@(���b�V����).
	ID3D11Buffer*		m_pCBufferPerMaterial;	// �R���X�^���g�o�b�t�@(�}�e���A����).
	ID3D11Buffer*		m_pCBufferPerFrame;		// �R���X�^���g�o�b�t�@(�t���[����).

	ID3D11Buffer*		m_pVertexBuffer;		// ���_�o�b�t�@.
	ID3D11Buffer**		m_ppIndexBuffer;		// �C���f�b�N�X�o�b�t�@.
	ID3D11SamplerState*	m_pSampleLinear;		// �T���v��.

	CCascadedShadowMap*	m_pShadowMap;			// �V���h�E�}�b�v(�O������|�C���^���擾.).

	LPD3DXMESH			m_pMesh;				// ���b�V���I�u�W�F�N�g.
	LPD3DXMESH			m_pMeshForRay;			// ���C�Ƃ̓����蔻��p���b�V���I�u�W�F�N�g.
	DWORD				m_NumMaterials;			// �}�e���A����.

	MY_MATERIAL*	m_pMaterials;				// �}�e���A���\����.
	DWORD			m_NumAttr;					// ������.
	DWORD			m_AttrID[300];				// ����ID ��300�����܂�.

	float			m_ShadowDepth;				// ���g�ɂ�����e�̔Z��.

	std::function<void()>	m_ShadowRenderFunc;

 };

#endif	// #ifndef DX9_STATIC_MESH_H.