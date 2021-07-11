#include "LightManager.h"
#include "..\NormalLight\NormalLight.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

namespace
{
	const D3DXVECTOR4	DEBUG_MESH_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };	// メッシュの色.
	constexpr float		DEBUG_MESH_SCALE = 0.5f;						// メッシュのサイズ.
};

CLightManager::CLightManager()
	: m_pLight			( nullptr )
	, m_pNormalLight	( nullptr )
	, m_LightStateQueue	()
#ifdef _DEBUG
	, m_StaticMesh		( nullptr )
#endif // #ifdef _DEBUG.
{
	m_pNormalLight = std::make_unique<CNormalLight>();
	m_pLight = m_pNormalLight.get();
}

CLightManager::~CLightManager()
{
}

//--------------------------------------.
// インスタンスの取得.
//--------------------------------------.
CLightManager* CLightManager::GetInstance()
{
	static std::unique_ptr<CLightManager> pInstance = std::make_unique<CLightManager>();
	return pInstance.get();
}

//--------------------------------------.
// 更新関数.
//--------------------------------------.
void CLightManager::Update( const float& deltaTime )
{
	CLightManager* pInstance = GetInstance();

	if( pInstance->m_pLight == nullptr ) return;

	pInstance->m_pLight->Update( deltaTime );

	const D3DXVECTOR3 pos		= pInstance->m_pLight->GetPosition();
	const D3DXVECTOR3 lookPos	= pInstance->m_pLight->GetLookPosition();
	const D3DXVECTOR3 direction	= pInstance->m_pLight->GetDirection();
	const D3DXVECTOR3 color		= pInstance->m_pLight->GetColor();

	CDebugText::PushText( "Light", "-----------------" );
	CDebugText::PushText( "Light", "----  Light  ----" );
	CDebugText::PushText( "Light", "-----------------" );
	CDebugText::PushText( "Light", "'L + M' Is Light Activce Change" );
	CDebugText::PushText( "Light", "'P' Is Position, 'L' Is LookPosition + " );
	CDebugText::PushText( "Light", "'D or A' X Move, 'E or Q' Y Move, 'W or S' Z Move" );
	CDebugText::PushText( "Light", "-----------------" );
	CDebugText::PushText( "Light", "IsActive  : ", pInstance->m_pLight->IsActive() == true ? "true" : "false" );
	CDebugText::PushText( "Light", "Pos       : ", pos.x,		", ", pos.y,		", ", pos.z );
	CDebugText::PushText( "Light", "LookPos   : ", lookPos.x,	", ", lookPos.y,	", ", lookPos.z );
	CDebugText::PushText( "Light", "Direction : ", direction.x,	", ", direction.y,	", ", direction.z );
	CDebugText::PushText( "Light", "Color     : ", color.x,		", ", color.y,		", ", color.z );
}

// 位置の描画.
void CLightManager::PositionRender()
{
#ifdef _DEBUG
	CLightManager* pInstance = GetInstance();

	if( pInstance->m_pLight->IsActive() == false ) return;

	if( pInstance->m_StaticMesh == nullptr ){
		pInstance->m_StaticMesh = CMeshResorce::GetStatic("Sphere");
		if( pInstance->m_StaticMesh == nullptr ) return;
	}

	pInstance->m_StaticMesh->SetScale( DEBUG_MESH_SCALE );
	pInstance->m_StaticMesh->SetColor( DEBUG_MESH_COLOR );
	pInstance->m_StaticMesh->SetPosition( pInstance->m_pLight->GetPosition() );
	pInstance->m_StaticMesh->Render();
	pInstance->m_StaticMesh->SetPosition( pInstance->m_pLight->GetLookPosition() );
	pInstance->m_StaticMesh->Render();

#endif // #ifdef _DEBUG.
}