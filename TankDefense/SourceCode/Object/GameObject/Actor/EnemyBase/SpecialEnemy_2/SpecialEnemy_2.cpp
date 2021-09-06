#include "SpecialEnemy_2.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\Restraint\Restraint.h"

CSpecialEnemy_2::CSpecialEnemy_2()
	: m_pEnemyBase		()
	, m_pRestraint		( nullptr )
	, m_pSprite			( nullptr )
	, m_ConfuCnt		( 0 )
	, m_IsConfu			( false )

{
	m_pRestraint	= std::make_shared<CRestraint>();
	m_ObjectTag		= EObjectTag::SpecialEnemy_2;
	Init();
}

CSpecialEnemy_2::~CSpecialEnemy_2()
{
}

// �������֐�.
bool CSpecialEnemy_2::Init()
{
	if ( m_pRestraint->Init() == false ) return false;	// �S���̏�����.

	m_pSkinMesh = CMeshResorce::GetSkin( "a_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	m_pSprite = CSpriteResource::GetSprite( "hukidasi1" );
	
	Spawn( D3DXVECTOR3( -5.0f, -2.5f, -20.0f ) );
	m_IsConfu = false;
	m_IsDelete = false;

	InitCollision();	// �����蔻��̏�����.
	
	return true;
}

// �X�V�֐�.
void CSpecialEnemy_2::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// ���݂̏�ԍX�V.

	// �X�|�[��.
	if ( CKeyInput::IsMomentPress( 'Y' ) == true ) m_IsDelete = false;
	// ���S.
	if ( CKeyInput::IsMomentPress( 'H' ) == true ) m_IsDelete = true;
	// �����ōS���U��������.
	if ( CKeyInput::IsMomentPress( VK_LSHIFT ) == true ) m_pRestraint->SetIsRestraint( false );

	DebugUpdate();			// �f�o�b�N�X�V.

	UpdateCollision();	// �����蔻��̍X�V.
}

// �`��֐�.
void CSpecialEnemy_2::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetTranceform( m_Tranceform );

	if ( m_IsDelete == false ) m_pSkinMesh->Render();
	m_pRestraint->Render();

	
}

// Sprite3D�`��֐�.
void CSpecialEnemy_2::Sprite3DRender()
{
	// �����摜��3D�`��.
	if ( m_IsConfu == true ){
		SSpriteRenderState ConfuSprite = m_pSprite->GetRenderState();
		ConfuSprite.Tranceform.Position = { m_Tranceform.Position.x - 2.5f, m_Tranceform.Position.y + 12.0f, m_Tranceform.Position.z };
		ConfuSprite.Tranceform.Scale = { 50.0f, 50.0f, 50.0f };
		ConfuSprite.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_pSprite->Render3D( false, &ConfuSprite );
		//m_pSprite->Render();
	}
}

// �����蔻��֐�.
void CSpecialEnemy_2::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
	if ( m_pRestraint->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	SpecialEnemy1Collsion( pActor );
	RestraintCollsion( pActor );
}

// �����蔻��̏�����.
void CSpecialEnemy_2::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
//	Update�֐��̍Ō�ɌĂ�.
void CSpecialEnemy_2::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y, m_Tranceform.Position.z ) );
}

// �X�|�[��.
bool CSpecialEnemy_2::Spawn( const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if ( m_NowState != Enemy::EEnemyState::None ) return true;
	m_Tranceform.Position = spawnPos;			// �X�|�[�����W�̐ݒ�.
	//m_LifePoint = pPARAMETER->LifeMax;		// �̗͂̐ݒ�.
	m_NowState = Enemy::EEnemyState::Spawn;		// ���݂̏�Ԃ��X�|�[���ɕύX.
	return true;
}

// ������W�̐ݒ�.
void CSpecialEnemy_2::SetTargetPos( CActor& actor )
{
	// �v���C���[�̍��W���擾.
	CEnemyBase::SetTargetPos( actor );
}

// �X�|�[����.
void CSpecialEnemy_2::Spawning()
{
	CEnemyBase::Spawning();
}

