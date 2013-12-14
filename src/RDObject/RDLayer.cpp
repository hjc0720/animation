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

RDObjectCreator<RDLayer,false> LayerCreator;

class RDLayerRenderData :public RDRenderData
{
public:
    RDLayerRenderData(RDNode& node,const RDSceneRenderData& SceneRenderData)
        :RDRenderData(node,SceneRenderData)
         ,m_nCurCameraID(0)
    { }
    size_t GetCurCameraID()const{return m_nCurCameraID;}
    void    SetCurCameraID(size_t nIndex){m_nCurCameraID = nIndex;}
public:
    size_t m_nCurCameraID;
};

RDLayer::RDLayer(RDLayerType nType,const QString& strName)
    :RDNode(strName)
     ,m_nType(nType)
{
     m_strName = strName;
     RDCamera* pCamera = new RDCamera("camera",1080,PerspectiveProject);
     pCamera->SetParent(this);
     m_vecCameraObj.push_back(pCamera);
}

RDNode* RDLayer::GetChild(size_t i)
{
    if(i < m_vecCameraObj.size())
        return GetCamera(i);
    return RDNode::GetChild(i - m_vecCameraObj.size());
}

const RDNode* RDLayer::GetChild(size_t i)const
{
    if(i < m_vecCameraObj.size())
        return GetCamera(i);
    return RDNode::GetChild(i - m_vecCameraObj.size());
}

RDNode* RDLayer::GetChild(const QUuid& NodeId)
{
    for(auto it = m_vecCameraObj.begin(); it != m_vecCameraObj.end(); it++)
    {
        if((*it)->GetNodeID() == NodeId)
            return *it;
        auto ret = (*it)->GetChild(NodeId);
        if(ret)
            return ret;
    }
    return RDNode::GetChild(NodeId);
}

RDNode* RDLayer::RemoveChild(size_t i)
{
    if(i < GetCameraCount())
        return RemoveCamera(i);

    return RDNode::RemoveChild(i - m_vecCameraObj.size());
} 

void RDLayer::RemoveChild(const RDNode& pChild)
{
    for(auto it = m_vecCameraObj.begin(); it != m_vecCameraObj.end(); it++)
    {
        if(*it == &pChild)
        {
            m_vecCameraObj.erase(it);
            return;
        }
    }
    RDNode::RemoveChild(pChild);
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
    for(size_t i = GetCameraCount();i < nCount;i++)
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
    while(!nodeSt.empty())
    {
        RDNode* pNode = nodeSt.top();
        RDRenderData* pRD = pNode->GetRenderData(pRDName);
        size_t nChildCount = pNode->GetChildCount();
        for(size_t i = 0;i < nChildCount;i++)
        {
            nodeSt.push(pNode->GetChild(i));
        }
        RDCalBoxMinMax(vNearFar.y,vNearFar.x,pRD->GetMin(),pRD->GetMax(),pCamera->GetViewMatrix(pRDName));
        nodeSt.pop();
    }
    vNearFar.x = min(-0.001,vNearFar.x + 0.1);
    vNearFar.y = max(vNearFar.x * 10e7,vNearFar.y - 0.1);
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

void RDLayer::CalFrame(const RDTime& nTime,const QString& pRDName) 
{
    RDLayerRenderData* pLayerRD = dynamic_cast<RDLayerRenderData*>(GetRenderData(pRDName));
    RDCamera* pCamera = GetCurCamera(*pLayerRD);
    pCamera->CalFrame(nTime,pRDName);
    RDNode::CalFrame(nTime,pRDName);
    float2 vNearFar = CalObjMinMax(pRDName);

    QRectF sceneRT(-(pLayerRD->GetSceneWidth() / 2.f),-(pLayerRD->GetSceneHeight() / 2.f),pLayerRD->GetSceneWidth(),pLayerRD->GetSceneHeight());
    pCamera->UpdateProject(pRDName,sceneRT,vNearFar.x,vNearFar.y);
    if(pCamera->GetRenderChangeLevel(pRDName) > RDRender_NoChange)
        pLayerRD->UnionDirty(sceneRT);
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
    
    int nCount = m_vecCameraObj.size();
    buffer.Serialize(nCount,bSave);
    for(int i = 0; i < nCount; i++)
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
}
