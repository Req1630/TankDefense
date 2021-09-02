#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "..\..\..\Global.h"
#include "..\GameObjectList.h"

#include <vector>

class CGameObject;		// ゲームオブジェクトクラス.
class CPlayer;			// プレイヤークラス.
class CEnemy;			// 雑魚敵クラス.
class CSpecialEnemy_1;	// 特殊な敵1クラス.
class CSpecialEnemy_2;	// 特殊な敵2クラス.

class CGameActorManager
{
public:
	CGameActorManager();
	~CGameActorManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();
	// スプライトの描画.
	void SpriteRender();
	// オブジェクト座標リストの取得.
	inline std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	GetObjPositionList() const { return m_ObjPositionList; }

private:
	// 座標リストの設定.
	void SetPositionList(CGameObject* pObj);

private:
	float								m_DeltaTime;
	std::shared_ptr<CPlayer>			m_pPlayer;			// プレイヤー.
	std::shared_ptr<CEnemy>				m_pEnemy;			// 雑魚敵.
	std::shared_ptr<CSpecialEnemy_1>	m_pSpecialEnemy_1;	// 特殊な敵1(爆発する敵).
	std::shared_ptr<CSpecialEnemy_2>	m_pSpecialEnemy_2;	// 特殊な敵2(拘束する敵).

	std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	m_ObjPositionList;	// 各オブジェクトの座標リスト.
	int		m_ObjPosListCount;		// オブジェクトの座標リストの数.
	bool	m_IsOllAnimationStop;	// 全部のアニメーションが停止したか.
};

#endif	// #ifndef CHARACTER_MANAGER_H.