// �ړ�.
void CSpecialEnemy_2::Move()
{
	// �v���C���[��ǐ�.
	if ( m_IsConfu == false ) {
		SetMoveVector( m_TargetPosition );								// �ړ��x�N�g���̐ݒ�.
		TargetRotation( m_MoveSpeed );									// ��].
		CEnemyBase::VectorMove( m_MoveSpeed, RESTRAINT_ATTACK_LENGTH );	// �ړ�.
		Attack();														// �U��.
		CEnemyBase::WaitMove();											// �ҋ@.
	}
	// �������.
	else {
		SetRandMoveVector( m_RandPos );
		if ( m_IsRandMove == false ) {
			Rand_Pos();
		}
		else {
			RandTargetRotation( m_MoveSpeed );
			RandVectorMove( m_MoveSpeed );
			CEnemyBase::WaitMove();		// �ҋ@.
		}

		// ������Ԃ��牽�b�o������.
		m_ConfuCnt++;								// �����J�E���g���Z.
		if ( m_ConfuCnt < 10.0*FPS ) return;		// 10�b�o�������������.
		m_IsConfu = false;							// ������Ԃ���������.
		m_IsRandMove = false;							// �����_�����W��ς���.
		m_NowMoveState = Enemy::EMoveState::Wait;	// �ҋ@��Ԃɂ���.
		m_ConfuCnt = 0;								// �����J�E���g�̏�����.
	}
}

// ����.
void CSpecialEnemy_2::Fright()
{
	CEnemyBase::Fright();
}

// ���S.
void CSpecialEnemy_2::Death()
{
	CEnemyBase::Death();
}

// �U���֐�.
void CSpecialEnemy_2::Attack()
{
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;

	if ( m_pRestraint->GetIsRestraint() == false ) m_pRestraint->SetIsRestraint( true );
	m_pRestraint->Update( m_DeltaTime );
	m_pRestraint->SetRestraintPos( m_Tranceform.Position, m_MoveVec3 );

	// �����ōS���U��������.
	if ( CKeyInput::IsMomentPress( VK_LSHIFT ) == true ) m_pRestraint->SetIsRestraint( false );

	// �S���U���Ńv���C���[�͑���s�\�ɂȂ�.
	// �S�����̓X�e�B�b�N�̃��o�K�`�������Ȃ��Ɣ������Ȃ�.
	// �e�X�g�i�K�͑���ɍ��V�t�g��A�łŎg��.
	// �S�����������ꂽ�����ȓG2�͍�����ԂɂȂ�.

	// �S�����������ꂽ��ҋ@��ԂɑJ�ڂ���.
	if ( m_pRestraint->GetIsRestraint() == true ) return;
	m_IsConfu = true;	// ������Ԃɂ���.
	m_NowMoveState = Enemy::EMoveState::Wait;
}

// �v���C���[�Ƃ̓����蔻��.
void CSpecialEnemy_2::PlayerCollsion( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// ����ȓG2�ƃv���C���[�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// ����ȓG2�ƃv���C���[�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// ����ȓG2�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// ����ȓG1�Ƃ̓����蔻��.
void CSpecialEnemy_2::SpecialEnemy1Collsion( CActor * pActor )
{
	// �I�u�W�F�N�g�̃^�O������ȓG1����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_1 ) return;

	// ����ȓG2�Ɠ���ȓG1�̓����蔻��̎���.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//���a.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// ����ȓG2�Ɠ���ȓG1�̋����ivector�j

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// ����ȓG2�̎h�����Ă��钷��

	D3DXVECTOR3 vecPushOut;// �����o��vector
	D3DXVec3Normalize( &vecPushOut, &Distance );// �o0 - �o1�̐��K���޸�� 

	// �޸�قɒ����������Ďh�����������݂��������߂�
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// �S���U���̓����蔻��.
void CSpecialEnemy_2::RestraintCollsion( CActor * pActor )
{
	// �S���������ꂽ�Ƃ��ɒʂ�.
	pActor->SetIsRestraint( [&]( bool& isRestraint )
	{
		isRestraint = false;
	});

	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// ���S���Ă�����I��.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// ���ݏ�ԂȂ�I��.

	// �S���U���ƎG���G�̓����蔻��.
	if ( coll::IsSphereToSphere( m_pRestraint->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// �v���C���[�͑���s�\��ԂɂȂ�.
	pActor->SetIsRestraint( [&]( bool& isRestraint )
	{
		isRestraint = true;
	});
}

// �f�o�b�N�X�V�֐�.
void CSpecialEnemy_2::DebugUpdate()
{
#ifdef _DEBUG
	CDebugText::PushText("SpecialEnemy_2", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_2", "--------------- SpecialEnemy_2 --------------");
	CDebugText::PushText("SpecialEnemy_2", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_2", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z);
	CDebugText::PushText("SpecialEnemy_2", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z);
	CDebugText::PushText("SpecialEnemy_2", "'Y' IsDelete = false, 'H' IsDelete = true");
	CDebugText::PushText("SpecialEnemy_2", "IsDelete : ", m_IsDelete == true ? "true" : "false");
	CDebugText::PushText("SpecialEnemy_2", "m_IsConfu : ", m_IsConfu == true ? "true" : "false");
	CDebugText::PushText("SpecialEnemy_2", "m_IsRestraint : ", m_pRestraint->GetIsRestraint() == true ? "true" : "false");
	// ���肶���̈ʒu���o��.
#endif
}
