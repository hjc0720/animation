// =====================================================================================
// 
//       Filename:  RDNode.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  27/06/11 07:38:51
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDNode.h"
#include "RDObject.h"
#include "RDFileDataStream.h"
#include "RDRenderData.h"
#include "mac_define.h"
#include "RDCreateObj.h"
#include "RDSection.h"
#include "RDSceneRenderData.h"
#include <QDebug>
#include "RDScene.h"
#include "RDStory.h"
#include "RDLayer.h"
#include "RDCamera.h"
#include "RDRenderDevice.h"

using namespace std;

const int g_nFirstVersion = 0;
const int g_nNowVersion = g_nFirstVersion;

RDObjectCreator<RDNode,false> nodeCreator;

RDNode::RDNode()
      :m_vScale(1,1,1)
     ,m_NodeID(QUuid::createUuid())
	 ,m_pParent(nullptr)
     ,m_pObj(nullptr)
     ,m_lock(QMutex::Recursive)
{
}

RDNode::RDNode(const QString& strName)
      :m_vScale(1,1,1)
     ,m_strName(strName)
     ,m_NodeID(QUuid::createUuid())
	 ,m_pParent(nullptr)
     ,m_pObj(nullptr)
     ,m_lock(QMutex::Recursive)
{
}

RDNode::RDNode(const QString& strName,const float3& pos,RDObject* pObj)
    :m_vPos(pos)
      ,m_vScale(1,1,1)
     ,m_strName(strName)
     ,m_NodeID(QUuid::createUuid())
	 ,m_pParent(nullptr)
     ,m_pObj(pObj)
     ,m_lock(QMutex::Recursive)
{
}

RDNode::~RDNode()
{
    for(size_t i = 0; i < GetChildCount(); i++)
    {
        RDNode* pNode = GetChild(i);
        SAFE_DELETE(pNode);
    }
    for(auto it = m_vecRenderData.begin();it != m_vecRenderData.end();it++)
    {
        RDRenderData* pRenderData = it->second;
        SAFE_DELETE(pRenderData);
    }   
    for(auto mapit = m_vecSetctionListMap.begin(); mapit != m_vecSetctionListMap.end(); mapit++)
    {
        std::vector<RDSection*>& pSectionList = mapit->second;
        for(auto listIt = pSectionList.begin(); listIt != pSectionList.end(); listIt++)
        {
            RDSection* pSection = *listIt;
            SAFE_DELETE(pSection);
        }
    }
    SAFE_DELETE(m_pObj);
}

RDNode* RDNode::RemoveChild(size_t i)
{
    RDNode* pRemove = m_vecChildObj[i];
    m_vecChildObj.erase(m_vecChildObj.begin() + i);
    return pRemove;
}
void RDNode::Render(const RDTime& nTime,const QString& pRDName) 
{
    QMutexLocker locker(&m_lock);
    RDRenderData& RenderData = *GetRenderData(pRDName);
    if(!RenderData.GetCurSection())
    {
        qDebug() << typeid(*this).name() << "no section";   
        return;
    }
    for(size_t i = 0; i < GetChildCount(); i++)
    {
        GetChild(i)->Render(nTime,pRDName);
    }
    if(m_pObj)
    {
        const RDLayer* pLayer = GetLayerNode();
        RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
        pDevice->SetShaderParam(3,pLayer->GetLightParam(pRDName));
        RenderData.SetMVPMatrix(RenderData.GetGlobalMatrix() * GetViewProjMat(pRDName));
        m_pObj->Render(nTime,RenderData);
    }
    RenderData.ResetRenderChangeLevel();
}

void RDNode::CalNodeMatrix(RDRenderData& RenderData)
{
    matrix4x4 mat;
    matrix4x4::CreateWorldMat(mat,float3::GetZero(),RenderData.GetScale(),RenderData.GetAngle(),RenderData.GetPos());
    RenderData.SetItemMatrix(mat);
    RenderData.SetGlobalMatrix(RenderData.GetItemMatrix());
}

