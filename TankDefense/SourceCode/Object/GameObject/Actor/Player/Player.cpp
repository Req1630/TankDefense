#include "Player.h"
#include <DirectXMath.h>	// XMConvertToRadians()�ŕK�v.
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\..\Object\CameraBase\RotLookAtCenter\RotLookAtCenter.h"
#include "..\Weapon\Weapon.h"
#include "..\Weapon\DefaultWepon\DefaultWepon.h"
#include "..\Weapon\MachineGun\MachineGun.h"
#include "..\Weapon\Missile\Missile.h"
#include "..\Weapon\Beam\Beam.h"

namespace{
	const float ROT_REST_RANG	= DirectX::XMConvertToRadians( 60.0f );	// ��]�̐����p�x(�x).
}

CPlayer::CPlayer()
	: CPlayer( nullptr )
{
}

CPlayer::CPlayer( const std::shared_ptr<CBulletManager> pBulletMng )
	: m_pSkinMesh		( nullptr )
	, m_pLookCamera		( nullptr )
	, m_pBulletMng		( pBulletMng )
	, m_pWeapon			( nullptr )
	, m_pDefaultWepon	( nullptr )
	, m_MoveVec			()
	, m_DireVec			()
	, m_AlignmentPos	()
	, m_Status			()
{
	m_ObjectTag = enObjectTagList::Player;

	Create();
	Init();
}

CPlayer::~CPlayer()
{
}

// �������֐�.
bool CPlayer::Init()
{
	// ������.
	m_MoveVec		= { 0.0f, 0.0f, 0.0f };
	m_DireVec		= { 0.0f, 0.0f, 0.0f };
	m_AlignmentPos	= { 0.0f, 0.0f, 0.0f };

	// ��C���Z�b�g.
	m_pWeapon		= m_pDefaultWepon;

	// �X�e�[�^�X�̐ݒ�.
	m_Status.Hp		= 100.0f;
	m_Status.Speed	= 0.2f;

	InitCollision();
	return true;
}

// �X�V�֐�.
void CPlayer::Update( const float& deltaTime )
{
	Controller();					// ����.
	CameraController();				// �J�����̑���.
	AttackController();				// �U������.

	Move();							// �ړ�.
	CameraUpdate();					// �J�����̍X�V.

	m_pWeapon->Update( deltaTime );	// ����̍X�V.
	DebugUpdate();					// �f�o�b�N�̍X�V.

	UpdateCollision();
}

// �`��֐�.
void CPlayer::Render()
{
	// ���f���̕`��.
	m_pSkinMesh->SetTranceform( m_Tranceform );
	m_pSkinMesh->SetShadowDepth( 0.1f );	// ���̊֐��Ŏ������g�ɂ�����e�̔Z����ݒ�ł���.
	m_pSkinMesh->Render();
	m_pWeapon->Render();
}

// �����蔻��֐�.
void CPlayer::Collision( CActor* pActor )
{
}

