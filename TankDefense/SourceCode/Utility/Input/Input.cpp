#include "Input.h"

CInput::CInput()
	: m_hWnd			( nullptr )
	, m_KeyBindList		()
	, m_AxisBindList	()
	, m_MouseNowPoint	{ 0, 0 }
	, m_MouseOldPoint	{ 0, 0 }
	, m_MouseWheel		( 0 )
{
}

CInput::~CInput()
{
}

//--------------------------------------------------.
// �o�C���h�̏�����.
//--------------------------------------------------.
void CInput::InitBind()
{
	InitKeyBind();	// �L�[�o�C���h�̏�����.
	InitAxisBind();	// ���o�C���h�̏�����.
}

//--------------------------------------------------.
// �o�C���h�̏�����.
//--------------------------------------------------.
void CInput::InitBind(
	std::function<void(std::unordered_map<EKeyBind, SKeyBindPair>&)>	keyFunc,
	std::function<void(std::unordered_map<EAxisBind, SAxisBind>&)>		axisFunc )
{
	keyFunc( GetInstance()->m_KeyBindList );	// �L�[�o�C���h�̏�����.
	axisFunc( GetInstance()->m_AxisBindList );	// ���o�C���h�̏�����.
}

//--------------------------------------------------.
// �L�[�o�C���h�̏�����.
//--------------------------------------------------.
void CInput::InitKeyBind()
{
	GetInstance()->m_KeyBindList =
	{
		// �L�[�o�C���h�񋓑�,		�ݒ肷��L�[�A�Q�[���p�b�h�̃{�^���̐ݒ�.
		{ EKeyBind::Decision,		SKeyBindPair( VK_RETURN,	XINPUT_GAMEPAD_B )				},
		{ EKeyBind::Cancel,			SKeyBindPair( VK_BACK,		XINPUT_GAMEPAD_A )				},
		{ EKeyBind::Skip,			SKeyBindPair( VK_RETURN,	XINPUT_GAMEPAD_B )				},
		
		{ EKeyBind::Attack,			SKeyBindPair( 'F',			XINPUT_GAMEPAD_X )				},
		{ EKeyBind::SpecialAbility,	SKeyBindPair( 'Y',			XINPUT_GAMEPAD_Y )				},
		{ EKeyBind::Avoidance,		SKeyBindPair( 'R',			XINPUT_GAMEPAD_A )				},
		{ EKeyBind::STGAttack,		SKeyBindPair( 'Z',			XINPUT_GAMEPAD_RIGHT_SHOULDER )	},

		{ EKeyBind::Up,				SKeyBindPair( VK_UP,		XINPUT_GAMEPAD_DPAD_UP )		},
		{ EKeyBind::Down,			SKeyBindPair( VK_DOWN,		XINPUT_GAMEPAD_DPAD_DOWN )		},
		{ EKeyBind::Right,			SKeyBindPair( VK_RIGHT,		XINPUT_GAMEPAD_DPAD_RIGHT )		},
		{ EKeyBind::Left,			SKeyBindPair( VK_LEFT,		XINPUT_GAMEPAD_DPAD_LEFT )		},

		{ EKeyBind::Start,			SKeyBindPair( VK_ESCAPE,	XINPUT_GAMEPAD_START )			},
		{ EKeyBind::Back,			SKeyBindPair( VK_BACK,		XINPUT_GAMEPAD_BACK )			},

		{ EKeyBind::Edit_ActorPut,	SKeyBindPair( VK_SPACE,		XINPUT_GAMEPAD_X )				},
		{ EKeyBind::Edit_BackMenu,	SKeyBindPair( VK_BACK,		XINPUT_GAMEPAD_BACK )			},
		{ EKeyBind::Edit_Up,		SKeyBindPair( 'R',			XINPUT_GAMEPAD_B )				},
		{ EKeyBind::Edit_Down,		SKeyBindPair( 'F',			XINPUT_GAMEPAD_A )				},
		{ EKeyBind::Edit_RightRot,	SKeyBindPair( 'E',			XINPUT_GAMEPAD_RIGHT_SHOULDER )	},
		{ EKeyBind::Edit_LeftRot,	SKeyBindPair( 'Q',			XINPUT_GAMEPAD_LEFT_SHOULDER )	},

	};
}