void RDNode::CalChildFrame(const RDTime& nTime,const QString& pRDName)
{
    RDRenderData* RenderData = GetRenderData(pRDName);

    std::for_each(m_vecChildObj.begin(),m_vecChildObj.end(),
            [&](RDNode* pChild){
            pChild->CalFrame(nTime,pRDName);
            RenderData->UnionDirty(pChild->GetRenderData(pRDName)->GetDirty());
            });
}

void RDNode::CalFrame(const RDTime& nTime,const QString& pRDName) 
{
    QMutexLocker locker(&m_lock);
    RDRenderData& RenderData = *GetRenderData(pRDName);
    RenderData.ResetDirty();
    UpdateSection(nTime,RenderData);
    if(!RenderData.GetCurSection())
        return;
    RDTime nSectionTime = nTime - RenderData.GetCurStory()->GetStartTime(RenderData.IsPlay()) - RenderData.GetCurSection()->GetStartTime();
    if(nSectionTime == RenderData.GetSectionTime() && GetMaxChangeLevel(pRDName) == RDRender_NoChange)
        return;
    if(CalSpaceVector(nSectionTime,RenderData))
        SetChangeLevel(RDRender_TransChange);

    if(RenderData.GetChangeLevel() >= RDRender_TransChange)
        CalNodeMatrix(RenderData);

    if(m_pObj)
    {
        m_pObj->CalFrame(nSectionTime,RenderData);
        if(RenderData.GetChangeLevel() > RDRender_NoChange)
        {
            //qDebug() << "Node Dirty" << RenderData.GetScaleBound();
            RenderData.UnionDirty(RenderData.GetScaleBound());
            m_pObj->UpdateBound(nTime,RenderData);
            RenderData.UnionDirty(RenderData.GetScaleBound());
        }
    }
    CalChildFrame(nTime,pRDName);
    RenderData.SetRenderChangeLevel(RenderData.GetChangeLevel());
    RenderData.ResetChangeLevel();
    RenderData.SetSectionTime(nSectionTime);
}

RDTime RDNode::GetSectionMaxLength(const QUuid& idStory)const
{
	auto pSectionList = m_vecSetctionListMap.find(idStory);
	RDTime nTime = 0;
	if(pSectionList != m_vecSetctionListMap.end())
	{
		auto it = pSectionList->second.rbegin();
		RDSection* pSection = *it;
		nTime = pSection->GetEndTime();
	}
	for(size_t i = 0;i < GetChildCount(); i++)
	{
		const RDNode* pChildNode = GetChild(i);
		nTime = max(nTime,pChildNode->GetSectionMaxLength(idStory));
	}
	return nTime;
}
bool RDNode::AddSection(const RDTime& nStoryTime,const RDTime& nLength,const QUuid& storyId)
{
    RDSection* pNewSection = new RDSection(nStoryTime,nLength);
    auto pFindRes = m_vecSetctionListMap.find(storyId);
    if(pFindRes == m_vecSetctionListMap.end())
    {
        std::vector<RDSection*> sectionList;
        sectionList.push_back(pNewSection);
        m_vecSetctionListMap.insert(pair<QUuid,std::vector<RDSection*>>(storyId,sectionList));
        return true;
    }
    std::vector<RDSection*>& pSectionList = pFindRes->second;
    for(auto it = pSectionList.begin(); it != pSectionList.end(); it++) 
    {
        if(nStoryTime < (*it)->GetStartTime())
        {
            pSectionList.insert(it,pNewSection);
            MoveSection(nLength,it,pSectionList.end());
            return true;
        }
       else if(nStoryTime >= (*it)->GetStartTime() && nStoryTime < (*it)->GetEndTime())
        {
            return false;
        }
    }
    pSectionList.push_back(pNewSection);
    return true;
}

