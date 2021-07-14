#ifndef ENEMY_SPAWN_STRUCT_H
#define ENEMY_SPAWN_STRUCT_H

#include "..\..\..\..\..\Utility\Tranceform\Tranceform.h"
#include "..\..\Stage\StageStruct.h"

#include <unordered_map>

// �X�|�[�����锠��͈̔�.
struct stSpawnBoxRange
{
	D3DXVECTOR2 Range;		// �͈�,
	STranceform	Tranceform;	// ���W�Ȃ�.
	stSpawnBoxRange()
		: Range			( 10.0f, 10.0f )
		, Tranceform	()
	{}
	stSpawnBoxRange( const D3DXVECTOR2& range, const STranceform& t )
		: Range			( range )
		, Tranceform	( t )
	{}
} typedef SSpawnBoxRange;

// �X�|�[�����X�g�̃}�b�v.
using spawn_list_map = std::unordered_map<EStageNo, std::vector<SSpawnBoxRange>>;

#endif	// #ifndef ENEMY_SPAWN_STRUCT_H.