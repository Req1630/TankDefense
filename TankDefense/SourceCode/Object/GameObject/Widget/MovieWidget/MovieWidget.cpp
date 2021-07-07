#include "MovieWidget.h"


CMovieWidget::CMovieWidget()
	: m_pRenderState		( nullptr )
	, m_MovieWidgetState	()
	, m_NowTime				( 0.0f )
{
}

CMovieWidget::~CMovieWidget()
{
}

//--------------------------------.
// èâä˙âªä÷êî.
//--------------------------------.
bool CMovieWidget::Init()
{
	if( SpriteSetting( m_MovieWidgetState.SpriteName.c_str() ) == false ) return false;
	m_pRenderState = &m_RenderStates.back();

	SettingRenderState();

	return true;
}

//--------------------------------.
// çXêVä÷êî.
//--------------------------------.
void CMovieWidget::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;
	m_NowTime += m_DeltaTime;

	if( m_NowTime < m_MovieWidgetState.AciveStartTime ) return;

}

//--------------------------------.
// ï`âÊä÷êî.
//--------------------------------.
void CMovieWidget::Render()
{
	if( m_NowTime < m_MovieWidgetState.AciveStartTime ) return;
	if( m_pSprite == nullptr ) return;
	m_pSprite->Render( m_pRenderState );
}

//--------------------------------.
// íPèÉÇ…ï`âÊÇ∑ÇÈ.
//--------------------------------.
void CMovieWidget::SimpleRender()
{
	if( m_pSprite == nullptr ) return;
	m_pSprite->Render( m_pRenderState );
}

//--------------------------------.
// ï`âÊèÓïÒÇê›íËÇ∑ÇÈ.
//--------------------------------.
void CMovieWidget::SettingRenderState()
{
	m_MovieWidgetState.AnimState.SetAnimState( &m_pRenderState->AnimState );
	m_pRenderState->Tranceform.Position.x = m_MovieWidgetState.Position.x;
	m_pRenderState->Tranceform.Position.y = m_MovieWidgetState.Position.y;
}