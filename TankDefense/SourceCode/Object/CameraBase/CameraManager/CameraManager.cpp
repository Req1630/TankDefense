#include "CameraManager.h"
#include "..\FreeCamera\FreeCamera.h"
#include "..\..\..\Utility\Input\Input.h"

namespace
{
	constexpr float MOVE_SECONDS_SPEED	= 0.6f;	// �t���[�J�����̐؂�ւ��ňړ�����b��.
	constexpr float MOVE_SECONDS_MAX	= 1.0f;	// �ő�b��.
};

CCameraManager::CCameraManager()
	: m_pCamera				( nullptr )
	, m_pTmpCamera			( nullptr )
	, m_pFreeCamera			( nullptr )
	, m_NewState			()
	, m_OldState			()
	, m_MoveSeconds			( 0.0f )
	, m_MoveSecondsSpeed	( MOVE_SECONDS_SPEED )
	, m_IsMoveCamera		( false )
	, m_IsActiveFreeCamera	( false )

	, m_LookCameraTargetPos	()
{
	m_pFreeCamera = std::make_unique<CFreeCamera>();
	m_pCamera = m_pFreeCamera.get();
	m_pFreeCamera->Update( 0.0f );	// ���_���W���X�V���Ă���.
}

CCameraManager::~CCameraManager()
{
}

//--------------------------------------.
// �C���X�^���X�̎擾.
//--------------------------------------.
CCameraManager* CCameraManager::GetInstance()
{
	static std::unique_ptr<CCameraManager> pInstance = std::make_unique<CCameraManager>();
	return pInstance.get();
}

//--------------------------------------.
// �X�V�֐�.
//--------------------------------------.
void CCameraManager::Update( const float& deltaTime )
{
	if( GetInstance()->m_pCamera == nullptr ) return;

	if( CKeyInput::IsHold('C') == true && CKeyInput::IsMomentPress('M') == true ){
		SetActiveFreeCamera();
		SetPassiveFreeCamera();
	}

	if( GetInstance()->m_IsMoveCamera == true ){
		GetInstance()->ChangeMoveUpdate( deltaTime );
	} else {
		GetInstance()->m_pCamera->Update( deltaTime );
	}

	GetInstance()->m_pCamera->UpdateViewProj();

	const D3DXVECTOR3 pos = GetInstance()->m_pCamera->GetPosition();
	const D3DXVECTOR3 lookPos = GetInstance()->m_pCamera->GetLookPosition();

	//CDebugText::PushText( "-----------------");
	//CDebugText::PushText( "---- Camera -----");
	//CDebugText::PushText( "-----------------");
	//CDebugText::PushText( "'C + M' Is Light Activce Change" );
	//CDebugText::PushText( "'L_Ctrl + " );
	//CDebugText::PushText( "'D or A' x, 'E or Q' y, 'W or S' z : Position Move" );
	//CDebugText::PushText( "Mouse Left Click : LookPosition Move" );
	//CDebugText::PushText( "-----------------" );
	//CDebugText::PushText( "IsFreeCamera : ", GetInstance()->m_IsActiveFreeCamera == true ? "true" : "false" );
	//CDebugText::PushText( "Pos      : ", pos.x, ", ", pos.y, ", ", pos.z );
	//CDebugText::PushText( "LookPos  : ", lookPos.x, ", ", lookPos.y, ", ", lookPos.z );
}

//--------------------------------------.
// �J������ύX.
//--------------------------------------.
void CCameraManager::ChangeCamera( CCameraBase* pCamera, const bool& isMove, const float& sec )
{
	if( pCamera == nullptr ) return;
	if( GetInstance()->m_pCamera == nullptr ){
		GetInstance()->m_pCamera = pCamera;
		return;
	}
	if( isMove == true ){
		// �O��̍��W�ƐV�������W��ݒ�.
		GetInstance()->m_NewState.LookPosition	= pCamera->GetLookPosition();
		GetInstance()->m_NewState.Position		= pCamera->GetPosition();
		GetInstance()->m_OldState.LookPosition	= GetInstance()->m_pCamera->GetLookPosition();
		GetInstance()->m_OldState.Position		= GetInstance()->m_pCamera->GetPosition();

		GetInstance()->m_MoveSeconds		= MOVE_SECONDS_MAX;
		GetInstance()->m_MoveSecondsSpeed	= sec;
		GetInstance()->m_IsMoveCamera		= true;
	}
	GetInstance()->m_pCamera = pCamera;
}

