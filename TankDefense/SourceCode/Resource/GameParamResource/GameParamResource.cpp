#include "GameParamResource.h"


CGameParamResource::CGameParamResource()
	: m_ParamList	()
{
}

CGameParamResource::~CGameParamResource()
{
}

//-------------------------------.
// インスタンスの取得.
//-------------------------------.
CGameParamResource* CGameParamResource::GetInstance()
{
	static std::unique_ptr<CGameParamResource> pInstance = std::make_unique<CGameParamResource>();
	return pInstance.get();
}

//-------------------------------.
// 全パラメータの読み込み.
//-------------------------------.
void CGameParamResource::ReadAllParam()
{
	GetInstance()->m_ParamList.emplace_back( ReadingParam<SPlayerParam>(EParamNo::Player) );
	GetInstance()->m_ParamList.emplace_back( ReadingParam<COutLineRender::OUTLINE_CBUFFER>(EParamNo::OutLine) );
	GetInstance()->m_ParamList.emplace_back( ReadingParam<CDownSamplingRender::SDownSamplePrame>(EParamNo::DownSamle) );
}