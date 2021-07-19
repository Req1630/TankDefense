#ifndef CASCADED_SHADOW_MAP_H
#define CASCADED_SHADOW_MAP_H

#include "..\RenderingTextuer.h"

#include <vector>
#include <functional>

/*********************************************
*	分割シャドウマップ描画クラス.
**/
class CCascadedShadowMap : public CRenderTexture
{
	// バウンディングボックス(カスケードシャドウに使用).
	struct stBBox
	{
		D3DXVECTOR3 Min;
		D3DXVECTOR3 Max;

		stBBox( D3DXVECTOR3 min, D3DXVECTOR3 max )
			: Min ( min )
			, Max ( max )
		{}

	} typedef SBBox;

	// コンスタントバッファ.
	struct C_CBUFFER
	{
		D3DXMATRIX ShadowVP;
	};

public:
	CCascadedShadowMap();
	virtual ~CCascadedShadowMap();

	// インスタンスの取得.
	static CCascadedShadowMap* GetInstance();

	// 初期化.
	virtual HRESULT Init( ID3D11DeviceContext* pContext11 ) override;

	// 更新.
	void Update();
	// 描画.
	bool Render( const bool& isSkin, const D3DXMATRIX& mW, std::function<void()>& func );

	// 描画が終了した.
	inline void SetEndRender(){ m_IsEndRender = true; }

	// バッファの設定.
	virtual void SetBuffer() override;
	// シェーダーリソースビューの数を取得.
	virtual int GetSRVCount() override { return m_CascadedNum; }

protected:
	// テクスチャの初期化.
	virtual HRESULT InitBufferTex() override;

private:
	// 頂点シェーダーの作成.
	HRESULT InitVertexShader();

	// 平行分割処理.
	void ComputeSplitPositions(
		const int splitCount, const float lamda, 
		const float nearClip, const float farClip );
	// 分割した視錘台の8角をもとめて，ライトのビュー射影空間でAABBを求める.
	SBBox CalculateFrustum( float nearClip, float farClip, D3DXMATRIX& viewProj );
	// クロップ行列を求める.
	D3DXMATRIX CreateCropMatrix( SBBox& box );

private:
	ID3D11VertexShader*		m_pSkinVertexShader;	// 頂点シェーダー.
	ID3D11Buffer*			m_pConstantBufferFrame;	// コンスタントバッファ.
	std::vector<D3DXMATRIX>	m_ShadowMatrix;			// シャドウ用行列(カスケード分用意).
	std::vector<float>		m_SplitPos;				// カスケードの区切り座標.
	D3DXMATRIX				m_ProjMatrix;			// プロジェクション.
	D3DXMATRIX				m_ViewMatrix;			// ビュー.
	float					m_MaxClipDistance;		// 最大クリッピング距離.
	float					m_MinClipDistance;		// 最小クリッピング距離.
	int						m_CascadedNum;			// 分割数.
	bool					m_IsEndRender;

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CCascadedShadowMap( const CCascadedShadowMap & )				= delete;
	CCascadedShadowMap& operator = ( const CCascadedShadowMap & )	= delete;
	CCascadedShadowMap( CCascadedShadowMap && )						= delete;
	CCascadedShadowMap& operator = ( CCascadedShadowMap && )		= delete;
};

#endif	// #ifndef CASCADED_SHADOW_MAP_H.