void RDNode::UpdateSection(const RDTime& nFrame,RDRenderData& pRD)
{
    const RDSceneRenderData& pSceneRD = pRD.GetSceneRD();
    const RDScene& pScene = dynamic_cast<const RDScene&>(pSceneRD.GetNode());
    auto pCurStory = pScene.GetCurStory(pSceneRD);

    auto pFindRes = m_vecSetctionListMap.find(pCurStory->GetStoryId());
    if(pFindRes != m_vecSetctionListMap.end())
    {
        auto pSectionList = pFindRes->second;
        RDTime nStoryFrame = nFrame - pCurStory->GetStartTime(pSceneRD.IsPlay());
        auto it = pSectionList.begin();
        while(it != pSectionList.end())
        {
            auto oldIt = it;
            it++;
            if(nStoryFrame < (*oldIt)->GetStartTime())
            {
                //find last action of prev story;
                RDSection* pLastSection = GetLastSectionBefore(pScene.GetCurStoryIndex(pSceneRD));
                if(pLastSection && pLastSection->GetType() == RDSectionFinish)
                    pRD.SetCurSection(0);
                else
                    pRD.SetCurSection(pLastSection);
                return;
            }
            else if(nStoryFrame < (*oldIt)->GetStartTime() + (*oldIt)->GetLength())
            {
                pRD.SetCurSection(*oldIt);
                return;
            }
            else if(it == pSectionList.end())
            {
                if((*oldIt)->GetType() != RDSectionFinish)
                    pRD.SetCurSection(*oldIt);
                else
                    pRD.SetCurSection(0);
                return;
            }
            else
            {
                if(nStoryFrame < (*it)->GetStartTime())
                {
                    if((*oldIt)->GetType() != RDSectionFinish)
                        pRD.SetCurSection(*oldIt);
                    else 
                        pRD.SetCurSection(0);
                    return;
                }
            }
        }
    }
    RDSection* pLastSection = GetLastSectionBefore(pScene.GetCurStoryIndex(pSceneRD));
    if(pLastSection && pLastSection->GetType() == RDSectionFinish)
        pRD.SetCurSection(0);
    else
        pRD.SetCurSection(pLastSection);
}

RDSection* RDNode::GetLastSectionBefore(size_t nCurStoryIndex)
{
    const RDScene* pScene = GetSceneNode();
    auto pCurStory = pScene->GetStory(--nCurStoryIndex);
    while(pCurStory)
    {
        auto pFindRes = m_vecSetctionListMap.find(pCurStory->GetStoryId());
        if(pFindRes != m_vecSetctionListMap.end())
        {
            auto& pSectionList = pFindRes->second;
            auto it = pSectionList.rbegin();
            return *it;
        }
        pCurStory = pScene->GetStory(--nCurStoryIndex);
    }
    return 0;
}
void RDNode::RemoveChild(const RDNode& pChild)
{
    size_t nCount = GetChildCount();
    for(size_t i = 0; i < nCount; i++)
    {
        RDNode* pNode = GetChild(i);
        if(pNode == &pChild)
        {
            RemoveChild(i);
            return ;
        }
    }
} 

void RDNode::MoveSection(const RDTime& nSteps, RDSectionList pStart,RDSectionList pEnd )
{
    for(RDSectionList it = pStart; it != pEnd; it++) 
    {
        (*it)->MovSection(nSteps);
    }
}

RDRenderData*   RDNode::CreateRenderData(const QString& pName)
{
    QMutexLocker locker(&m_lock);
    const RDScene* pScene = GetSceneNode();
    RDRenderData* pRenderData = new RDRenderData(*this,*dynamic_cast<const RDSceneRenderData*>(pScene->GetRenderData(pName)));
    m_vecRenderData[pName] = pRenderData;
    return pRenderData;
}

const RDScene*        RDNode::GetSceneNode() const
{
    const RDScene* pScene = dynamic_cast<const RDScene*>(this);
    if(!pScene && m_pParent)
        return m_pParent->GetSceneNode();
    return pScene;
}

RDRenderData*   RDNode::GetRenderData(const QString& pName)
{
    RDRenderData* pRD = m_vecRenderData[pName];
    if(!pRD)
        pRD = CreateRenderData(pName);
    return pRD;
}

