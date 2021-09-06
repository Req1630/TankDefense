#include "DropItemManager.h"
#include "..\DropMachineGun\DropMachineGun.h"
#include "..\DropMissile\DropMissile.h"
#include "..\DropBeam\DropBeam.h"
#include "..\..\Player\Player.h"
#include "..\..\..\..\.\..\Utility\Input\Input.h"
#include <random>

CDropItemManager::CDropItemManager()
	: CDropItemManager( nullptr )
{
}

CDropItemManager::CDropItemManager( const std::shared_ptr<CPlayer> pPlayer )
	: m_DropItem	()
	, m_pPlayer		( pPlayer )
{
}

CDropItemManager::~CDropItemManager()
{
}

// �������֐�.
bool CDropItemManager::Init()
{
	InitCollision();
	return true;
}

// �X�V�֐�.
void CDropItemManager::Update( const float& deltaTime )
{
	// �h���b�v�A�C�e���̍X�V.
	for ( auto &i : m_DropItem ) i.pItem->Update( deltaTime );

	// �h���b�v�A�C�e���̍폜.
	const int EndIndex = m_DropItem.size() - 1;
	if ( EndIndex >= 0 ){	// �v�f��0��(EndIndex = -1)�̏ꍇ�ł͂Ȃ���.
		if ( m_DropItem[EndIndex].pItem->GetDisp() == false )
			m_DropItem.pop_back();
	}

	// �f�o�b�N�̍X�V.
	DebugUpdate();

	UpdateCollision();
}

// �`��֐�.
void CDropItemManager::Render()
{
	// �h���b�v�A�C�e���̕`��.
	for ( auto &i : m_DropItem ) i.pItem->Render();
}

// Sprite3D�`��֐�.
void CDropItemManager::Sprite3DRender()
{
	// �A�C�e�������r���{�[�h�ŕ\��.
	for ( auto &i : m_DropItem ) i.pItem->Sprite3DRender();
}

// �����蔻��֐�.
void CDropItemManager::Collision( CActor* pActor )
{
	// �h���b�v�A�C�e���̓����蔻��.
	for ( auto &i : m_DropItem ) i.pItem->Collision( pActor );
}

// �����蔻��̏�����.
void CDropItemManager::InitCollision()
{
}

// �����蔻��̍��W��A���a�Ȃǂ̍X�V.
void CDropItemManager::UpdateCollision()
{
}

// �w�肵������𗎂Ƃ��֐�.
void CDropItemManager::Drop( const DropItem::EList& DropItem, const D3DXVECTOR3& DropPos )
{
	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto &i : m_DropItem ){
		if ( i.pItem->GetDisp() == false ){
			// �\���A�C�e�����Ⴄ�ꍇ�ύX����.
			if ( i.Tag != DropItem ) {
				i = SDropItem( CreateDropItem( DropItem ), DropItem );
				i.pItem->SetPlayer( m_pPlayer );
			}

			// �A�C�e�����h���b�v.
			i.pItem->Drop( DropPos );

			return;
		}
	}

	// �V�����쐬.
	const int NewIndex = m_DropItem.size();
	m_DropItem.push_back( SDropItem( CreateDropItem( DropItem ), DropItem ) );
	m_DropItem[NewIndex].pItem->SetPlayer( m_pPlayer );

	// �A�C�e�����h���b�v.
	m_DropItem[NewIndex].pItem->Drop( DropPos );
}

// �����_���ȕ���𗎂Ƃ��֐�.
void CDropItemManager::RandomDrop( const D3DXVECTOR3& DropPos, const DropItem::SProb& Prob  )
{
	// �����_���̐ݒ�.
	std::random_device	rnd;
	std::mt19937		mt( rnd() );
	std::uniform_int_distribution<>	Rand_Num( 1, 100 );
	DropItem::EList		RandItem;

	// �w�肵���m���̌��ʂ�Ԃ�.
	const int MissileProb = Prob.MachineGun + Prob.Missile;
	if		( Rand_Num( mt ) <= Prob.MachineGun )	RandItem = DropItem::EList::MachineGun;
	else if ( Rand_Num( mt ) <= MissileProb		)	RandItem = DropItem::EList::Missile;
	else											RandItem = DropItem::EList::Beam;

	// �h���b�v������.
	Drop( RandItem, DropPos );
}

// �h���b�v�A�C�e���̍쐬�֐�(�t�@�N�g���[���\�b�h).
std::unique_ptr<CDropItem> CDropItemManager::CreateDropItem( const DropItem::EList& DropItem )
{
	// �Ή������N���X��Ԃ�.
	switch ( DropItem ) {
	default:
	case DropItem::EList::MachineGun:	return std::make_unique<CDropMachineGun>();
	case DropItem::EList::Missile:		return std::make_unique<CDropMissile>();
	case DropItem::EList::Beam:			return std::make_unique<CDropBeam>();
	}
}

// �f�o�b�N�̍X�V�֐�.
void CDropItemManager::DebugUpdate()
{
	CDebugText::PushText( "DropItem", "--------------------" );
	CDebugText::PushText( "DropItem", "----  DropItem  ----" );
	CDebugText::PushText( "DropItem", "--------------------" );
	CDebugText::PushText( "DropItem", "L_Ctrl + " );
	CDebugText::PushText( "DropItem", "'R' RandomItemDrop" );
	CDebugText::PushText( "DropItem", "--------------------" );

	for ( auto &i : m_DropItem ){
		const size_t		Index = &i - &m_DropItem[0];
		const D3DXVECTOR3	IPos = i.pItem->GetPosition();
		CDebugText::PushTextF( "DropItem", "DropItem[%3d].Pos  : %f, %f, %f", Index, IPos.x, IPos.y, IPos.z );
		CDebugText::PushTextF( "DropItem", "DropItem[%3d].Disp : %s", Index, i.pItem->GetDisp() == true ? "true" : "false" );
		CDebugText::PushTextF( "DropItem", "DropItem[%3d].Tag  : %s", Index, i.Tag == DropItem::EList::MachineGun ? "MachineGun" : i.Tag == DropItem::EList::Missile ? "Missile" : "Beam" );
	}

	if ( CKeyInput::IsPress( VK_LCONTROL ) == false ) return;
	const D3DXVECTOR3 PPos		= m_pPlayer->GetPosition();
	const D3DXVECTOR3 DropPos	= { PPos.x, PPos.y, PPos.z - 10.0f };
	if ( CKeyInput::IsMomentPress( 'R' ) )
		RandomDrop( DropPos, DropItem::SProb( 50, 30 ) );

	// �v���C���[�̕���ύX�p.
	if ( CKeyInput::IsMomentPress( '2' ) ) Drop( DropItem::EList::MachineGun, PPos );
	if ( CKeyInput::IsMomentPress( '3' ) ) Drop( DropItem::EList::Missile, PPos );
	if ( CKeyInput::IsMomentPress( '4' ) ) Drop( DropItem::EList::Beam, PPos );
}