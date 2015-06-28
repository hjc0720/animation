#include "RDModel.h"
#include <map>
#include <QString>
#include "HVector4f.h"
#include "mac_define.h"
#include "RDSpaceConvert.h"
#include <cfloat>

RDModel::RDModel()
    :m_pVertexShader(nullptr)
     ,m_hVertex(InvalidHandle)
{
}

RDModel::~RDModel()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->ReleaseShader(m_pVertexShader);
    pDevice->ReleaseVertexBuffer(m_hVertex);
}

void RDModel::UpdateRenderData()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    if(m_hVertex != InvalidHandle)
    {
        pDevice->ReleaseVertexBuffer(m_hVertex);
    } 

    m_hVertex = pDevice->CreateVertexBuffer((float*)&m_vVertex[0],m_vVertex.size(),sizeof(ModelVertexPt),RDVB_Pos_Normal_Texcoord);
    m_pVertexShader = pDevice->CreateShader(":/shader/model_vs",VertexShader);
}

void RDModel::AddSubModel(int nStart,int nCount)
{
    RDSubModel subModel = {nStart,nCount};
    m_arSubModel.push_back(subModel);
}

float RDModel::HitTest(const float3& vMouse, const RDSpaceParam& param) const
{
	float3 vHitPt;
	float fDistance = FLT_MAX;
	bool bHit = false;
    for(size_t i = 0; i < m_vVertex.size() / 3; i++)
    {
        int nIndex = i * 3;
        if(param.HitTriangle(vHitPt,vMouse,m_vVertex[nIndex].m_vPos,m_vVertex[nIndex + 1].m_vPos,m_vVertex[nIndex + 2].m_vPos))
		{
			fDistance = std::min(fDistance,param.ConvertWorldToView(vHitPt).z());
			bHit = true;
		}
    }
		return bHit ? fDistance : -1;
}

void RDModel::AddSubModel(int nCount)
{
    int nStart = 0;
    for(size_t i = 0; i < m_arSubModel.size();i++)
    {
        nStart += m_arSubModel[i].nCount;
    }
    AddSubModel(nStart,nCount);
}

void RDModel::DrawSubset(size_t nSubset) const
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    const RDSubModel& subModel = m_arSubModel[nSubset];
    pDevice->SetShader(m_pVertexShader,VertexShader);
    pDevice->SetVertexBuffer(m_hVertex);
    pDevice->Render(GL_TRIANGLES,subModel.nStart,subModel.nCount );
}

////////////////////////////////////////////////////////////////////////////////
//static function
RDModel* RDModel::CreateModel(RDModelType nType)
{
    switch(nType)
    {
    case RDSegmentModel:
        return CreateSegmentModel();
    }
    return nullptr;
}

RDModel* RDModel::CreateSegmentModel()
{
    RDModel* pModel = new RDModel();
	pModel->m_vVertex.resize(6);
    
    pModel->m_vVertex[0].m_vPos.Set(-1,1,0,1);
    pModel->m_vVertex[0].m_vUV.x = 0;
    pModel->m_vVertex[0].m_vUV.y = 1;
    pModel->m_vVertex[1].m_vPos.Set(1,1,0,1);
    pModel->m_vVertex[1].m_vUV.x = 1;
    pModel->m_vVertex[1].m_vUV.y = 1;
    pModel->m_vVertex[2].m_vPos.Set(1,-1,0,1);
    pModel->m_vVertex[2].m_vUV.x = 1;
    pModel->m_vVertex[2].m_vUV.y = 0;
    pModel->m_vVertex[3].m_vPos.Set(-1,1,0,1);
    pModel->m_vVertex[3].m_vUV.x = 0;
    pModel->m_vVertex[3].m_vUV.y = 1;
    pModel->m_vVertex[4].m_vPos.Set(1,-1,0,1);
    pModel->m_vVertex[4].m_vUV.x = 1;
    pModel->m_vVertex[4].m_vUV.y = 0;
    pModel->m_vVertex[5].m_vPos.Set(-1,-1,0,1);
    pModel->m_vVertex[5].m_vUV.x = 0;
    pModel->m_vVertex[5].m_vUV.y = 0;
    float4 vNormal(0,0,-1,0);
    for(size_t i = 0; i < pModel->m_vVertex.size(); i++)
    {
        pModel->m_vVertex[i].m_vNormal = vNormal;
    }
    pModel->AddSubModel(6);
    pModel->UpdateRenderData();
    return pModel;
}