// �����蔻��̏�����.
void CPlayer::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
void CPlayer::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// �쐬�֐�.
void CPlayer::Create()
{
	// �J�����̐ݒ�.
	m_pLookCamera	= std::make_unique<CRotLookAtCenter>();
	CCameraManager::ChangeCamera( m_pLookCamera.get() );
	m_pLookCamera	->SetHeight( 10.0f );

	// ����̐ݒ�.
	m_pDefaultWepon = std::make_shared<CDefaultWepon>();
	m_pDefaultWepon	->SetBulletMng( m_pBulletMng );

	// �X�^�e�B�b�N���b�V���̎擾.
	m_pSkinMesh		= CMeshResorce::GetSkin( "syatai_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
}

// ����֐�.
void CPlayer::Controller()
{
	// �����x�N�g���̍X�V.
	m_DireVec.x = sinf( m_CameraRot.y );
	m_DireVec.z = cosf( m_CameraRot.y );

	// �ړ��x�N�g���̍X�V.
	m_MoveVec = { 0.0f, 0.0f, 0.0f };
	if ( CKeyInput::IsPress( VK_UP		) ) m_MoveVec =  m_DireVec;
	if ( CKeyInput::IsPress( VK_DOWN	) ) m_MoveVec = -m_DireVec;
}

// �J�����̑���֐�.
void CPlayer::CameraController()
{
	if ( CKeyInput::IsPress( VK_LEFT	) ) m_pLookCamera->DegreeHorizontalLeftMove();
	if ( CKeyInput::IsPress( VK_RIGHT	) ) m_pLookCamera->DegreeHorizontalRightMove();
}

// �U���֐�.
void CPlayer::AttackController()
{
	// �e�̔���.
	if ( CKeyInput::IsPress( VK_SPACE ) ){
		if ( m_pWeapon->Shot( m_DireVec ) == false ){
			// �������C�ɖ߂�.
			ChangeWeapon( m_pDefaultWepon );
		};
	}
}

// �ړ��֐�.
void CPlayer::Move()
{
	D3DXVECTOR3* Pos = &m_Tranceform.Position;
	D3DXVECTOR3* Rot = &m_Tranceform.Rotation;

	// �ړ��x�N�g���ňړ�.
	*Pos -= m_MoveVec * m_Status.Speed;

	// �{�[�����W���擾.
	D3DXVECTOR3 BoneBodyPos		= m_Tranceform.Position;
	m_pSkinMesh->GetPosFromBone( "tyuusinjiku", &BoneBodyPos );

	// ����̈ړ�.
	m_pWeapon->Move( BoneBodyPos, m_CameraRot );
	
	// �ړ����Ă���ꍇ
	const float WRotY		= m_pWeapon->GetRotation().y;
	const float CMoveSpd	= m_pLookCamera->GetMoveSpeed();
	if ( m_MoveVec != D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ){
		// �J�����̉�]���x��2�{�̑��x�ŉ�]�������𑵂���.
		if ( Rot->y > WRotY ) Rot->y -= CMoveSpd * 2.0f;
		if ( Rot->y < WRotY ) Rot->y += CMoveSpd * 2.0f;
	}
	// ��~���Ă���ꍇ.
	else{
		if ( Rot->y - WRotY >= ROT_REST_RANG )
			Rot->y -= CMoveSpd;
		else if ( Rot->y - WRotY <= -ROT_REST_RANG )
			Rot->y += CMoveSpd;
	}
}

// �J�����̍X�V�֐�.
void CPlayer::CameraUpdate()
{
	// �J������ݒ�.
	m_pLookCamera->RotationLookAtObject( m_Tranceform.Position, m_Status.Speed );

	// �㉺.
	m_pLookCamera->SetLookPosition( m_Tranceform.Position + m_AlignmentPos );

	// �J�����̉�]�l�̍X�V.
	m_CameraRot = D3DXVECTOR3( 0.0f, m_pLookCamera->GetRadianX(), 0.0f );
}

// �f�o�b�N�̍X�V�֐�.
void CPlayer::DebugUpdate()
{
#ifdef _DEBUG
	const D3DXVECTOR3 Pos = m_Tranceform.Position;
	const D3DXVECTOR3 Rot = m_Tranceform.Rotation;
	CDebugText::PushText( "Player", "------------------" );
	CDebugText::PushText( "Player", "----  Player  ----" );
	CDebugText::PushText( "Player", "------------------" );
	CDebugText::PushText( "Player", "L_Ctrl + " );
	CDebugText::PushText( "Player", "'1' GetDefaultWepon, '2' GetMachineGun, '3' GetMissile, '4' GetBeam" );
	CDebugText::PushText( "Player", "'U' AlignmentPos_UP, 'D' AlignmentPos_Down" );
	CDebugText::PushText( "Player", "-------------------" );
	CDebugText::PushText( "Player", "Pos     : ", Pos.x, ", ", Pos.y, ", ", Pos.z );
	CDebugText::PushText( "Player", "Ros     : ", Rot.x, ", ", Rot.y, ", ", Rot.z );
	CDebugText::PushText( "Player", "MoveVec : ", m_MoveVec.x, ", ", m_MoveVec.y, ", ", m_MoveVec.z );
	CDebugText::PushText( "Player", "DireVec : ", m_DireVec.x, ", ", m_DireVec.y, ", ", m_DireVec.z );
	CDebugText::PushText( "Player", "Weapon  : ", m_pWeapon->GetObjectTag() == EObjectTag::DefaultWepon ? "DefaultWepon" : m_pWeapon->GetObjectTag() == EObjectTag::MachineGun ? "MachineGun" : m_pWeapon->GetObjectTag() == EObjectTag::Missile ? "Missile" : "Beam" );
	CDebugText::PushText( "Player", "HP      : ", m_Status.Hp );
	CDebugText::PushText( "Player", "Speed   : ", m_Status.Speed );

	if ( !CKeyInput::IsPress( VK_LCONTROL ) ) return;
	if ( CKeyInput::IsMomentPress( '1' ) ) ChangeWeapon( m_pDefaultWepon	);

	if ( CKeyInput::IsPress( 'U' ) ) m_AlignmentPos.y += 0.1f;
	if ( CKeyInput::IsPress( 'D' ) ) m_AlignmentPos.y -= 0.1f;
#endif
}

// ����̕ύX�֐�.
void CPlayer::ChangeWeapon( const std::shared_ptr<CWeapon> pWeapon )
{
	// ����̕ύX.
	m_pWeapon = pWeapon;
	m_pWeapon->SetBulletMng( m_pBulletMng );

	// ����̏�����.
	m_pWeapon->Init();
}