//--------------------------------------------------.
// ���o�C���h�̏�����.
//--------------------------------------------------.
void CInput::InitAxisBind()
{
	GetInstance()->m_AxisBindList = 
	{
		// ���o�C���h�̗񋓑�	�ݒ肷��L�[�A					�󂯎�鎲�̊֐��A							�󂯎��ŏ��l�A�ő�l.
		{ EAxisBind::L_Forward,	SAxisBind( 'S',		'W',		[](){ return CXInput::GetLeftThumbY(); },	-1.0f, 1.0f ) },
		{ EAxisBind::L_Right,	SAxisBind( 'A',		'D',		[](){ return CXInput::GetLeftThumbX(); },	-1.0f, 1.0f ) },
		{ EAxisBind::R_Forward,	SAxisBind( VK_UP,	VK_DOWN,	[](){ return CXInput::GetRightThumbY(); },	-1.0f, 1.0f ) },
		{ EAxisBind::R_Right,	SAxisBind( VK_LEFT,	VK_RIGHT,	[](){ return CXInput::GetRightThumbX(); },	-1.0f, 1.0f ) },
	};
}

//--------------------------------------------------.
// �L�[�o�C���h�̏�����.
//--------------------------------------------------.
void CInput::InitKeyBind( std::function<void(std::unordered_map<EKeyBind, SKeyBindPair>&)> func )
{
	// �֐����Ăяo��.
	func( GetInstance()->m_KeyBindList );
}

//--------------------------------------------------.
// ���o�C���h�̏����� : �O������ݒ�p.
//--------------------------------------------------.
void CInput::InitAxisBind( std::function<void(std::unordered_map<EAxisBind, SAxisBind>&)> func )
{
	// �֐����Ăяo��.
	func( GetInstance()->m_AxisBindList );
}

//--------------------------------------------------.
// ���͏�Ԃ̍X�V.
//--------------------------------------------------.
void CInput::Update( const float& time )
{
	CXInput::Update( time );
	CKeyInput::Update();
	GetInstance()->UpdateMouse();
}

//--------------------------------------------------.
// ������Ă���Ƃ�.
//--------------------------------------------------.
bool CInput::IsPress( const EKeyBind& key )
{
	return 
		CKeyInput::IsPress( GetInstance()->m_KeyBindList[key].Key ) ||
		CXInput::IsPress( GetInstance()->m_KeyBindList[key].Button );
}

//--------------------------------------------------.
// �����ꂽ�u��.
//--------------------------------------------------.
bool CInput::IsMomentPress( const EKeyBind& key )
{
	return 
		CKeyInput::IsMomentPress( GetInstance()->m_KeyBindList[key].Key ) || 
		CXInput::IsMomentPress( GetInstance()->m_KeyBindList[key].Button );
}

//--------------------------------------------------.
// ���������Ă���Ƃ�.
//--------------------------------------------------.
bool CInput::IsHold( const EKeyBind& key )
{
	return 
		CKeyInput::IsHold( GetInstance()->m_KeyBindList[key].Key ) ||
		CXInput::IsHold( GetInstance()->m_KeyBindList[key].Button );
}

//--------------------------------------------------.
// �������u��.
//--------------------------------------------------.
bool CInput::IsRelease( const EKeyBind& key )
{
	return 
		CKeyInput::IsRelease( GetInstance()->m_KeyBindList[key].Key ) ||
		CXInput::IsRelease( GetInstance()->m_KeyBindList[key].Button );
}

