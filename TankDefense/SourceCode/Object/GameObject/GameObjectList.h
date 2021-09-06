/**
* @file GameObjctList.h.
* @brief ゲーム内で出てくるオブジェクトリストをまとめたヘッダー.
* @author 福田玲也.
*/
#ifndef GAME_OBJECT_LIST_H
#define GAME_OBJECT_LIST_H

// オブジェクトタグリスト.
enum class enObjectTagList : unsigned int
{
	None,

	Player,			// プレイヤー.
	Enemy,			// 雑魚敵.
	SpecialEnemy_1,	// 特殊な敵1.
	SpecialEnemy_2, // 特殊な敵2.
	Boss,			// ボス.
	PlayerBullet,	// プレイヤーの弾.
	EnemyBullet,	// 敵の弾.
	BossBullet,		// ボスの弾.
	DropItem,		// ドロップアイテム.
	DefaultWepon,	// 主砲.
	MachineGun,		// マシンガン.
	Missile,		// ミサイル.
	Beam,			// ビーム砲.

	Max,

} typedef EObjectTag;

#endif	// #ifndef GAME_OBJECT_LIST_H.
