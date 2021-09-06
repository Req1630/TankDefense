#include "GameActorManager.h"
#include "..\Actor\Player\Player.h"
#include "..\Actor\Weapon\Bullet\BulletManager\BulletManager.h"
#include "..\Actor\EnemyBase\Enemy\Enemy.h"
#include "..\Actor\EnemyBase\SpecialEnemy_1\SpecialEnemy_1.h"
#include "..\Actor\EnemyBase\SpecialEnemy_2\SpecialEnemy_2.h"
#include "..\Actor\Boss\Boss.h"
#include "..\Actor\DropItem\DropItemManager\DropItemManager.h"

CGameActorManager::CGameActorManager()
	: m_pPlayer				( nullptr )
	, m_pBulletMng			( nullptr )
	, m_pEnemy				( nullptr )
	, m_pSpecialEnemy_1		( nullptr )
	, m_pSpecialEnemy_2		( nullptr )
	, m_pBoss				( nullptr )
	, m_pDropItemMng		( nullptr )
{
	m_pBulletMng		= std::make_shared<CBulletManager>();
	m_pPlayer			= std::make_shared<CPlayer>( m_pBulletMng );
	m_pEnemy			= std::make_shared<CEnemy>();
	m_pSpecialEnemy_1	= std::make_shared<CSpecialEnemy_1>();
	m_pSpecialEnemy_2	= std::make_shared<CSpecialEnemy_2>();
	m_pBoss				= std::make_shared<CBoss>();
	m_pDropItemMng		= std::make_shared<CDropItemManager>( m_pPlayer );
}

CGameActorManager::~CGameActorManager()
{
}

// �������֐�.
bool CGameActorManager::Init()
{
	if ( m_pBulletMng->Init()		== false ) return false;	// �e�̏�����.
	if ( m_pPlayer->Init()			== false ) return false;	// �v���C���[�̏�����.
	if ( m_pEnemy->Init()			== false ) return false;	// �G���G�̏�����.
	if ( m_pSpecialEnemy_1->Init()	== false ) return false;	// ����ȓG1�̏�����.
	if ( m_pSpecialEnemy_2->Init()	== false ) return false;	// ����ȓG2�̏�����.
	if ( m_pBoss->Init()			== false ) return false;	// �{�X�̏�����.
	if ( m_pDropItemMng->Init()		== false ) return false;	// �h���b�v�A�C�e���̏�����.

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

	// �{�X�̍X�V.
	m_pBoss->SetTargetPos( *m_pPlayer.get() );					// �v���C���[�̍��W���擾.
	m_pBoss->Update( m_DeltaTime );								// �X�V.
	// �����蔻��.
	m_pBoss->Collision( m_pPlayer.get() );
	m_pBoss->Collision( m_pEnemy.get() );
	m_pBoss->Collision( m_pSpecialEnemy_1.get() );
	m_pBoss->Collision( m_pSpecialEnemy_2.get() );

	// �e�̍X�V.
	m_pBulletMng->Update( m_DeltaTime );		// �X�V.

	SetPositionList( m_pPlayer.get() );			// �v���C���[���W��ݒ�.
	m_ObjPositionList.shrink_to_fit();

	// �h���b�v�A�C�e���̍X�V.
	m_pDropItemMng->Update( m_DeltaTime );
	m_pDropItemMng->Collision( m_pPlayer.get() );
}

// �`��֐�.
void CGameActorManager::Render()
{
	m_pPlayer->Render();			// �v���C���[�̕`��.
	m_pEnemy->Render();				// �G���G�̕`��.
	m_pSpecialEnemy_1->Render();	// ����ȓG1�̕`��.
	m_pBoss->Render();				// �{�X�̕`��.
	m_pSpecialEnemy_2->Render();	// ����ȓG2�̕`��.
	m_pBulletMng->Render();			// �e�̕`��.

	m_pDropItemMng->Render();		// �h���b�v�A�C�e���̕`��.
}

// Sprite3D�`��֐�.
void CGameActorManager::Sprite3DRender()
{
	m_pPlayer->Sprite3DRender();			// �v���C���[�̕`��.
	m_pEnemy->Sprite3DRender();				// �G���G�̕`��.
	m_pSpecialEnemy_1->Sprite3DRender();	// ����ȓG1�̕`��.
	m_pBoss->Sprite3DRender();				// �{�X�̕`��.
	m_pSpecialEnemy_2->Sprite3DRender();	// ����ȓG2�̕`��.
	m_pBulletMng->Sprite3DRender();			// �e�̕`��.

	m_pDropItemMng->Sprite3DRender();
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

