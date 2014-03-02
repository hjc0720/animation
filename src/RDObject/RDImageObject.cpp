// =====================================================================================
// 
//       Filename:  RDImageObject.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/06/11 18:45:38
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDImageObject.h"
#include "RDImageResource.h"
#include "RDResourceManager.h"
#include "RDRenderData.h"
#include "mac_define.h"
#include <QRectF>
#include "RDNode.h"
#include "RDSpaceConvert.h"
#include <QDebug>
#include "RDFileDataStream.h"
#include "HVector4f.h"
#include "RDRenderDevice.h"
#include "RDModel.h"
#include "RDTexture.h"
#include "RDModelResource.h"
#include "RDCamera.h"
#include "RDMaterial.h"
#include "RDCreateObj.h"
class RDModel;

RDObjectCreator<RDImageObject,true> imageCreator;

class RDImagePrivateData: public RDRenderPrivateData
{
public:
    RDImagePrivateData(){
        m_pSegModel = nullptr;
        m_pImage = nullptr;
        m_pMaterial = nullptr;
        m_pVertexParam = nullptr;
    }
public:
    RDModelResource*            m_pSegModel;
    const RDTexture*    m_pImage; 
    RDUBO*              m_pVertexParam;
    RDMaterial*         m_pMaterial;
    matrix4x4          m_vRenderMatrix;
};

RDImageObject::RDImageObject()
{
}

RDImageObject::RDImageObject(const QString& fileName)
{
    RDResourceManager* pResManger = RDResourceManager::GetResourceManager();
    m_Image = pResManger->AddResource(fileName,RDResource_Image)->GetMd5();
}

RDImageObject::RDImageObject(const RDMd5& image)
     :m_Image(image)
{
    RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
    pResManager->AddResource(m_Image);
    m_nWidth = 0;
    m_nHeight = 0;
}


RDImageObject::~RDImageObject()
{
    RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
    pResManager->RemoveResource(&m_Image);
}


void RDImageObject::Render(unsigned long ,RDRenderData& RenderData) 
{
    RDImagePrivateData* pPrivateData = dynamic_cast<RDImagePrivateData*>( RenderData.GetPrivateData());
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    const RDModel* pModel = pPrivateData->m_pSegModel->GetModel();

    ModelVSParam param;
    param.MVP = pPrivateData->m_vRenderMatrix * RenderData.GetMVPMatrix();
    param.ModelMat = pPrivateData->m_vRenderMatrix * RenderData.GetGlobalMatrix();
    param.NormalMat = param.ModelMat.toNormalMat();
    if(pPrivateData->m_pVertexParam)
        pDevice->ModifyUniformBufferObject(pPrivateData->m_pVertexParam,reinterpret_cast<float*>(&param));
    else
        pPrivateData->m_pVertexParam = pDevice->CreateUniformBufferObject(sizeof(ModelVSParam) / sizeof(float),reinterpret_cast<float*>(&param));
    pDevice->SetShaderParam(0,pPrivateData->m_pVertexParam);

    pPrivateData->m_pMaterial->SetParamToDevice();
    for(size_t i = 0; i <  pModel->GetSubsetCount(); i++)
        pModel->DrawSubset(i);
    qDebug() << "image render";
    return;
}

void RDImageObject::CalFrame(const RDTime& time,RDRenderData& RenderData) 
{
    RDImagePrivateData* pPrivateData = dynamic_cast<RDImagePrivateData*>( RenderData.GetPrivateData());
    if(RenderData.GetChangeLevel() >= RDRender_TransChange)
    {
        pPrivateData->m_vRenderMatrix = matrix4x4(m_nWidth / 2,m_nHeight / 2,1,HMatrixQ4F_Scale);
    }
    pPrivateData->m_pMaterial->UpdateFrame(time,0,0,0);
}

bool RDImageObject::HitTest(const float3& vScenePt,const RDNode& pNode,const QString& RDName) const
{
    const RDRenderData* pRenderData = pNode.GetRenderData(RDName);
    const RDImagePrivateData* pPrivateData = dynamic_cast<const RDImagePrivateData*>( pRenderData->GetPrivateData());
    matrix4x4 worldMat = pPrivateData->m_vRenderMatrix * pRenderData->GetGlobalMatrix();
    RDSpaceParam param(pNode.GetEditSpaceParam(RDName,&worldMat));
    const RDModel* pModel = pPrivateData->m_pSegModel->GetModel();
    float3 vHitPt;

    return pModel->HitTest(vHitPt,vScenePt,param);
}

const RDMd5& RDImageObject::GetObjMd5()const 
{
    static RDMd5 ImageMd5("RDImageObject", -1);
    return ImageMd5;
}

void RDImageObject::UpdateBound(const RDTime& ,RDRenderData& RenderData) 
{
    float3 bufferPos;
    RDSceneToBuffer(bufferPos,RenderData.GetPos() ,-(float)RenderData.GetSceneWidth()/ 2,RenderData.GetSceneHeight()/ 2);
    RenderData.SetBound( QRectF(bufferPos.x(),bufferPos.y(),m_nWidth,m_nHeight));

    RenderData.SetMin(float3(-m_nWidth / 2,-m_nHeight / 2,0));
    RenderData.SetMax(float3(m_nWidth / 2,m_nHeight / 2,0));
}

void RDImageObject::CreateRenderData(RDRenderData& pRD)
{
    RDImagePrivateData* pPrivateData = dynamic_cast<RDImagePrivateData*>( pRD.GetPrivateData());
    if(!pPrivateData)
    {
        pPrivateData = new RDImagePrivateData;
        pRD.SetPrivateData(pPrivateData);
    }
    RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
    pPrivateData->m_pSegModel = dynamic_cast<RDModelResource*>(pResManager->AddModelResource("segment"));
    RDImageResource* pResource = dynamic_cast<RDImageResource*>(pResManager->GetResource(m_Image));
    pPrivateData->m_pImage = pResource->GetBuffer();

    pPrivateData->m_pMaterial = new RDMaterial(true,0xffffffff);
    QRectF bound(0,0,pPrivateData->m_pImage->GetWidth(),pPrivateData->m_pImage->GetHeight());
    pPrivateData->m_pMaterial->AddTex(RDNormalMatTexture,pPrivateData->m_pImage,bound);
}

void RDImageObject::Serialize(RDFileDataStream& buffer,bool bSave)
{
    int nVersion = 0;
    buffer.Serialize(nVersion,bSave);
	RDObject::Serialize(buffer,bSave);
    buffer.Serialize(m_Image,bSave);
    if(bSave)
    {
        RDImageResource* pResource = dynamic_cast<RDImageResource*>(RDResourceManager::GetResourceManager()->GetResource(m_Image));
        buffer.SaveResource(pResource->GetPath());
    }
    else
    {
        RDResourceManager::GetResourceManager()->AddResource(m_Image);
    }
    buffer.Serialize(m_nWidth,bSave);
    buffer.Serialize(m_nHeight,bSave);
}
