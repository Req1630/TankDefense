#include "StageRender.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

namespace
{
	constexpr char	ACOTR_MESH_LIST_PATH[]			= "Data\\Parameter\\StageObjectList.txt";
}

CStageRender::CStageRender()
	: m_pStageLoader	( std::make_unique<CStageLoader>() )
	, m_ActorMeshList	()
	, m_ActorList		()
{
}

CStageRender::~CStageRender()
{
}

//-------------------------.
// 初期化.
//-------------------------.
bool CStageRender::Init()
{
	if( m_pStageLoader->Init() == false ) return false;
	m_ActorList = m_pStageLoader->GetActorList( EStageNo_Enemy );

	const std::vector<std::string> meshNameList = fileManager::TextLoading(ACOTR_MESH_LIST_PATH);

	if( meshNameList.empty() == true ) return false;

	EActorNo actorNo = EActorNo_Begin;
	for( auto& meshName : meshNameList ){
		m_ActorMeshList.emplace_back( actorNo, meshName, CMeshResorce::GetStatic(meshName) );

		if( m_ActorMeshList[actorNo].pStaticMesh == nullptr ){
			m_ActorMeshList.clear();
			return false;
		}
		const int no = static_cast<int>(actorNo)+1;
		actorNo = static_cast<EActorNo>(no);
	}
	return true;
}

//-------------------------.
// 描画.
//-------------------------.
void CStageRender::Render()
{
	for( auto& actor : m_ActorList ){
		CDX9StaticMesh*	pStaticMesh = m_ActorMeshList[actor.ActorNo].pStaticMesh;
		pStaticMesh->SetTranceform(actor.Tranceform);
		pStaticMesh->Render();
	}
}

//-------------------------.
// ステージの設定.
//-------------------------.
void CStageRender::SetStage( const EStageNo& no )
{
	m_ActorList = m_pStageLoader->GetActorList( no );
}