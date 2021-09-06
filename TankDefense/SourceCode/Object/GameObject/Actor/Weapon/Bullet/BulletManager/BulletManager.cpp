#include "BulletManager.h"
#include "..\..\..\..\..\..\Utility\Input\KeyInput\KeyInput.h"
#include "..\Bullet.h"

CBulletManager::CBulletManager()
	: m_pBullet		()
{
	Init();
}

CBulletManager::~CBulletManager()
{
}

// �������֐�.
bool CBulletManager::Init()
{
	InitCollision();
	return true;
}

// �X�V�֐�.
void CBulletManager::Update( const float & deltaTime )
{
	// �e�̍X�V.
	for ( auto &b : m_pBullet ) b->Update( deltaTime );

	// �e�̍폜.
	const int EndIndex = m_pBullet.size() - 1;
	if ( EndIndex >= 0 ) {	// �v�f��0��(EndIndex = -1)�̏ꍇ�ł͂Ȃ���.
		if ( m_pBullet[EndIndex]->GetDisp() == false )
			m_pBullet.pop_back();
	}

	// �f�o�b�N�̍X�V.
	DebugUpdate();

	UpdateCollision();
}

// �`��֐�.
void CBulletManager::Render()
{
	// �e�̕`��.
	for ( auto &b : m_pBullet ) b->Render();
}

// �����蔻��֐�.
void CBulletManager::Collision( CActor* pActor )
{
	// �h���b�v�A�C�e���̓����蔻��.
	for ( auto &b : m_pBullet ) b->Collision( pActor );
}

// �����蔻��̏�����.
void CBulletManager::InitCollision()
{
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
void CBulletManager::UpdateCollision()
{
}

// ����̌����ɔ�Ԓe�̔���.
void CBulletManager::NormalShot(
	std::string StaticMeshName,
	EObjectTag	ObjTag,	SStatus&	Status,
	D3DXVECTOR3	Pos,	D3DXVECTOR3 Rot, D3DXVECTOR3 MoveVec )
{
	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto &b : m_pBullet ){
		if ( b->GetDisp() == false ){
			// �X�e�[�^�X�̐ݒ�.
			b->SetStatus( Status );

			// �e�̔���.
			b->NormalShot( StaticMeshName, ObjTag, Pos, Rot, MoveVec );
			return;
		}
	}

	// �V�����쐬.
	const int NewIndex = m_pBullet.size();
	m_pBullet.push_back( std::unique_ptr<CBullet>() );
	m_pBullet[NewIndex] = std::make_unique<CBullet>();

	// �e������.
	m_pBullet[NewIndex]->SetStatus( Status );
	m_pBullet[NewIndex]->NormalShot( StaticMeshName, ObjTag, Pos, Rot, MoveVec );
}

// �ǔ�����e�̔���.
void CBulletManager::HomingShot(
	std::string StaticMeshName,
	EObjectTag	ObjTag,		SStatus&	Status,
	D3DXVECTOR3	StartPos,	D3DXVECTOR3	EndPos,
	D3DXVECTOR3 Rot )
{
	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto &b : m_pBullet ){
		if ( b->GetDisp() == false ){
			// �X�e�[�^�X�̐ݒ�.
			b->SetStatus( Status );

			// �e�̔���.
			b->HomingShot( StaticMeshName, ObjTag, StartPos, EndPos, Rot );
			return;
		}
	}

	// �V�����쐬.
	const int NewIndex = m_pBullet.size();
	m_pBullet.push_back( std::unique_ptr<CBullet>() );
	m_pBullet[NewIndex] = std::make_unique<CBullet>();

	// �e������.
	m_pBullet[NewIndex]->SetStatus( Status );
	m_pBullet[NewIndex]->HomingShot( StaticMeshName, ObjTag, StartPos, EndPos, Rot );
}

// �f�o�b�N�̍X�V.
void CBulletManager::DebugUpdate()
{
	for ( auto &b : m_pBullet ){
		const size_t		Index	= &b - &m_pBullet[0];
		const D3DXVECTOR3	BPos	= b->GetPosition();
		CDebugText::PushTextF( "Bullet", "Bullet[%3d].Pos  : %f, %f, %f", Index, BPos.x, BPos.y, BPos.z );
		CDebugText::PushTextF( "Bullet", "Bullet[%3d].Disp : %s", Index, b->GetDisp() == true ? "true" : "false" );
	}
}
