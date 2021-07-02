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

	Max,

} typedef EObjectTag;

#endif	// #ifndef GAME_OBJECT_LIST_H.