//--------------------------------------.
// �t���[�J������L����.
//--------------------------------------.
void CCameraManager::SetActiveFreeCamera()
{
	if( GetInstance()->m_IsMoveCamera		== true ) return;
	if( GetInstance()->m_IsActiveFreeCamera == true ) return;
	if( GetInstance()->m_pCamera			== nullptr ) return;
	if( GetInstance()->m_MoveSeconds > 0.0f ) return;


	GetInstance()->m_pTmpCamera = GetInstance()->m_pCamera;
	GetInstance()->m_pCamera = GetInstance()->m_pFreeCamera.get();

	// �O��̍��W�ƐV�������W��ݒ�.
	GetInstance()->m_NewState.LookPosition	= GetInstance()->m_pCamera->GetLookPosition();
	GetInstance()->m_NewState.Position		= GetInstance()->m_pCamera->GetPosition();
	GetInstance()->m_OldState.LookPosition	= GetInstance()->m_pTmpCamera->GetLookPosition();
	GetInstance()->m_OldState.Position		= GetInstance()->m_pTmpCamera->GetPosition();

	GetInstance()->m_MoveSeconds = MOVE_SECONDS_MAX;
	GetInstance()->m_MoveSecondsSpeed = MOVE_SECONDS_SPEED;
	GetInstance()->m_IsActiveFreeCamera = true;
	GetInstance()->m_IsMoveCamera = true;
}

//--------------------------------------.
// �t���[�J�����𖳌���.
//--------------------------------------.
void CCameraManager::SetPassiveFreeCamera()
{
	if( GetInstance()->m_IsMoveCamera		== true ) return;
	if( GetInstance()->m_IsActiveFreeCamera	== false ) return;
	if( GetInstance()->m_pTmpCamera			== nullptr ) return;
	if( GetInstance()->m_MoveSeconds > 0.0f ) return;

	// �O��̍��W�ƐV�������W��ݒ�.
	GetInstance()->m_NewState.LookPosition	= GetInstance()->m_pTmpCamera->GetLookPosition();
	GetInstance()->m_NewState.Position		= GetInstance()->m_pTmpCamera->GetPosition();
	GetInstance()->m_OldState.LookPosition	= GetInstance()->m_pCamera->GetLookPosition();
	GetInstance()->m_OldState.Position		= GetInstance()->m_pCamera->GetPosition();

	GetInstance()->m_pCamera = GetInstance()->m_pTmpCamera;
	GetInstance()->m_MoveSeconds = MOVE_SECONDS_MAX;
	GetInstance()->m_MoveSecondsSpeed = MOVE_SECONDS_SPEED;

	GetInstance()->m_IsActiveFreeCamera = false;
	GetInstance()->m_IsMoveCamera = true;
}

// �J������null����������.
void CCameraManager::SetNullCamera()
{
	GetInstance()->m_pCamera = nullptr;
}

//--------------------------------------.
// �؂�ւ��ړ��̍X�V.
//--------------------------------------.
void CCameraManager::ChangeMoveUpdate( const float& deltaTime )
{
	if( GetInstance()->m_MoveSeconds <= 0.0f ){
			GetInstance()->m_IsMoveCamera = false;
	}
	GetInstance()->m_pCamera->AnySecondsMove(
		GetInstance()->m_NewState,
		GetInstance()->m_OldState,
		GetInstance()->m_MoveSeconds );
	GetInstance()->m_MoveSeconds -= deltaTime/GetInstance()->m_MoveSecondsSpeed;
}
