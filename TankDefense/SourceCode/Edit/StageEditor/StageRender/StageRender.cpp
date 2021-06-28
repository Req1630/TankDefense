#include "StageRender.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

namespace
{
	constexpr char	ACOTR_MESH_LIST_PATH[]			= "Data\\Parameter\\StageObjectList.txt";
	constexpr char	STAGE_OBJECT_LIST_PATH[]		= "Data\\Parameter\\StageObjectList.bin";
}

CStageRender::CStageRender()
	: m_ActorMeshList	()
	, m_ActorList		()
{
}

CStageRender::~CStageRender()
{
}

//-------------------------.
// èâä˙âª.
//-------------------------.
bool CStageRender::Init()
{
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
	return fileManager::BinaryVectorReading( STAGE_OBJECT_LIST_PATH, m_ActorList );
}

//-------------------------.
// ï`âÊ.
//-------------------------.
void CStageRender::Render()
{
	for( auto& actor : m_ActorList ){
		CDX9StaticMesh*	pStaticMesh = m_ActorMeshList[actor.ActorNo].pStaticMesh;
		pStaticMesh->SetTranceform(actor.Tranceform);
		pStaticMesh->Render();
	}
}