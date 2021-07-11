#include "SpriteResource.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\DebugConsole\DebugConsole.h"
#include "..\..\Common\DebugText\DebugText.h"

CSpriteResource::CSpriteResource()
	: m_SpriteList			()
	, m_HasFinishedLoading	( false )
{
}

CSpriteResource::~CSpriteResource()
{
	m_SpriteList.clear();
}

//-------------------------------.
// インスタンスの取得.
//-------------------------------.
CSpriteResource* CSpriteResource::GetInstance()
{
	static std::unique_ptr<CSpriteResource> pInstance = 
		std::make_unique<CSpriteResource>();
	return pInstance.get();
}

//-------------------------------.
// スプライトの読み込み(ラッパー).
//-------------------------------.
HRESULT CSpriteResource::Load( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( GetInstance()->SpriteLoad( pContext11 ) )) return E_FAIL;
	return S_OK;
}

//-------------------------------.
// スプライト取得関数.
//-------------------------------.
CSprite* CSpriteResource::GetSprite( const std::string& spriteName )
{
	// 指定したスプライト名を取得する.
	for( auto& s : GetInstance()->m_SpriteList ){
		if( s.first == spriteName ) return s.second.get();
	}

	// スプライトの読込が終了してないので null を返す.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( spriteName + " が存在しません" );
	return nullptr;
}

//-------------------------------.
// スプライト名リスト取得関数.
//-------------------------------.
std::vector<std::string> CSpriteResource::GetSpriteNameList()
{
	std::vector<std::string> nameList;
	// 指定したスプライト名を取得する.
	for( auto& s : GetInstance()->m_SpriteList ){
		nameList.emplace_back( s.first );
	}
	return nameList;
}

//-------------------------------.
// スプライト読み込み.
//-------------------------------.
HRESULT CSpriteResource::SpriteLoad( ID3D11DeviceContext* pContext11 )
{
	std::unique_lock<std::mutex> lock( m_Mutex );
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// 拡張子.
		const std::string filePath	= entry.path().string();				// ファイルパス.
		const std::string fileName	= entry.path().stem().string();			// ファイル名.
		const std::string errorMsg	= fileName+" : スプライト読み込み失敗";	// あらかじめエラーメッセージを設定する.

		// "_"が入ったものは無視される.
		if( filePath.find("_") != std::string::npos ) return;
		// 指定した拡張子以外は読み込い.
		if( exe != ".png" && exe != ".PNG" && 
			exe != ".bmp" && exe != ".BMP" &&
			exe != ".jpg" && exe != ".JPG" ) return;

		// スプライトの作成.
		m_SpriteList[fileName] = std::make_unique<CSprite>();
		if( FAILED( m_SpriteList[fileName]->Init( pContext11, filePath.c_str() ) ))
			throw errorMsg;
	};

	CLog::Print( "------ 画像読み込み開始 -------" );
	CDebugText::PushLog( "Sprite Load Begin" );

	try {
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );
	} catch ( const fs::filesystem_error& e ){
		// エラーメッセージを表示.
		ERROR_MESSAGE( e.path1().string().c_str() );
		return E_FAIL;
	} catch( const std::string& e ){
		// エラーメッセージを表示.
		ERROR_MESSAGE( e );
		return E_FAIL;
	}

	CDebugText::PushLog( "Sprite Load End" );
	CLog::Print( "------ 画像読み込み終了 -------" );

	// 読込が終わったので true にする.
	m_HasFinishedLoading = true;
	return S_OK;
}