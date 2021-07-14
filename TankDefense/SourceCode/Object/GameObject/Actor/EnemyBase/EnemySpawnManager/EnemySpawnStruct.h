#ifndef ENEMY_SPAWN_STRUCT_H
#define ENEMY_SPAWN_STRUCT_H

#include "..\..\..\..\..\Utility\Tranceform\Tranceform.h"
#include "..\..\Stage\StageStruct.h"

#include <unordered_map>

// スポーンする箱状の範囲.
struct stSpawnBoxRange
{
	D3DXVECTOR2 Range;		// 範囲,
	STranceform	Tranceform;	// 座標など.
	stSpawnBoxRange()
		: Range			( 10.0f, 10.0f )
		, Tranceform	()
	{}
	stSpawnBoxRange( const D3DXVECTOR2& range, const STranceform& t )
		: Range			( range )
		, Tranceform	( t )
	{}
} typedef SSpawnBoxRange;

// スポーンリストのマップ.
using spawn_list_map = std::unordered_map<EStageNo, std::vector<SSpawnBoxRange>>;

#endif	// #ifndef ENEMY_SPAWN_STRUCT_H.