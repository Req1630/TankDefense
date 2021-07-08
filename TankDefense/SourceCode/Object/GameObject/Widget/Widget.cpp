#include "Widget.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace
{
	constexpr float ALPHA_MAX = 1.0f;	// �A���t�@�ő�l.
	constexpr float ALPHA_MIN = 0.0f;	// �A���t�@�ŏ��l.
};

CWidget::CWidget()
	: m_pSprite			( nullptr )
	, m_RenderStates	()
	, m_Alpha			( ALPHA_MAX )
	, m_AlphaMax		( ALPHA_MAX )
	, m_AlphaMin		( ALPHA_MIN )
	, m_FadeSpeed		( FOR_INIT_ZERO_FLOAT )
	, m_NowFadeState	( EFadeState::Finish )
	, m_OldFadeState	( EFadeState::Finish )
{
}

CWidget::~CWidget()
{
	m_pSprite = nullptr;
}

//--------------------------------------.
// �t�F�[�h�C���̐ݒ�.
//--------------------------------------.
void CWidget::SetFadeIn( const float& speed )
{
	m_NowFadeState = EFadeState::In;
	m_FadeSpeed = speed;
}

//--------------------------------------.
// �t�F�[�h�A�E�g�̐ݒ�.
//--------------------------------------.
void CWidget::SetFadeOut( const float& speed )
{
	m_NowFadeState = EFadeState::Out;
	m_FadeSpeed = speed;
}

//--------------------------------------.
// �摜�̐ݒ�.
//--------------------------------------.
bool CWidget::SpriteSetting( const char* spriteName, const int& stateNum )
{
	// �X�v���C�g�̎擾.
	m_pSprite = CSpriteResource::GetSprite( spriteName );
	if( m_pSprite == nullptr ) return false;

	// �X�v���C�g�`��X�e�[�g��`�搔���擾.
	m_RenderStates.resize( stateNum, m_pSprite->GetRenderState() );

	return true;
}

//--------------------------------------.
// �t�F�[�h�̍X�V.
//--------------------------------------.
void CWidget::FadeUpdate( float& alpha )
{
	switch( m_NowFadeState )
	{
	case EFadeState::In:
		FadeIn( alpha );
		break;
	case EFadeState::Out:
		FadeOut( alpha );
		break;
	default:
		break;
	}
}

//--------------------------------------.
// �t�F�[�h�C��.
//--------------------------------------.
void CWidget::FadeIn( float& alpha )
{
	AlphaCalc( alpha );
	if( m_Alpha <= m_AlphaMin ){
		m_Alpha = m_AlphaMin;
		m_OldFadeState = m_NowFadeState;
		m_NowFadeState = EFadeState::Finish;
	}
}

//--------------------------------------.
// �t�F�[�h�A�E�g.
//--------------------------------------.
void CWidget::FadeOut( float& alpha )
{
	AlphaCalc( alpha );
	if( m_Alpha >= m_AlphaMax ){
		m_Alpha = m_AlphaMax;
		m_OldFadeState = m_NowFadeState;
		m_NowFadeState = EFadeState::Finish;
	}
}

//--------------------------------------.
// �A���t�@�̌v�Z.
//--------------------------------------.
void CWidget::AlphaCalc( float& alpha )
{
	alpha += m_DeltaTime / m_FadeSpeed;
}