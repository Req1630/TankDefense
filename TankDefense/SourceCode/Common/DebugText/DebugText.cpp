#include "DebugText.h"
#include "..\Font\Font.h"
#include "..\Sprite\SpriteRender.h"
#include "..\Sprite\Sprite.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Utility\Input\Input.h"

// localtime��unsafe�x���G���[�𖳌���.
#pragma warning(disable:4996)

#include <memory>
#include <chrono>
#include <iomanip>

namespace
{
	constexpr	int		INDEX_SUB_MAX		= 21;								// �\���s�T�C�Y.
	constexpr	float	RENDER_SIZE			= 0.4f;								// �`��T�C�Y.
	constexpr	float	PAGE_RENDER_COLOR[]	= { 0.90f, 0.50f, 0.50f, 1.0f };	// �`��F.
	constexpr	float	RENDER_COLOR[]		= { 0.85f, 0.85f, 0.85f, 1.0f };	// �`��F.
	constexpr	char	SPRITE_NAME[]		= "debugBack";						// �摜��.
};

CDebugText::CDebugText()
	: m_TextList		()
	, m_TextCountList	()
	, m_pFont			( nullptr )
	, m_pSpriteRender	( nullptr )
	, m_TagList			()
	, m_pBackSprite		( nullptr )
	, m_NowTagName		( LOG_NAME )
	, m_Mutex			()
	, m_BackColor		( { 0.0f, 0.0f, 0.0f, 0.5f } )
	, m_FontColor		( RENDER_COLOR )
	, m_BackTextureSize	( 0.0f, 0.0f )
	, m_TextIndex		( 0 )
	, m_NowTagIndex		( 0 )
	, m_NowColorChangeNo( 0 )
#ifdef _DEBUG
	, m_IsRender		( true )
#else
	, m_IsRender		( false )
#endif	// #ifdef _DEBUG.
	, m_IsMoveing		( false )
{
	m_pFont			= std::make_unique<CFont>();
	m_pSpriteRender	= std::make_unique<CSpriteRender>();
}

CDebugText::~CDebugText()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CDebugText* CDebugText::GetInstance()
{
	static std::unique_ptr<CDebugText> pInstance = std::make_unique<CDebugText>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}

//----------------------------.
// �������֐�.
//----------------------------.
HRESULT CDebugText::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( GetInstance()->SetDevice( pContext11 ) ))		return E_FAIL;
	if( FAILED( GetInstance()->m_pFont->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( GetInstance()->m_pSpriteRender->Init( pContext11 ) ))	return E_FAIL;

	GetInstance()->m_pFont->SetScale( RENDER_SIZE );
	GetInstance()->m_pFont->SetColor( RENDER_COLOR );

	const std::string msg = GetInstance()->GetTime() + "Init Debug Text";
	GetInstance()->m_TextList[LOG_NAME].emplace_back( msg );

	return S_OK;
}

//----------------------------.
// �`��֐�.
//----------------------------.
void CDebugText::Render()
{
	// �f�o�b�O�e�L�X�g�̕`��؂�ւ�.
	if( CKeyInput::IsPress(VK_LCONTROL) && CKeyInput::IsMomentPress(VK_F7) ) GetInstance()->m_IsRender = !GetInstance()->m_IsRender;

	GetInstance()->TextureLoad();

	if( GetInstance()->m_pSpriteRender		== nullptr )	return;
	if( GetInstance()->m_pBackSprite		== nullptr )	return;
	if( GetInstance()->m_IsRender			== false )		return;

	GetInstance()->Update();			// �X�V.
	GetInstance()->BackSpriteRender();	// �w�i�`��.
	GetInstance()->FontRender();		// �t�H���g�̕`��.
}

