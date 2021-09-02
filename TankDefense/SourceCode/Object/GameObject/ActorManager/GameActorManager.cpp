#include "GameActorManager.h"
#include "..\Actor\Player\Player.h"
#include "..\Actor\EnemyBase\Enemy\Enemy.h"
#include "..\Actor\EnemyBase\SpecialEnemy_1\SpecialEnemy_1.h"
#include "..\Actor\EnemyBase\SpecialEnemy_2\SpecialEnemy_2.h"

CGameActorManager::CGameActorManager()
	: m_pPlayer				( nullptr )
	, m_pEnemy				( nullptr )
	, m_pSpecialEnemy_1		( nullptr )
	, m_pSpecialEnemy_2		( nullptr )
{
	m_pPlayer		  = std::make_shared<CPlayer>();
	m_pEnemy		  = std::make_shared<CEnemy>();
	m_pSpecialEnemy_1 = std::make_shared<CSpecialEnemy_1>();
	m_pSpecialEnemy_2 = std::make_shared<CSpecialEnemy_2>();
}

CGameActorManager::~CGameActorManager()
{
}

// �������֐�.
bool CGameActorManager::Init()
{
	if ( m_pPlayer->Init()		   == false ) return false;	// �v���C���[�̏�����.
	if ( m_pEnemy->Init()		   == false ) return false;	// �G���G�̏�����.
	if ( m_pSpecialEnemy_1->Init() == false ) return false;	// ����ȓG1�̏�����.
	if ( m_pSpecialEnemy_2->Init() == false ) return false;	// ����ȓG2�̏�����.

	return true;
}

// �X�V�֐�.
void CGameActorManager::Update()
{
	m_DeltaTime = GetDeltaTime();

	m_ObjPosListCount = 0;		// �J�E���g�̏�����.
	m_ObjPositionList.clear();	// ���X�g�̏�����.

	// �v���C���[�̍X�V.
	m_pPlayer->Update( m_DeltaTime );			// �X�V.

	// �G���G�̍X�V.
	m_pEnemy->SetTargetPos( *m_pPlayer.get() );					// �v���C���[�̍��W���擾.
	//m_pEnemy->SetEnemyPos( *m_pEnemy.get() );
	m_pEnemy->Update( m_DeltaTime );							// �X�V.
	// �����蔻��.
	m_pEnemy->Collision( m_pPlayer.get() );
	m_pEnemy->Collision( m_pSpecialEnemy_1.get() );
	m_pEnemy->Collision( m_pSpecialEnemy_2.get() );

	// ����ȓG1�̍X�V.
	m_pSpecialEnemy_1->SetTargetPos( *m_pPlayer.get() );		// �v���C���[�̍��W���擾.
	m_pSpecialEnemy_1->Update( m_DeltaTime );					// �X�V.
	// �����蔻��.
	m_pSpecialEnemy_1->Collision( m_pPlayer.get() );
	m_pSpecialEnemy_1->Collision( m_pEnemy.get() );
	m_pSpecialEnemy_1->Collision( m_pSpecialEnemy_2.get() );

	// ����ȓG2�̍X�V.
	m_pSpecialEnemy_2->SetTargetPos( *m_pPlayer.get() );		// �v���C���[�̍��W���擾.
	m_pSpecialEnemy_2->Update( m_DeltaTime );					// �X�V.
	// �����蔻��.
	m_pSpecialEnemy_2->Collision( m_pPlayer.get() );
	m_pSpecialEnemy_2->Collision( m_pSpecialEnemy_1.get() );


	SetPositionList( m_pPlayer.get() );			// �v���C���[���W��ݒ�.
	m_ObjPositionList.shrink_to_fit();

}

// �`��֐�.
void CGameActorManager::Render()
{
	m_pPlayer->Render();			// �v���C���[�̕`��.
	m_pEnemy->Render();				// �G���G�̕`��.
	m_pSpecialEnemy_1->Render();	// ����ȓG1�̕`��.
	m_pSpecialEnemy_2->Render();	// ����ȓG2�̕`��.

}

// �X�v���C�g�̕`��.
void CGameActorManager::SpriteRender()
{
}

// ���W���X�g�̐ݒ�.
void CGameActorManager::SetPositionList( CGameObject* pObj )
{
	if ( pObj == nullptr ) return;
	m_ObjPosListCount++;	// �I�u�W�F�N�g���̉��Z.
	m_ObjPositionList.emplace_back(
		pObj->GetObjectTag(),	// �^�O�̎擾.
		// ���W�A��]�l �̎擾..
		std::pair<D3DXVECTOR3, float>( pObj->GetPosition(), pObj->GetRotation().y ));
}