const RDRenderData*   RDNode::GetRenderData(const QString& pName)const
{
    auto it = m_vecRenderData.find(pName);
    if(it == m_vecRenderData.end())
        return 0;
    RDRenderData* pRD = it->second;
    return pRD;
}
void  RDNode::SetRenderScale(float fScale,const QString& pName)
{
    RDRenderData* pRD = GetRenderData(pName);
    pRD->SetSceneScale(fScale);
    for(size_t i = 0; i < GetChildCount(); i++)
    {
        GetChild(i)->SetRenderScale(fScale,pName);
    }   
}

RDRenderChangeLevel RDNode::GetChangeLevel(const QString& pName)const
{
    const RDRenderData* pRD = GetRenderData(pName);
    if(pRD)
        return pRD->GetChangeLevel();
    else
        return RDRender_NoChange;
}

RDRenderChangeLevel RDNode::GetRenderChangeLevel(const QString& pName)const
{
    const RDRenderData* pRD = GetRenderData(pName);
    if(pRD)
        return pRD->GetRenderChangeLevel();
    else
        return RDRender_NoChange;
}

RDRenderChangeLevel RDNode::GetMaxRenderChangeLevel(const QString& pName)const
{
    RDRenderChangeLevel maxC = GetRenderChangeLevel(pName);
    for(size_t i = 0 ;i < GetChildCount();i++)
    {
        RDRenderChangeLevel childMax = GetChild(i)->GetMaxRenderChangeLevel(pName);
        maxC = maxC > childMax ? maxC : childMax;
    }
    return maxC;
}

RDRenderChangeLevel RDNode::GetMaxChangeLevel(const QString& pName)const
{
    RDRenderChangeLevel maxC = GetChangeLevel(pName);
    for(size_t i = 0 ;i < GetChildCount();i++)
    {
        RDRenderChangeLevel childMax = GetChild(i)->GetMaxChangeLevel(pName);
        maxC = maxC > childMax ? maxC : childMax;
    }
    return maxC;
}

void RDNode::SetChangeLevel(RDRenderChangeLevel nLevel)
{
    for(auto it = m_vecRenderData.begin();it != m_vecRenderData.end();it++)
    {
        RDRenderData* pRenderData = it->second;
        pRenderData->SetChangeLevel(nLevel);
    }   
}
void RDNode::RemoveRenderData()
{
    for(size_t i = 0;i < GetChildCount();i++)
    {
        RDNode* pChild = GetChild(i);
        pChild->RemoveRenderData();
    }
    for(auto it = m_vecRenderData.begin();it != m_vecRenderData.end();it++)
    {
        RDRenderData* pRenderData = it->second;
        SAFE_DELETE(pRenderData);
    }   
    m_vecRenderData.clear();
}
size_t RDNode::GetTotalChildCount()const
{
	int nCount = GetChildCount();
	for(size_t i = 0; i < GetChildCount(); i++)
		nCount += GetChild(i)->GetTotalChildCount();
	return nCount;
}
RDSection* RDNode::GetSection(const QUuid& nStoryId,const RDTime& nStoryFrame)
{
    auto SectionList = m_vecSetctionListMap[nStoryId];
    for(auto it = SectionList.rbegin(); it != SectionList.rend(); it++)
    {
        if(nStoryFrame >= (*it)->GetStartTime())
            return *it;
    }
    return 0;
}

void RDNode::AddAngleKey(const RDTime& nTime,const float3& vOffsetAngle )
{
    const RDScene* pScene = GetSceneNode();
    const RDStory* pCurStory = pScene->GetStory(nTime,false);
    RDTime nStoryTime = nTime - pCurStory->GetStartTime(false);
    RDSection* pSection = GetSection(pCurStory->GetStoryId(),nStoryTime);
    pSection->AddAngleKey(nStoryTime,vOffsetAngle);
}

void RDNode::AddScaleKey(const RDTime& nTime,const float3& vOffsetScale )
{
    const RDScene* pScene = GetSceneNode();
    const RDStory* pCurStory = pScene->GetStory(nTime,false);
    RDTime nStoryTime = nTime - pCurStory->GetStartTime(false);
    RDSection* pSection = GetSection(pCurStory->GetStoryId(),nStoryTime);
    pSection->AddScaleKey(nStoryTime,vOffsetScale);
}

