#include "RDModel.h"
#include <map>
#include <QString>
#include "HVector4f.h"
#include "mac_define.h"
#include "RDSpaceConvert.h"

const int n2013_02_18 = 0;
const int g_nVersion = n2013_02_18;

RDModel::RDModel()
    :m_nCount(0)
    ,m_vPos(nullptr)
    ,m_vNormal(nullptr)
    , m_vUV(nullptr)
{
}

RDModel::RDModel(int nCount)
    :m_nCount(nCount)
     ,m_hVertex(InvalidHandle)
{
    m_vPos = new float4[nCount];
    m_vNormal = new float4[nCount];
    m_vUV = new RDTexcoord[nCount];
}

RDModel::~RDModel()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->ReleaseVertexBuffer(m_hVertex);
    SAFE_DELETE_ARRAY(m_vPos);
    SAFE_DELETE_ARRAY(m_vNormal);
    SAFE_DELETE_ARRAY(m_vUV);
}

void RDModel::UpdateRenderData()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    if(m_hVertex != InvalidHandle)
    {
        pDevice->ReleaseVertexBuffer(m_hVertex);
    } 

    std::vector<RDVertexData> vertex;
    vertex.resize(3);
    vertex[0].nType = RDVB_Pos;
    vertex[0].pVertexData = (float*)m_vPos;
    vertex[0].nVertexCount = m_nCount * 4; 

    vertex[1].nType = RDVB_Normal;
    vertex[1].pVertexData = (float*)m_vNormal;
    vertex[1].nVertexCount = m_nCount * 4; 

    vertex[2].nType = RDVB_Texcoord;
    vertex[2].pVertexData = (float*)m_vUV;
    vertex[2].nVertexCount = m_nCount * 2 ;
    m_hVertex = pDevice->CreateVertexBuffer(vertex);
}

void RDModel::AddSubModel(int nStart,int nCount)
{
    RDSubModel subModel = {nStart,nCount};
    m_arSubModel.push_back(subModel);
}

bool RDModel::HitTest(float3& vHitPt, const float3& vMouse, const RDSpaceParam& param) const
{
    for(int i = 0; i < m_nCount / 3; i++)
    {
        int nIndex = i * 3;
        if(param.HitTriangle(vHitPt,vMouse,m_vPos[nIndex],m_vPos[nIndex + 1],m_vPos[nIndex + 2]))
            return true;
    }
    return false;
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
    RDModel* pModel = new RDModel(6);
    pModel->m_vPos[0].Set(-1,1,0,1);
    pModel->m_vUV[0].u = 0;
    pModel->m_vUV[0].v = 0;
    pModel->m_vPos[1].Set(1,1,0,1);
    pModel->m_vUV[1].u = 1;
    pModel->m_vUV[1].v = 0;
    pModel->m_vPos[2].Set(1,-1,0,1);
    pModel->m_vUV[2].u = 1;
    pModel->m_vUV[2].v = 1;
    pModel->m_vPos[3].Set(-1,1,0,1);
    pModel->m_vUV[3].u = 0;
    pModel->m_vUV[3].v = 0;
    pModel->m_vPos[4].Set(1,-1,0,1);
    pModel->m_vUV[4].u = 1;
    pModel->m_vUV[4].v = 1;
    pModel->m_vPos[5].Set(-1,-1,0,1);
    pModel->m_vUV[5].u = 0;
    pModel->m_vUV[5].v = 1;
    float4 vNormal(0,0,1,0);
    for(int i = 0; i < pModel->m_nCount; i++)
    {
        pModel->m_vNormal[i] = vNormal;
    }
    pModel->AddSubModel(6);
    pModel->UpdateRenderData();
    return pModel;
}
