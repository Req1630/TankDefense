#include "Font.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\Sprite\SpriteRender.h"

CFont::CFont()
	: m_pSpriteRender	( nullptr )
	, m_pVertexBuffer3D	( nullptr )
	, m_pVertexBufferUI	( nullptr )
{
	m_pSpriteRender = std::make_unique<CSpriteRender>();
}

CFont::~CFont()
{
	Release();
}

//---------------------------------.
// ������.
//---------------------------------.
HRESULT CFont::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( SetDevice( pContext11 ) ))				return E_FAIL;
	if( FAILED( m_pSpriteRender->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( CreateVerTexBuffer() ))					return E_FAIL;

	return S_OK;
}

//---------------------------------.
// ���.
//---------------------------------.
void CFont::Release()
{
	SAFE_RELEASE( m_pVertexBufferUI );
	SAFE_RELEASE( m_pVertexBuffer3D );
}

//---------------------------------.
// UI�ŕ`��.
//---------------------------------.
void CFont::RenderUI( const std::string& text )
{
	// �����������[�v.
	float posX = m_Tranceform.Position.x;
	for( int i = 0; i < static_cast<int>(text.length()); i++ ){
		std::string f = text.substr( i, 1 );
		if( IsDBCSLeadByte( text[i] ) == TRUE ){
			f = text.substr( i++, 2 );	// �S�p����.
		} else {
			f = text.substr( i, 1 );	// ���p����.
		}
		RenderFontUI( f.c_str() );
		m_Tranceform.Position.x += (FONT_SIZE) * m_Tranceform.Scale.x;
	}
	m_Tranceform.Position.x = posX;
}

//---------------------------------.
// 3D�ŕ`��.
//---------------------------------.
void CFont::Render3D( const std::string& text, const bool& isBillboard )
{
	// �����������[�v.
	float posX = m_Tranceform.Position.x;
	for( int i = 0; i < static_cast<int>(text.length()); i++ ){
		std::string f = text.substr( i, 1 );
		if( IsDBCSLeadByte( text[i] ) == TRUE ){
			f = text.substr( i++, 2 );	// �S�p����.
		} else {
			f = text.substr( i, 1 );	// ���p����.
		}
		RenderFont3D( f.c_str(), isBillboard );
	}
}

//---------------------------------.
// �e�N�X�`���̕`��.
//---------------------------------.
void CFont::RenderFontUI( const char* c )
{
	ID3D11ShaderResourceView* pResourceView = CFontResource::GetTexture2D(c);	// ���\�[�X�r���[.

	SSpriteRenderState renderState;
	renderState.Tranceform	= m_Tranceform;
	renderState.Color		= m_Color;
	renderState.Tranceform.Scale.y *= 1.2f;
	m_pSpriteRender->Render( renderState, pResourceView, m_pVertexBufferUI );
}

//---------------------------------.
// �e�N�X�`���̕`��.
//---------------------------------.
void CFont::RenderFont3D( const char* c, const bool& isBillboard  )
{
	ID3D11ShaderResourceView* pResourceView = CFontResource::GetTexture2D(c);	// ���\�[�X�r���[.

	SSpriteRenderState renderState;
	renderState.Tranceform	= m_Tranceform;
	renderState.Color		= m_Color;
	m_pSpriteRender->Render( renderState, pResourceView, m_pVertexBuffer3D );
}

//---------------------------------.
// �o�[�e�b�N�X�o�b�t�@�̍쐬.
//---------------------------------.
HRESULT CFont::CreateVerTexBuffer()
{
	SpriteVertex verticesUI[] =
	{
		D3DXVECTOR3( 0.0f,			 FONT_SIZE*1.2f,	0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3( 0.0f,			 0.0f,				0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( FONT_SIZE*1.0f, FONT_SIZE*1.2f,	0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( FONT_SIZE*1.0f, 0.0f,				0.0f ),	D3DXVECTOR2( 1.0f, 0.0f )
	};
	// �ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(verticesUI) / sizeof(verticesUI[0]);

	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@.
	bd.ByteWidth			= sizeof(SpriteVertex)*uVerMax;	//���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;		// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;		// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;		// �\���̂̃T�C�Y(���g�p).

	// �T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = verticesUI;		// �|���̒��_���Z�b�g.

	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBufferUI ) ) ){
		ERROR_MESSAGE("���_�ޯ̧�쐬���s");
		return E_FAIL;
	}

	SpriteVertex vertices[] =
	{
		D3DXVECTOR3(-1.0,  1.0f, 0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3(-1.0, -1.0f, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( 1.0,  1.0f, 0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( 1.0, -1.0f, 0.0f ),	D3DXVECTOR2( 1.0f, 0.0f )
	};
	// �ő�v�f�����Z�o����.
	uVerMax = sizeof(vertices) / sizeof(vertices[0]);
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@.
	bd.ByteWidth			= sizeof(SpriteVertex)*uVerMax;	//���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;		// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;		// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;		// �\���̂̃T�C�Y(���g�p).

	InitData.pSysMem = vertices;		// �|���̒��_���Z�b�g.
	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer3D ) ) ){
		ERROR_MESSAGE("���_�ޯ̧�쐬���s");
		return E_FAIL;
	}
	return S_OK;
}