void RDNode::AddPosKey(const RDTime& nTime,const float3& vOffsetPos )
{
    const RDScene* pScene = GetSceneNode();
    const RDStory* pCurStory = pScene->GetStory(nTime,false);
    RDTime nStoryTime = nTime - pCurStory->GetStartTime(false);
    RDSection* pSection = GetSection(pCurStory->GetStoryId(),nStoryTime);
    //qDebug() << "add key Story Time" << nStoryTime << nTime;
    pSection->AddPosKey(nStoryTime,vOffsetPos);
}

const RDNode* RDNode::GetChild(const QUuid& NodeId)const
{
    int nCount = GetChildCount();
    for(int i = 0; i < nCount; i++)
    {
        const RDNode* pChild = GetChild(i);
        if(pChild->GetNodeID() == NodeId)
            return pChild;
    }
    return 0;
}
RDNode* RDNode::GetChild(const QUuid& NodeId)
{
    int nCount = GetChildCount();
    for(int i = 0; i < nCount; i++)
    {
        RDNode* pChild = GetChild(i);
        if(pChild->GetNodeID() == NodeId)
            return pChild;
        RDNode* ret = pChild->GetChild(NodeId);
        if(ret)
            return ret;
    }
    return 0;
}

bool RDNode::CalSpaceVector(const RDTime& nSectionTime,RDRenderData& RenderData)
{
    bool ret = false;
    RDSection* pSection = RenderData.GetCurSection();
    //======================================================================================================================
    //cal pos
    float3 vOffsetPos = pSection->GetPosVector(nSectionTime);
    vOffsetPos += m_vPos;
    ret |= (vOffsetPos != RenderData.GetPos());
    RenderData.SetPos(vOffsetPos);
    //======================================================================================================================
    //cal Rotate
    float3 vOffsetAngle = pSection->GetAngleVector(nSectionTime);
    vOffsetAngle += m_vAngle;
    ret |= (vOffsetAngle != RenderData.GetAngle());
    RenderData.SetAngle(vOffsetAngle);
    //======================================================================================================================
    //cal Scale
    float3 vOffsetScale = pSection->GetScaleVector(nSectionTime);
    vOffsetScale.Mul(m_vScale);
    ret |= (vOffsetScale != RenderData.GetScale());
    RenderData.SetScale(vOffsetScale);
    return ret;
}

size_t RDNode::GetSectionCount(const QUuid& idStory)const
{
    auto pFindRes = m_vecSetctionListMap.find(idStory);
    if(pFindRes != m_vecSetctionListMap.end())
        return pFindRes->second.size();
    else
        return 0;
}

RDSection* RDNode::GetSection(const QUuid& idStory,size_t nIndex)
{
    return m_vecSetctionListMap[idStory].at(nIndex);
}

const matrix4x4&     RDNode::GetViewProjMat(const QString& RDName)
{
    RDCamera* pCamera = GetCamera(RDName);
    return pCamera->GetViewProjMat(RDName);
}

RDCamera *RDNode::GetCamera(const QString& strName)const
{
    const RDLayer* pLayer = GetLayerNode();
    return pLayer->GetCurCamera(strName);
}

RDSpaceParam RDNode::GetEditSpaceParam(const QString &strName, const matrix4x4 *pWorldMat) const
{
    RDCamera* pCamera = GetCamera(strName);
    return RDSpaceParam(pWorldMat,&pCamera->GetViewMatrix(strName),&pCamera->GetEditProjMatrix(strName),GetSceneRt(strName));
}

QRectF RDNode::GetSceneRt(const QString& strName)const
{
    const RDScene* pScene= GetSceneNode();
    return pScene->GetSceneRt(strName);
}

const matrix4x4 &RDNode::GetNodeMatrix(const QString &strName) const
{
    const RDRenderData* pData = GetRenderData(strName);
    return pData->GetGlobalMatrix();
}

const RDLayer*        RDNode::GetLayerNode()const
{
    const RDLayer* pLayer = dynamic_cast<const RDLayer*>(this);
    if(!pLayer && m_pParent)
        return m_pParent->GetLayerNode();
    return pLayer;
}

