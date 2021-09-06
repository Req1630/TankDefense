#include "Bullet.h"
#include <random>
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Utility\Input\Input.h"

CBullet::CBullet()
	: m_pStaticMesh		( nullptr )

	, m_Type			( EType::Normal )
	, m_MoveVec			()

	, m_ControlPoints	()
	, m_Distance		()

	, m_DispTimeCnt		()
	, m_Disp			()
{
	Create();
	Init();
}

CBullet::~CBullet()
{
}

// �������֐�.
bool CBullet::Init()
{
	// ������.
	m_Type			= EType::Normal;
	m_MoveVec		= { 0.0f, 0.0f, 0.0f };
	m_DispTimeCnt	= 0.0f;
	m_Disp			= false;

	InitCollision();
	return true;
}

// �X�V�֐�.
void CBullet::Update( const float& deltaTime )
{
	if ( m_Disp == true ){
		// �e�̃^�C�v�ɉ����Ĉړ�.
		switch ( m_Type ) {
		case EType::Normal:
			m_Tranceform.Position -= m_MoveVec * m_Status.Speed;
			break;
		case EType::Homing:
			m_Distance += m_Status.Speed / 10.0f;
			m_Tranceform.Position = Evaluate( m_ControlPoints, m_Distance );
			break;
		case EType::Penetrating:
			break;
		default:
			break;
		}

		// �������Ԃ����炷.
		m_DispTimeCnt -= deltaTime;

		// �������Ԃ�0�ȉ��ɂȂ�����e���폜����.
		if ( m_DispTimeCnt <= 0.0f ) m_Disp = false;
	}
}

// �`��֐�.
void CBullet::Render()
{
	if ( m_Disp == true ){
		// ���f���̈ړ�.
		m_pStaticMesh->SetTranceform( m_Tranceform );
		// �e�̕`��.
		m_pStaticMesh->Render();
	}
}

// �����蔻��֐�.
void CBullet::Collision( CActor* pActor )
{
}

// �����蔻��̏�����.
void CBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
void CBullet::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( m_Tranceform.Position );
}

// ����̕����ɔ�Ԓe�̔���.
void CBullet::NormalShot( std::string StaticMeshName, EObjectTag ObjTag, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 MoveVec )
{
	// �X�^�e�B�b�N���b�V���̎擾.
	m_pStaticMesh			= CMeshResorce::GetStatic( StaticMeshName );

	m_ObjectTag				= ObjTag;			// �^�O��ݒ�.

	m_Tranceform.Position	= Pos;				// ���W�ݒ�.
	m_Tranceform.Rotation	= Rot;				// ��]�̐ݒ�.
	m_MoveVec				= MoveVec;			// �x�N�g���ݒ�.
	m_Type					= EType::Normal;	// �e�^�C�v�̐ݒ�.
	m_DispTimeCnt			= DISP_TIME;		// �������Ԃ̐ݒ�.

	m_Disp					= true;				// �\��.
}

// �ǔ�����e�̔���.
void CBullet::HomingShot( std::string StaticMeshName, EObjectTag ObjTag, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, D3DXVECTOR3 Rot )
{
	// �X�^�e�B�b�N���b�V���̎擾.
	m_pStaticMesh			= CMeshResorce::GetStatic( StaticMeshName );

	m_ObjectTag				= ObjTag;			// �^�O��ݒ�.

	m_Tranceform.Position	= StartPos;			// ���W�ݒ�.
	m_Tranceform.Rotation	= Rot;				// ��]�̐ݒ�.
	m_Type					= EType::Homing;	// �e�^�C�v�̐ݒ�.
	m_DispTimeCnt			= DISP_TIME;		// �������Ԃ̐ݒ�.
	
	// �J�n���W����I�����W�܂ł̃x�N�g���̍쐬.
	D3DXVECTOR3 LengthVec = StartPos - EndPos;
	D3DXVECTOR3 Vec;
	D3DXVec3Normalize( &Vec, &LengthVec );
	// �������擾.
	float Length	= D3DXVec3Length( &LengthVec );
	// �x�N�g�����ړ������邽�߂̈ړ��x�N�g���̍쐬.
	D3DXVECTOR3 MoveVec;
	MoveVec.x		=  Vec.z;
	MoveVec.y		=  0.0f;
	MoveVec.z		= -Vec.x;
	// �����_���̐ݒ�.
	std::random_device	rnd;
	std::mt19937		mt( rnd() );
	std::uniform_int_distribution<>			Rand_LR			( 0, 2 );
	std::uniform_real_distribution<float>	Rand_Length		( 1.0f, Length );
	std::uniform_real_distribution<float>	Rand_Distance	( 10.0f, 30.0f );
	// ���ԍ��W�̐ݒ�.
	D3DXVECTOR3 MiddlePos;
	MiddlePos = Vec * Rand_Length( mt );
	// ���ԍ��W�̈ړ�.
	if ( Rand_LR( mt ) )
		MiddlePos += MoveVec * Rand_Distance( mt );
	else
		MiddlePos -= MoveVec * Rand_Distance( mt );

	// �x�W�F�Ȑ��̐ݒ�.
	m_ControlPoints[0]		= StartPos;		// �x�W�F�Ȑ��̊J�n���W.
	m_ControlPoints[1]		= MiddlePos;	// �x�W�F�Ȑ��̒��ԍ��W.
	m_ControlPoints[2]		= EndPos;		// �x�W�F�Ȑ��̏I�����W.

	m_Disp = true;							// �\��.
}

// �쐬�֐�.
void CBullet::Create()
{
	// �x�W�F�Ȑ��̐ݒ�.
	m_ControlPoints.resize( 3 );
}

// �x�W�F�Ȑ��֐�.
D3DXVECTOR3 CBullet::Evaluate( std::vector<D3DXVECTOR3> ControlPoints, float t )
{
	m_ControlPoints = ControlPoints;

	D3DXVECTOR3 result = { 0.0f, 0.0f, 0.0f };
	int n = ControlPoints.size();
	for ( int i = 0; i < n; i++ )
	{
		result += m_ControlPoints[i] * Bernstein( n - 1, i, t );
	}

	return result;
}

// �o�[���X�^�C�����֐�.
float CBullet::Bernstein( int n, int i, float t )
{
	return Binomial( n, i ) * pow( t, i ) * pow( 1 - t, n - i );
}

// �񍀌W�����v�Z����.
float CBullet::Binomial( int n, int k )
{
	return Factorial( n ) / ( Factorial( k ) * Factorial( n - k ) );
}

// �K����v�Z����.
float CBullet::Factorial( int a )
{
	float result = 1.0f;
	for ( int i = 2; i <= a; i++ )
	{
		result *= i;
	}

	return result;
}
