/**
* @file SceneList.h.
* @brief 各シーンをまとめったヘッダー.
* @author 福田玲也.
*/
#ifndef SCENE_LIST_H
#define SCENE_LIST_H

/****************************************
*	各シーンのヘッダーをまとめるヘッダー.
*
*/

#include "..\SceneManager\SceneManager.h"
#include "Title/Title.h"
#include "Game/Game.h"
#include "Edit/Edit.h"

// シーンの種類.
enum class enSceneNo : unsigned char
{
	None,

	Title,		// タイトル.
	GameMain,	// ゲームメイン.

	Edit,
	EnemyBattle,// 雑魚戦メイン.
	BossBattle,	// ボス戦メイン.
	GameClear,	// ゲームクリア.
	GameOver,	// ゲームオーバー.
	Ending,		// エンディング.

	Max,

	ResultCheck,		// ゲームクリアかゲームオーバーの確認.
	Start = Title,		// 初めのシーン.
};

#endif	// #ifndef SCENE_LIST_H.