//--------------------------------------------------.
// �����Ă��Ȃ�.
//--------------------------------------------------.
bool CInput::NotPress( const EKeyBind& key )
{
	// �ǂ�������������Ă����� false��Ԃ�.
	if( !CKeyInput::IsPress( GetInstance()->m_KeyBindList[key].Key ) == false ||
		!CXInput::IsPress( GetInstance()->m_KeyBindList[key].Button ) == false ){
		return false;
	}
	return true;
}

//--------------------------------------------------.
// ���l�̎擾(vector2).
//--------------------------------------------------.
CInput::Vector2 CInput::GetAxisVector( const EAxisBind& key_x, const EAxisBind& key_y )
{
	Vector2 t;
	t.x = GetAxisValue(key_x);
	t.y = GetAxisValue(key_y);

	if( fabsf(t.x) < INPUT_AXIS_DEAD_ZONE && 
		fabsf(t.y) < INPUT_AXIS_DEAD_ZONE ){
		t.x = 0.0f; t.y = 0.0f;
	}

	return t;
}

#ifdef	__D3DX9MATH_H__
//--------------------------------------------------.
// ���l�̎擾(D3DXVECTOR2).
//--------------------------------------------------.
D3DXVECTOR2 CInput::GetAxisDxVector( const EAxisBind& key_x, const EAxisBind& key_y )
{
	D3DXVECTOR2 t;
	t.x = GetAxisValue(key_x);
	t.y = GetAxisValue(key_y);

	if( fabsf(t.x) < INPUT_AXIS_DEAD_ZONE && 
		fabsf(t.y) < INPUT_AXIS_DEAD_ZONE ){
		t.x = 0.0f; t.y = 0.0f;
	}

	return t;
}
#endif	// #ifdef __D3DX9MATH_H__

//--------------------------------------------------.
// ���l�̎擾.
//--------------------------------------------------.
float CInput::GetAxisValue( const EAxisBind& key )
{
	float value = 0.0f;
	if( CKeyInput::IsPress( GetInstance()->m_AxisBindList[key].PlusKey ) == true ){
		value += GetInstance()->m_AxisBindList[key].MaxValue;
	}
	if( CKeyInput::IsPress( GetInstance()->m_AxisBindList[key].MinusKey ) == true ){
		value += GetInstance()->m_AxisBindList[key].MinValue;
	}
	if( value != 0.0f ) return value;

	// �m�[�}���C�Y�����l���擾.
	value = static_cast<float>(GetInstance()->m_AxisBindList[key].GetValue())/static_cast<float>(SHRT_MAX);
	value = pow( value, INPUT_AXIS_POW_VALUE );
	if( fabsf(value) < 0.01f ) value = 0.0f;

	return value;
}

//--------------------------------------------------.
// �}�E�X���X�N���[���̒���.
//--------------------------------------------------.
bool CInput::IsScreenMiddleMouse()
{
	RECT rect;
	if( GetClientRect( GetInstance()->m_hWnd, &rect ) == FALSE ) return false;
	return (( rect.left < GetInstance()->m_MouseNowPoint.x && GetInstance()->m_MouseNowPoint.x < rect.right ) &&
			( rect.top < GetInstance()->m_MouseNowPoint.y && GetInstance()->m_MouseNowPoint.y < rect.bottom ));
}

//--------------------------------------------------.
// �C���X�^���X�̎擾.
//--------------------------------------------------.
CInput* CInput::GetInstance()
{
	static std::unique_ptr<CInput> pInstance = std::make_unique<CInput>();
	return pInstance.get();
}

//--------------------------------------------------.
// �}�E�X�̍X�V.
//--------------------------------------------------.
void CInput::UpdateMouse()
{
	// �O���ݒ�.
	m_MouseOldPoint = m_MouseNowPoint;
	// �}�E�X�J�[�\���ʒu�擾.
	GetCursorPos( &m_MouseNowPoint );
	// �X�N���[�����W���N���C�A���g���W�ɕϊ�.
	ScreenToClient( m_hWnd, &m_MouseNowPoint );
}