//----------------------------.
// �X�V�֐�.
//----------------------------.
void CDebugText::Update()
{
	ChangeColor();	// �F�̐؂�ւ�.
	ChangePage();	// �y�[�W�̐؂�ւ�.

	// �}�E�X�������Ȃ�I��.
	if( CInput::IsScreenMiddleMouse() == false ) return;

	// �}�E�X���W����ʊO�Ȃ�I��.
	POINT mousePos = CInput::GetMousePosition();
	if( m_Tranceform.Position.x > mousePos.x || mousePos.x > (m_Tranceform.Position.x+m_BackTextureSize.x) ||
		m_Tranceform.Position.y > mousePos.y || mousePos.y > (m_Tranceform.Position.y+m_BackTextureSize.y) ){
		m_IsMoveing = false;
		return;
	}

	// �e�L�X�g�̕\���ʒu��ύX����.
	int mouseDelta = CInput::GetMouseWheelDelta();	// �z�C�[���l���擾.
	if( mouseDelta < 0 ) m_TextIndex++;				// �������ֈړ�.
	if( mouseDelta > 0 ) m_TextIndex--;				// ������ֈړ�.

	// �X�N���[���͈͂�ݒ�.
	int listSize = 0;
	for( auto& t : m_TextList[m_NowTagName] ) if( t.empty() == false ) listSize++;
	const int indexMax	= listSize > INDEX_SUB_MAX ? abs(INDEX_SUB_MAX-listSize) : listSize-INDEX_SUB_MAX-1;

	// �l�͈̔͊O�̒���.
	if( m_TextIndex > indexMax )	m_TextIndex = indexMax;
	if( m_TextIndex < 0 )			m_TextIndex = 0;

	if( CKeyInput::IsMomentPress(VK_LBUTTON) == true ) m_IsMoveing = true;
	if( CKeyInput::IsRelease(VK_LBUTTON) == true ){
		m_IsMoveing = false;
		return;
	}

	if( m_IsMoveing == false ) return;

	// �O��̍��W�ƍ��̍��W�̍����`����W�ɑ���.
	const float ySub = CInput::GetMousePosisionY() - CInput::GetMouseOldPosisionY();
	const float xSub = CInput::GetMousePosisionX() - CInput::GetMouseOldPosisionX();

	m_Tranceform.Position.x += xSub;
	m_Tranceform.Position.y += ySub;
}

//----------------------------.
// �e�N�X�`���̓ǂݍ���.
//----------------------------.
void CDebugText::TextureLoad()
{
	if( m_pBackSprite != nullptr ) return;
	m_pBackSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if( m_pBackSprite == nullptr ) return;

	// �摜�T�C�Y���擾.
	m_BackTextureSize.x		= m_pBackSprite->GetTextureSize().w;
	m_BackTextureSize.y		= m_pBackSprite->GetTextureSize().h;
	m_Tranceform.Position	= m_pBackSprite->GetRenderPos();
	if( m_BackTextureSize.x	== 0.0f ) m_pBackSprite = nullptr;
}

//----------------------------.
// �y�[�W�̐؂�ւ�.
//----------------------------.
void CDebugText::ChangePage()
{
	m_TagList.clear();
	for( auto& t : m_TextList ) m_TagList.emplace_back( t.first );

	bool isChange = false;
	if( CKeyInput::IsHold( VK_LCONTROL ) && CKeyInput::IsMomentPress( VK_RIGHT ) ){
		m_NowTagIndex++;
		isChange = true;
	}
	if( CKeyInput::IsHold( VK_LCONTROL ) && CKeyInput::IsMomentPress( VK_LEFT ) ){
		m_NowTagIndex--;
		isChange = true;
	}

	// �؂�ւ����s���Ă��Ȃ���ΏI��.
	if( isChange == false ) return;

	const int tagSize = m_TagList.size();

	if( m_NowTagIndex > tagSize-1 )	m_NowTagIndex = 0;
	if( m_NowTagIndex < 0 )			m_NowTagIndex = tagSize-1;

	m_NowTagName = m_TagList[m_NowTagIndex];
}

//----------------------------.
// �F�̕ύX.
//----------------------------.
void CDebugText::ChangeColor()
{
	PushText( "Index", SEPARATOR );
	PushText( "Index", "Color Key = R:'1' G:'2' B:'3' A:'4'" );
	PushText( "Index", "Des Font Color = 'F'" );
	PushText( "Index", "Des Back Color = 'B'" );
	PushText( "Index", "'Left Ctrl' + Des Key + Color Key + 'Mouse Wheel'" );
	PushText( "Index", " " );
	PushTextF( "Index", "Font Color : R[%.2f], G[%.2f], B[%.2f], A[%.2f]",
			 m_FontColor.x, m_FontColor.y, m_FontColor.z, m_FontColor.w );
	PushTextF( "Index", "Back Color : R[%.2f], G[%.2f], B[%.2f], A[%.2f]",
			 m_BackColor.x, m_BackColor.y, m_BackColor.z, m_BackColor.w );
	PushText( "Index", SEPARATOR );

	if( !CKeyInput::IsHold( VK_LCONTROL ) ) return;

	D3DXVECTOR4* pColor = nullptr;
	if( CKeyInput::IsHold( 'F' ) ) pColor = &m_FontColor;
	if( CKeyInput::IsHold( 'B' ) ) pColor = &m_BackColor;

	if( pColor == nullptr ) return;

	const float speed = static_cast<float>(CInput::GetMouseWheelDelta()) * 0.001f;

	auto changeColor = []( float* pColor, const float& speed )
	{
		*pColor += speed;
		if( *pColor > 1.0f ) *pColor = 1.0f;
		if( *pColor < 0.0f ) *pColor = 0.0f;
	};

	if( CKeyInput::IsHold( '1' ) ) changeColor( &pColor->x, speed );
	if( CKeyInput::IsHold( '2' ) ) changeColor( &pColor->y, speed );
	if( CKeyInput::IsHold( '3' ) ) changeColor( &pColor->z, speed );
	if( CKeyInput::IsHold( '4' ) ) changeColor( &pColor->w, speed );
}

