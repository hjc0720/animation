/*
 * =====================================================================================
 *
 *       Filename:  RDLayer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/02/13 20:40:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "RDLayer.h"
#include "RDCamera.h"
#include <stack>
#include <cfloat>
#include "RDSpaceConvert.h"
#include "RDScene.h"
#include "RDSceneRenderData.h"
#include "RDCreateObj.h"
#include "RDFileDataStream.h"
#include "RDLight.h"
#include <algorithm>
#include "RDRenderDevice.h"
#include "RDBox.h"

RDObjectCreator<RDLayer,false> LayerCreator;

class RDLayerRenderData :public RDRenderData
{
    friend RDLayer;
public:
    RDLayerRenderData(RDNode& node,const RDSceneRenderData& SceneRenderData)
        :RDRenderData(node,SceneRenderData)
         ,m_pLightParam(nullptr)
         ,m_nCurCameraID(0)
    { 
        ClearLightCount();
    }
    size_t GetCurCameraID()const{return m_nCurCameraID;}
    void    SetCurCameraID(size_t nIndex){m_nCurCameraID = nIndex;}
    void    ClearLightCount(){memset(m_nTypeLightCount,0,sizeof(m_nTypeLightCount));}
protected:
    RDUBO* m_pLightParam;
    size_t m_nCurCameraID;
    size_t m_nTypeLightCount[RDLightTypeCount]; 
};

RDLayer::RDLayer(RDLayerType nType,const QString& strName)
    :RDNode(strName)
     ,m_nType(nType)
{
     m_strName = strName;
     RDCamera* pCamera = new RDCamera("camera",1080,PerspectiveProject);
     pCamera->SetParent(this);
     m_vecCameraObj.push_back(pCamera);

     RDLight* pLight = new RDLight("light",RDPointLight,float3(0,100,-100));
     pLight->SetParent(this);
     m_vecLight.push_back(pLight);
}

RDNode* RDLayer::GetChild(size_t i)
{
    if(i < m_vecCameraObj.size())
        return GetCamera(i);
    else if(i - m_vecCameraObj.size() < GetLightCount())
        return GetLight(i - m_vecCameraObj.size());
    return RDNode::GetChild(i - m_vecCameraObj.size() - m_vecLight.size());
}

const RDNode* RDLayer::GetChild(size_t i)const
{
    if(i < m_vecCameraObj.size())
        return GetCamera(i);
    else if(i - m_vecCameraObj.size() < GetLightCount())
        return GetLight(i - m_vecCameraObj.size());
    return RDNode::GetChild(i - m_vecCameraObj.size() - m_vecLight.size());
}

RDNode* RDLayer::RemoveChild(size_t i)
{
    if(i < GetCameraCount())
        return RemoveCamera(i);
    else if(i - m_vecCameraObj.size() < GetLightCount())
        return RemoveLight(i - m_vecCameraObj.size());
    return RDNode::RemoveChild(i - m_vecCameraObj.size() - m_vecLight.size());
} 

RDCamera* RDLayer::RemoveCamera(size_t i)
{
    RDCamera* pRemove = m_vecCameraObj[i];
    m_vecCameraObj.erase(m_vecCameraObj.begin() + i);
    return pRemove;
}

float2      RDLayer::CalObjMinMax(const QString& pRDName)
{
    std::stack<RDNode*> nodeSt;

    float3 vMin(FLT_MAX,FLT_MAX,FLT_MAX),vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    RDLayerRenderData* pLayerRD = dynamic_cast<RDLayerRenderData*>(GetRenderData(pRDName));
    float2 vNearFar;
    vNearFar.x = -FLT_MAX;
    vNearFar.y = FLT_MAX;

    size_t nCount = GetChildCount();
    for(size_t i = GetCameraCount() + GetLightCount();i < nCount;i++)
    {
        nodeSt.push(GetChild(i));
    }
    RDCamera* pCamera = GetCurCamera(*pLayerRD);
    if(nodeSt.empty())
    {
        vNearFar.x = -100;
        vNearFar.y = -10000;
        return vNearFar;
    }
    RDBox cameraBox;
    while(!nodeSt.empty())
    {
        RDNode* pNode = nodeSt.top();
        RDRenderData* pRD = pNode->GetRenderData(pRDName);
        size_t nChildCount = pNode->GetChildCount();
        for(size_t i = 0;i < nChildCount;i++)
        {
            nodeSt.push(pNode->GetChild(i));
        }
        cameraBox.UnionBox(RDBox(pRD->GetMin(),pRD->GetMax()),
                pRD->GetGlobalMatrix() * pCamera->GetViewMatrix(pRDName));
        nodeSt.pop();
    }
    vNearFar.x = min(-0.001,cameraBox.GetMax().z() + 0.1);
    vNearFar.y = max(vNearFar.x * 10e7,cameraBox.GetMin().z() - 0.1);
    if(vNearFar.y >= vNearFar.x)
        vNearFar.y = vNearFar.x + 100;
    return vNearFar;
}

RDRenderData *RDLayer::CreateRenderData(const QString &pName)
{
    QMutexLocker locker(&m_lock);
    const RDScene* pScene = GetSceneNode();
    RDRenderData* pRenderData = new RDLayerRenderData(*this,*dynamic_cast<const RDSceneRenderData*>(pScene->GetRenderData(pName)));
    m_vecRenderData[pName] = pRenderData;
    return pRenderData;
}

void    RDLayer::CalChildFrame(const RDTime& nTime,const QString& pRDName)
{
    RDLayerRenderData* pLayerRD = dynamic_cast<RDLayerRenderData*>(GetRenderData(pRDName));
    std::for_each(m_vecCameraObj.begin(),m_vecCameraObj.end(),
            [&](RDCamera* pCamera){pCamera->CalFrame(nTime,pRDName);});

    pLayerRD->ClearLightCount();
    std::for_each(m_vecLight.begin(),m_vecLight.end(),
            [&](RDLight* pLight){
            pLight->CalFrame(nTime,pRDName);
            if(pLight->GetCurSection(pRDName))
                pLayerRD->m_nTypeLightCount[pLight->GetLightType()]++;
            });

    std::for_each(m_vecChildObj.begin(),m_vecChildObj.end(),
            [&](RDNode* pChild){pChild->CalFrame(nTime,pRDName);});

    float2 vNearFar = CalObjMinMax(pRDName);

    QRectF sceneRT(-(pLayerRD->GetSceneWidth() / 2.f),-(pLayerRD->GetSceneHeight() / 2.f),pLayerRD->GetSceneWidth(),pLayerRD->GetSceneHeight());
    RDCamera* pCamera = GetCurCamera(*pLayerRD);
    pCamera->UpdateProject(pRDName,sceneRT,vNearFar.x,vNearFar.y);

    if(pCamera->GetRenderChangeLevel(pRDName) > RDRender_NoChange)
        pLayerRD->UnionDirty(sceneRT);

    std::vector<RDLight*> arLight(m_vecLight);
    std::sort(arLight.begin(),arLight.end(),
            [](RDLight* l1,RDLight* l2){return l1->GetLightType() < l2->GetLightType();});

    size_t nLightSize = sizeof(float4);
    std::for_each(arLight.begin(),arLight.end(),
            [&](RDLight* pLight){nLightSize += pLight->GenerateShaderParam(nullptr,pRDName);});

    char* pBuffer = new char[nLightSize]; 
    char* pTmp = pBuffer;
    RDSaveData(pTmp,pCamera->GetDynamicPos(pRDName));
    std::for_each(arLight.begin(),arLight.end(),
            [&](RDLight* pLight){pLight->GenerateShaderParam(pTmp,pRDName);});

    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    if(pLayerRD->m_pLightParam)
        pDevice->ModifyUniformBufferObject(pLayerRD->m_pLightParam ,reinterpret_cast<float*>(pBuffer));
    else
        pLayerRD->m_pLightParam = pDevice->CreateUniformBufferObject(nLightSize ,reinterpret_cast<float*>(pBuffer));
    
    SAFE_DELETE(pBuffer);
//    qDebug() << GetLight(0)->GetDynamicPos(pRDName);
}

RDCamera* RDLayer::GetCurCamera(const RDLayerRenderData& pLayerRD)const
{
    return m_vecCameraObj[pLayerRD.GetCurCameraID()];
}

RDCamera* RDLayer::GetCurCamera(const QString& pRDName)const
{
    const RDLayerRenderData* pLayerRD = dynamic_cast<const RDLayerRenderData*>(GetRenderData(pRDName));
    return GetCurCamera(*pLayerRD);
}

void        RDLayer::SetCurCamera(const QString& pRDName,size_t nIndex)
{
    RDLayerRenderData* pLayerRD = dynamic_cast<RDLayerRenderData*>(GetRenderData(pRDName));
    pLayerRD->SetCurCameraID(nIndex);
}

RDLayer::~RDLayer()
{
    for(size_t i = 0; i < m_vecCameraObj.size();i++)
    {
        RDCamera* pCamera = m_vecCameraObj[i];
        SAFE_DELETE(pCamera);
    }
}

void RDLayer::Serialize(RDFileDataStream& buffer,bool bSave)
{
    int nVersion = 0;
    buffer.Serialize(nVersion,bSave);
    RDNode::Serialize(buffer,bSave);
    int nType = m_nType;
    buffer.Serialize(nType,bSave);
    if(!bSave)
        m_nType = static_cast<RDLayerType>(nType);
    
    int nCameraCount = m_vecCameraObj.size();
    buffer.Serialize(nCameraCount,bSave);
    for(int i = 0; i < nCameraCount; i++)
    {
        RDCamera* pCamera = nullptr;
        if(bSave)
            pCamera = m_vecCameraObj[i];
        else
        {
            pCamera = new RDCamera;
            m_vecCameraObj.push_back(pCamera);
            pCamera->SetParent(this);
        }
        pCamera->Serialize(buffer,bSave);
    }

    int nLightCount = m_vecLight.size();
    buffer.Serialize(nLightCount,bSave);
    for(int i = 0;i  < nLightCount; i++)
    {
        RDLight* pLight = nullptr;
        if(bSave)
            pLight = m_vecLight[i];
        else
        {
            pLight = new RDLight;
            m_vecLight.push_back(pLight);
            pLight->SetParent(this);
        }
        pLight->Serialize(buffer,bSave);
    }
}

RDLight* RDLayer::RemoveLight(size_t nIndex)
{
    RDLight* pLight = m_vecLight[nIndex];
    m_vecLight.erase(m_vecLight.begin() + nIndex);
    return pLight ;
}

RDUBO*      RDLayer::GetLightParam(const QString& name)const
{
    const RDLayerRenderData* pLayerRD = dynamic_cast<const RDLayerRenderData*>(GetRenderData(name));
    return pLayerRD->m_pLightParam;
}
