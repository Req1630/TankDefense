#include "Player.h"
#include <DirectXMath.h>	// XMConvertToRadians()�ŕK�v.
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include "..\..\..\..\Common\Mesh\DX9StaticMesh\DX9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\..\Object\CameraBase\RotLookAtCenter\RotLookAtCenter.h"
#include "..\Weapon\Weapon.h"
#include "..\Weapon\DefaultWepon\DefaultWepon.h"
#include "..\Weapon\MachineGun\MachineGun.h"
#include "..\Weapon\Missile\Missile.h"

namespace{
	const float ROT_REST_DANG	= 60.0f;	// ��]�̐����p�x(�x).
}

// ���W�A���ϊ��֐�.
inline float DxToRadian( float Ang ) {
	return DirectX::XMConvertToRadians( Ang );
}

CPlayer::CPlayer()
	: m_pStaticMesh		( nullptr )
	, m_pLookCamera		( nullptr )
	, m_pWeapon			( nullptr )
	, m_pDefaultWepon	( nullptr )
	, m_pMachineGun		( nullptr )
	, m_MoveVec			()
	, m_Status			()
{
	Init();
}

CPlayer::~CPlayer()
{
}

// �������֐�.
bool CPlayer::Init()
{
	// �J�����̐ݒ�.
	m_pLookCamera	= std::make_unique<CRotLookAtCenter>();
	CCameraManager::ChangeCamera( m_pLookCamera.get() );
	m_pLookCamera->SetHeight( 10.0f );

	// ����̐ݒ�.
	m_pDefaultWepon = std::make_shared<CDefaultWepon>();
	m_pMachineGun	= std::make_shared<CMachineGun>();
	m_pMissile		= std::make_shared<CMissile>();

	// ��C���Z�b�g.
	m_pWeapon		= m_pMissile;

	// �X�^�e�B�b�N���b�V���̎擾.
	m_pStaticMesh = CMeshResorce::GetStatic( "syatai" );

	// �X�e�[�^�X�̐ݒ�.
	m_Status.Hp		= 100.0f;
	m_Status.Speed	= 0.2f;

	return true;
}

// �X�V�֐�.
void CPlayer::Update( const float & deltaTime )
{
	Controller();					// ����.
	CameraController();				// �J�����̑���.
	AttackController();				// �U������.

	Move();							// �ړ�.
	CameraUpdate();					// �J�����̍X�V.

	m_pWeapon->Update( deltaTime );	// ����̍X�V.

	if ( CKeyInput::IsMomentPress( 'N' ) )m_pWeapon = m_pDefaultWepon;
	if ( CKeyInput::IsMomentPress( 'M' ) )m_pWeapon = m_pMachineGun;

	UpdateCollision();
}

// �`��֐�.
void CPlayer::Render()
{
	/* �������� �ꉞ�`�悷�钼�O�ɍ��W�n��ݒ肵�Ă������� �������� */
	m_pStaticMesh->SetRotation( m_Tranceform.Rotation );
	m_pStaticMesh->SetPosition( m_Tranceform.Position );
//	m_pStaticMesh->SetTranceform( m_Tranceform );	// ���̂悤�ɂ��ł���.

	// ���f���̕`��.
	m_pStaticMesh->SetShadowDepth( 0.1f );	// ���̊֐��Ŏ������g�ɂ�����e�̔Z����ݒ�ł���.
	m_pStaticMesh->Render();
	m_pWeapon->Render();
}

// �����蔻��֐�.
void CPlayer::Collision( CActor* pActor )
{
}

// �����蔻��̏�����.
void CPlayer::InitCollision()
{
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
void CPlayer::UpdateCollision()
{
}

// ����֐�.
void CPlayer::Controller()
{
	// �ړ��x�N�g���̏�����.
	m_MoveVec			= { 0.0f, 0.0f, 0.0f };

	if ( CKeyInput::IsPress( VK_UP ) ){
		// �O�i�p�̈ړ��x�N�g����p��.
		m_MoveVec.x		= sinf( m_CameraRot.y );
		m_MoveVec.z		= cosf( m_CameraRot.y );
	}
	if ( CKeyInput::IsPress( VK_DOWN ) ){
		// ��i�p�̈ړ��x�N�g����p��.
		m_MoveVec.x		= -sinf( m_CameraRot.y );
		m_MoveVec.z		= -cosf( m_CameraRot.y );
	}
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
	if ( CKeyInput::IsPress( VK_SPACE ) ){
		// �e�̈ړ��x�N�g���̍쐬.
		D3DXVECTOR3 ShotMoveVec;
		ShotMoveVec.x = sinf( m_CameraRot.y );
		ShotMoveVec.z = cosf( m_CameraRot.y );

		// �e�̔���.
		m_pWeapon->Shot( ShotMoveVec );
	}
}

// �ړ��֐�.
void CPlayer::Move()
{
	// �ړ��x�N�g���ňړ�.
	m_Tranceform.Position -= m_MoveVec * m_Status.Speed;

	/* �� */
	D3DXVECTOR3 BonePos = m_Tranceform.Position;
	BonePos.y += 1.2f;

	// ����̈ړ�.
	m_pWeapon->Move( BonePos, m_CameraRot );
	
	// �ړ����Ă���ꍇ
	if ( m_MoveVec != D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ){
		// �J�����̉�]���x��2�{�̑��x�ŉ�]�������𑵂���.
		if ( m_Tranceform.Rotation.y > m_pWeapon->GetRotation().y )
			m_Tranceform.Rotation.y -= m_pLookCamera->GetMoveSpeed() * 2.0f;
		if ( m_Tranceform.Rotation.y < m_pWeapon->GetRotation().y ) 
			m_Tranceform.Rotation.y += m_pLookCamera->GetMoveSpeed() * 2.0f;
	}
	// ��~���Ă���ꍇ.
	else{
		// �ړ������ړ��Ȃ�ԑ̂���]������.
		if ( m_Tranceform.Rotation.y - m_pWeapon->GetRotation().y >= DxToRadian( ROT_REST_DANG ) )
			m_Tranceform.Rotation.y -= m_pLookCamera->GetMoveSpeed();
		else if ( m_Tranceform.Rotation.y - m_pWeapon->GetRotation().y <= DxToRadian( -ROT_REST_DANG ) )
			m_Tranceform.Rotation.y += m_pLookCamera->GetMoveSpeed();
	}
}

// �J�����̍X�V.
void CPlayer::CameraUpdate()
{
	// �J������ݒ�.
	m_pLookCamera->RotationLookAtObject( m_Tranceform.Position, m_Status.Speed );

	// �㉺.
	static D3DXVECTOR3 AddPos = { 0.0f, 0.0f, 0.0f };
	if ( CKeyInput::IsPress( '1' ) ) AddPos.y += 0.1f;
	if ( CKeyInput::IsPress( '2' ) ) AddPos.y -= 0.1f;
	m_pLookCamera->SetLookPosition( m_Tranceform.Position + AddPos );

	// �J�����̉�]�l�̍X�V.
	m_CameraRot = D3DXVECTOR3( 0.0f, m_pLookCamera->GetRadianX(), 0.0f );
}