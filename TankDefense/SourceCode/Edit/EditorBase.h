#ifndef EDITOR_BASE_H
#define EDITOR_BASE_H

#include "..\Global.h"
#include "..\Utility\ImGuiManager\ImGuiManager.h"

class CEditPlayer;

/**********************************
*	エディタ基底クラス.
**/
class CEditorBase
{
public:
	CEditorBase( )
		: m_MessageText			( "" )
		, m_MessageRenderCount	( 0.0f )
		, m_IsImGuiGamepad		( true )
	{}
	virtual ~CEditorBase()
	{}

	// 初期化.
	virtual bool Init() = 0;
	// 更新.
	virtual void Update() = 0;
	// ImGui描画.
	virtual bool ImGuiRender() = 0;
	// モデル描画.
	virtual void ModelRender() = 0;
	// エフェクト描画.
	virtual void EffectRneder() = 0;
	// ウィジェット描画.
	virtual void WidgetRender() = 0;

	// エディタ用プレイヤーの設定.
	virtual void SetEditPlayer( CEditPlayer* pPlayer ){}

	// ImGuiでゲームパッド操作を有効にする.
	void OnImGuiGamepad();
	// ImGuiでゲームパッド操作を無効にする.
	void OffImGuiGamepad();

protected:
	// ImGuiタブの開始.
	bool BeginTab( const char* tabName );
	// ImGuiタブの終了.
	void EndTab();

	// 保存ボタン.
	void SaveButton( const char* filePath );
	// 読込ボタン.
	void LoadButton( const char* filePath );
	// メッセージの描画.
	void MessageRender();

	// パラメータの書き込み.
	virtual void ParameterWriting( const char* filePath ) = 0;
	// パラメータの読み込み.
	virtual void ParameterLoading( const char* filePath ) = 0;

	// パラメータの書き込みメッセージの設定.
	bool SetParameterWritingMsg( const bool& isSucceeded );
	// パラメータの読み込みメッセージの設定.
	bool SetParameterLoadingMsg( const bool& isSucceeded );

protected:
	std::string		m_MessageText;			// メッセージテキスト.
	float			m_MessageRenderCount;	// メッセージ描画カウント.
	bool			m_IsImGuiGamepad;		// コントローラーが

};

#endif	// #ifndef EDITOR_BASE_H.