//----------------------------.
// �w�i�摜�̕`��.
//----------------------------.
void CDebugText::BackSpriteRender()
{
	SSpriteRenderState ss;
	ss.Tranceform = m_Tranceform;
	ss.Color = m_BackColor;
	m_pSpriteRender->Render( ss, m_pBackSprite->GetTexture(), m_pBackSprite->GetVertexBufferUI() );
}

//----------------------------.
// �t�H���g�̕`��.
//----------------------------.
void CDebugText::FontRender()
{
	// �`����W���擾.
	D3DXVECTOR3 pos = m_Tranceform.Position;
	pos.x += (m_pFont->FONT_SIZE*0.3f * m_Tranceform.Scale.x);

	PageRender( pos );
	
	// �L���[�̃T�C�Y���擾.
	int i = 0;
	for( auto& t : m_TextList[m_NowTagName] ){
		if( t.empty() == true ) continue;
		// �e�L�X�g�̃C���f�b�N�X���L���͈͊O�Ȃ�I��.
		if( i < m_TextIndex ){
			if( m_NowTagName != LOG_NAME ) t = "";
			i++;
			continue;
		}
		if( i >= INDEX_SUB_MAX ) break;

		// �e�L�X�g���w�i�O�Ȃ�I��.
		if( pos.y < m_Tranceform.Position.y+m_BackTextureSize.y ){
			m_pFont->SetColor( m_FontColor );
			m_pFont->SetPosition( pos );
			m_pFont->RenderUI( t );
		}
		if( m_NowTagName != LOG_NAME ) t = "";

		// ���W�����ɉ��낷.
		pos.y += (m_pFont->FONT_SIZE*0.6f * m_Tranceform.Scale.x);
		i++;
	}
	if( m_NowTagName == LOG_NAME ) return;
	m_TextCountList[m_NowTagName] = 0;
}

//----------------------------.
// �y�[�W�̕`��.
//----------------------------.
void CDebugText::PageRender( D3DXVECTOR3& pos )
{
	if( m_TagList.empty() == true ) return;
	m_NowTagName = m_TagList[m_NowTagIndex];
	const std::string backTag = m_NowTagName;
	const std::string nextTag = m_NowTagName;
	char pageString[256];
	sprintf( pageString, " [%s] << Back [%s %d/%d] Next >> [%s]",
			 backTag.c_str(), m_NowTagName.c_str(), m_NowTagIndex+1, m_TagList.size(), nextTag.c_str() );

	const std::vector<std::string> textList =
	{
		"-------------------------  Page  -------------------------",
		pageString,
		"    'Left Ctrl' + '<-' or '->'",
		SEPARATOR,
	};

	pos.y += (m_pFont->FONT_SIZE*0.2f * m_Tranceform.Scale.x);
	for( auto& t : textList ){
		// �e�L�X�g���w�i�O�Ȃ�I��.
		if( pos.y < m_Tranceform.Position.y+m_BackTextureSize.y ){
			m_pFont->SetColor( PAGE_RENDER_COLOR );
			m_pFont->SetPosition( pos );
			m_pFont->RenderUI( t );
		}
		pos.y += (m_pFont->FONT_SIZE*0.6f * m_Tranceform.Scale.x);
	}
}

//----------------------------.
// ���Ԃ̎擾.
//----------------------------.
std::string CDebugText::GetTime()
{
	auto now	= std::chrono::system_clock::now();
	auto now_c	= std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time( localtime(&now_c), "%T" );
	std::string timeMsg = "> [" + ss.str() + "] ";
	return timeMsg;
}