#include "CameraManager.h"
#include "..\FreeCamera\FreeCamera.h"
#include "..\..\..\Utility\Input\Input.h"

namespace
{
	constexpr float MOVE_SECONDS_SPEED	= 0.6f;	// フリーカメラの切り替えで移動する秒数.
	constexpr float MOVE_SECONDS_MAX	= 1.0f;	// 最大秒数.
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
	m_pFreeCamera->Update( 0.0f );	// 視点座標を更新しておく.
}

CCameraManager::~CCameraManager()
{
}

//--------------------------------------.
// インスタンスの取得.
//--------------------------------------.
CCameraManager* CCameraManager::GetInstance()
{
	static std::unique_ptr<CCameraManager> pInstance = std::make_unique<CCameraManager>();
	return pInstance.get();
}

//--------------------------------------.
// 更新関数.
//--------------------------------------.
void CCameraManager::Update( const float& deltaTime )
{
	CCameraManager* pInstance = GetInstance();
	if( pInstance->m_pCamera == nullptr ) return;

	if( CKeyInput::IsHold('C') == true && CKeyInput::IsMomentPress('M') == true ){
		SetActiveFreeCamera();
		SetPassiveFreeCamera();
	}

	if( pInstance->m_IsMoveCamera == true ){
		pInstance->ChangeMoveUpdate( deltaTime );
	} else {
		pInstance->m_pCamera->Update( deltaTime );
	}

	pInstance->m_pCamera->UpdateViewProj();

	const D3DXVECTOR3 pos = pInstance->m_pCamera->GetPosition();
	const D3DXVECTOR3 lookPos = pInstance->m_pCamera->GetLookPosition();

	CDebugText::PushText("Camera", CDebugText::SEPARATOR );
	CDebugText::PushText("Camera", "'C + M' Is Light Activce Change" );
	CDebugText::PushText("Camera", "'L_Ctrl + " );
	CDebugText::PushText("Camera", "'D or A' x, 'E or Q' y, 'W or S' z : Position Move" );
	CDebugText::PushText("Camera", "Mouse Left Click : LookPosition Move" );
	CDebugText::PushText("Camera", CDebugText::SEPARATOR );
	CDebugText::PushText("Camera", "IsFreeCamera : ", pInstance->m_IsActiveFreeCamera == true ? "true" : "false" );
	CDebugText::PushTextF("Camera", "Position     : x[%08.3f], y[%08.3f], z[%08.3f]", pos.x,		pos.y,		pos.z );
	CDebugText::PushTextF("Camera", "LookPosition : x[%08.3f], y[%08.3f], z[%08.3f]", lookPos.x,	lookPos.y,	lookPos.z );
	CDebugText::PushText("Camera", CDebugText::SEPARATOR );
}

//--------------------------------------.
// カメラを変更.
//--------------------------------------.
void CCameraManager::ChangeCamera( CCameraBase* pCamera, const bool& isMove, const float& sec )
{
	CCameraManager* pInstance = GetInstance();
	if( pCamera == nullptr ) return;
	if( pInstance->m_pCamera == nullptr ){
		pInstance->m_pCamera = pCamera;
		return;
	}
	if( isMove == true ){
		// 前回の座標と新しい座標を設定.
		pInstance->m_NewState.LookPosition	= pCamera->GetLookPosition();
		pInstance->m_NewState.Position		= pCamera->GetPosition();
		pInstance->m_OldState.LookPosition	= pInstance->m_pCamera->GetLookPosition();
		pInstance->m_OldState.Position		= pInstance->m_pCamera->GetPosition();

		pInstance->m_MoveSeconds		= MOVE_SECONDS_MAX;
		pInstance->m_MoveSecondsSpeed	= sec;
		pInstance->m_IsMoveCamera		= true;
	}
	pInstance->m_pCamera = pCamera;
}

//--------------------------------------.
// フリーカメラを有効化.
//--------------------------------------.
void CCameraManager::SetActiveFreeCamera()
{
	CCameraManager* pInstance = GetInstance();

	if( pInstance->m_IsMoveCamera		== true ) return;
	if( pInstance->m_IsActiveFreeCamera == true ) return;
	if( pInstance->m_pCamera			== nullptr ) return;
	if( pInstance->m_MoveSeconds > 0.0f ) return;


	pInstance->m_pTmpCamera = pInstance->m_pCamera;
	pInstance->m_pCamera = pInstance->m_pFreeCamera.get();

	// 前回の座標と新しい座標を設定.
	pInstance->m_NewState.LookPosition	= pInstance->m_pCamera->GetLookPosition();
	pInstance->m_NewState.Position		= pInstance->m_pCamera->GetPosition();
	pInstance->m_OldState.LookPosition	= pInstance->m_pTmpCamera->GetLookPosition();
	pInstance->m_OldState.Position		= pInstance->m_pTmpCamera->GetPosition();

	pInstance->m_MoveSeconds = MOVE_SECONDS_MAX;
	pInstance->m_MoveSecondsSpeed = MOVE_SECONDS_SPEED;
	pInstance->m_IsActiveFreeCamera = true;
	pInstance->m_IsMoveCamera = true;
}

//--------------------------------------.
// フリーカメラを無効化.
//--------------------------------------.
void CCameraManager::SetPassiveFreeCamera()
{
	CCameraManager* pInstance = GetInstance();

	if( pInstance->m_IsMoveCamera		== true ) return;
	if( pInstance->m_IsActiveFreeCamera	== false ) return;
	if( pInstance->m_pTmpCamera			== nullptr ) return;
	if( pInstance->m_MoveSeconds > 0.0f ) return;

	// 前回の座標と新しい座標を設定.
	pInstance->m_NewState.LookPosition	= pInstance->m_pTmpCamera->GetLookPosition();
	pInstance->m_NewState.Position		= pInstance->m_pTmpCamera->GetPosition();
	pInstance->m_OldState.LookPosition	= pInstance->m_pCamera->GetLookPosition();
	pInstance->m_OldState.Position		= pInstance->m_pCamera->GetPosition();

	pInstance->m_pCamera = pInstance->m_pTmpCamera;
	pInstance->m_MoveSeconds = MOVE_SECONDS_MAX;
	pInstance->m_MoveSecondsSpeed = MOVE_SECONDS_SPEED;

	pInstance->m_IsActiveFreeCamera = false;
	pInstance->m_IsMoveCamera = true;
}

// カメラをnull初期化する.
void CCameraManager::SetNullCamera()
{
	GetInstance()->m_pCamera = nullptr;
}

//--------------------------------------.
// 切り替え移動の更新.
//--------------------------------------.
void CCameraManager::ChangeMoveUpdate( const float& deltaTime )
{
	CCameraManager* pInstance = GetInstance();

	if( pInstance->m_MoveSeconds <= 0.0f ){
		pInstance->m_IsMoveCamera = false;
	}
	pInstance->m_pCamera->AnySecondsMove(
		pInstance->m_NewState,
		pInstance->m_OldState,
		pInstance->m_MoveSeconds );
	pInstance->m_MoveSeconds -= deltaTime/pInstance->m_MoveSecondsSpeed;
}
