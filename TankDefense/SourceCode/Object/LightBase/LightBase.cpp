#include "LightBase.h"
#include "..\..\Utility\Input\Input.h"

namespace
{
	const D3DXVECTOR3	DEFFULT_LIGHT_POSITON	= { 50.0f, 60.0f, -10.0f };
	constexpr float		MOVE_SPEED				= 1.0f;
};

CLightBase::CLightBase()
	: m_LookPosition	( -10.0f, -10.0f, 10.0f )
	, m_Direction		( 0.0f, 0.0f, 0.0f )
	, m_Color			( 1.0f, 1.0f, 1.0f )
	, m_Intensity		( 1.0f )
	, m_IsActive		( false )
{
	m_Tranceform.Position = DEFFULT_LIGHT_POSITON;
}

CLightBase::~CLightBase()
{
}

// �V���v���Ȉړ�.
void CLightBase::SimpleMove( const float& deltaTime )
{
	if( m_IsActive == false ) return;

	D3DXVECTOR3* changePos = nullptr;

	if( CKeyInput::IsHold('P') == true || CXInput::IsHold(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		changePos = &m_Tranceform.Position;
	if( CKeyInput::IsHold('L') == true || CXInput::IsHold(XINPUT_GAMEPAD_LEFT_SHOULDER))
		changePos = &m_LookPosition;

	if( changePos == nullptr ) return;

	// �O�i.
	if( CKeyInput::IsHold('W') == true || CXInput::GetLeftThumbY() > IDLE_THUMB_MAX )
		changePos->z += MOVE_SPEED * deltaTime;
	// ���.
	if( CKeyInput::IsHold('S') == true || CXInput::GetLeftThumbY() < IDLE_THUMB_MIN )
		changePos->z -= MOVE_SPEED * deltaTime;
	// �E�Ɉړ�.
	if( CKeyInput::IsHold('D') == true || CXInput::GetLeftThumbX() > IDLE_THUMB_MAX )
		changePos->x += MOVE_SPEED * deltaTime;
	// ���Ɉړ�.
	if( CKeyInput::IsHold('A') == true || CXInput::GetLeftThumbX() < IDLE_THUMB_MIN )
		changePos->x -= MOVE_SPEED * deltaTime;
	// �㏸.
	if( CKeyInput::IsHold('E') == true || CXInput::GetRightTrigger() > IDLE_TIGGER_MAX ) 
		changePos->y += MOVE_SPEED * deltaTime;
	// ���~.
	if( CKeyInput::IsHold('Q') == true || CXInput::GetLeftTrigger() > IDLE_TIGGER_MAX )
		changePos->y -= MOVE_SPEED * deltaTime;
}