void RDNode::Serialize(RDFileDataStream& buffer,bool bSave)
{
    QMutexLocker locker(&m_lock);
    int nVersion = g_nNowVersion;
    buffer.Serialize(nVersion,bSave);
    buffer.Serialize(m_strName,bSave);
    //qDebug() << "begin to serialize node :" << m_strName;
    buffer.Serialize(m_vPos,bSave);
    buffer.Serialize(m_NodeID,bSave);

    bool bHaveObj = m_pObj != nullptr;
    buffer.Serialize(bHaveObj,bSave);
    if(bHaveObj)
    {
        if(bSave)
        {
            QString ObjType(typeid(*m_pObj).name());
            buffer.Serialize(ObjType,true);
        }
        else
        {
            QString ObjType;
            buffer.Serialize(ObjType,false);
            m_pObj = CreateObj(ObjType);
        }
        m_pObj->Serialize(buffer,bSave);
    }
    //serialize section
    int nSectionListCount = m_vecSetctionListMap.size();
    buffer.Serialize(nSectionListCount,bSave);
    if(bSave)
    {
        for(auto it = m_vecSetctionListMap.begin(); it != m_vecSetctionListMap.end(); it++)
        {
            buffer << it->first;
            const vector<RDSection*>& SectionList = it->second;
            int nSectionCount = SectionList.size();
            buffer << nSectionCount;
            for(auto SectionIt = SectionList.begin(); SectionIt != SectionList.end(); SectionIt ++)
            {
                RDSection* pSection = *SectionIt;
                buffer << *pSection;
            }
        }
    }
    else
    {
        for(int i = 0; i < nSectionListCount; i++)
        {
            QUuid StoryId;
            buffer >> StoryId;
            vector<RDSection*> SectionList;
            int nSectionCount;
            buffer >> nSectionCount;
            for(int i = 0; i < nSectionCount; i++)
            {
                RDSection* pSection = new RDSection;
                buffer >> *pSection;
                SectionList.push_back(pSection);
            }
            m_vecSetctionListMap[StoryId] = SectionList;
        }
    }
    //serialize child
    int nChildCount = static_cast<int>(m_vecChildObj.size());
    buffer.Serialize(nChildCount,bSave);
    for(int i = 0; i < nChildCount; i++)
    {
        RDNode* pNode = nullptr;
        if(bSave)
        {
            pNode = m_vecChildObj[i];
            QString pTypeName(typeid(*pNode).name());

            //qDebug() << "serialize node type" << pTypeName;
            buffer.Serialize(pTypeName,true);
        }
        else
        {
            QString pTypeName;
            buffer.Serialize(pTypeName,false);
            pNode = CreateNode(pTypeName);
            m_vecChildObj.push_back(pNode);
            pNode->m_pParent = this;
        }
        pNode->Serialize(buffer,bSave);
    }

    //qDebug() << "end to serialize node :" << m_strName;
}

const float3& RDNode::GetDynamicPos(const QString& pName)const
{
    const RDRenderData* pData = GetRenderData(pName);
    return pData->GetPos();
}
//================================================================================
//undo
RDPosUndo::RDPosUndo(RDNode& pNode)
    :RDUndoCommand(pNode.GetNodeID(),QObject::tr("pos"))
     ,m_OldPos(pNode.GetPos())
{
}

void RDPosUndo::undo()
{
    float3 vOldPos = m_OldPos;
    qDebug() << "oldPos" << vOldPos.x() << vOldPos.y() << vOldPos.z();
    m_OldPos = m_pNode->GetPos();
    m_pNode->SetPos(vOldPos);
    m_pNode->SetChangeLevel(RDRender_TransChange);
    //qDebug() << "newPos" << m_OldPos.x() << m_OldPos.y() << m_OldPos.z();
}

void RDPosUndo::redo()
{
    float3 vOldPos = m_OldPos;
    m_OldPos = m_pNode->GetPos();
    m_pNode->SetPos(vOldPos);
    m_pNode->SetChangeLevel(RDRender_TransChange);
}
