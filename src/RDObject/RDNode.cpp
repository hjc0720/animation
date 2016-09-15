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
#include <cfloat>

using namespace std;

const int g_nFirstVersion = 0;
const int g_nNowVersion = g_nFirstVersion;

RDObjectCreator<RDNode,false> nodeCreator;

RDNode::RDNode(const std::string& strName)
    :m_strName(strName)
{
}

RDNode::RDNode(const std::string& strName,const float3& pos,RDObject* pObj)
     :m_vPos(pos)
     ,m_strName(strName)
     ,m_pObj(pObj)
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
void RDNode::Render(const RDTime& nTime,const std::string& pRDName) 
{
    RDSingleLock locker(m_lock);
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

void RDNode::CalChildFrame(const RDTime& nTime,const std::string& pRDName)
{
    RDRenderData* RenderData = GetRenderData(pRDName);

    std::for_each(m_vecChildObj.begin(),m_vecChildObj.end(),
            [&](RDNode* pChild){
            pChild->CalFrame(nTime,pRDName);
            RenderData->UnionDirty(pChild->GetRenderData(pRDName)->GetDirty());
            });
}

void RDNode::CalFrame(const RDTime& nTime,const std::string& pRDName) 
{
    RDSingleLock locker(m_lock);
    RDRenderData& RenderData = *GetRenderData(pRDName);
    RenderData.ResetDirty();
    RDTime nSectionTime = UpdateSection(nTime,RenderData);
    if(!RenderData.GetCurSection())
        return;
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
	if(pSectionList == m_vecSetctionListMap.end())
		return 0;

	auto it = pSectionList->second.rbegin();
	RDSection* pSection = *it;
	RDTime nTime = pSection->GetEndTime();

	for(size_t i = 0;i < GetChildCount(); i++)
	{
		const RDNode* pChildNode = GetChild(i);
		nTime = max(nTime,pChildNode->GetSectionMaxLength(idStory));
	}
	qDebug()<<m_strName << "section length" << nTime;
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

RDTime RDNode::UpdateSection(const RDTime& nFrame,RDRenderData& pRD)
{
	RDTime storyTime = 0;
	pRD.SetCurSection(FindNearestSection(storyTime,pRD,nFrame));
	pRD.setStoryTime(storyTime);

    RDTime nSectionTime = calSectionTime(nFrame,pRD);
    if(nSectionTime < 0)
        pRD.SetCurSection(nullptr);
    return nSectionTime;
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

RDRenderData*   RDNode::CreateRenderData(const std::string& pName)
{
    const RDScene* pScene = GetSceneNode();
    RDRenderData* pRenderData = new RDRenderData(pName,*this,*dynamic_cast<const RDSceneRenderData*>(pScene->GetRenderData(pName)));
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

RDRenderData*   RDNode::GetRenderData(const std::string& pName)
{
    RDRenderData* pRD = m_vecRenderData[pName];
    if(!pRD)
        pRD = CreateRenderData(pName);
    return pRD;
}

const RDRenderData*   RDNode::GetRenderData(const std::string& pName)const
{
    auto it = m_vecRenderData.find(pName);
    if(it == m_vecRenderData.end())
        return 0;
    RDRenderData* pRD = it->second;
    return pRD;
}

RDRenderChangeLevel RDNode::GetChangeLevel(const std::string& pName)const
{
    const RDRenderData* pRD = GetRenderData(pName);
    if(pRD)
        return pRD->GetChangeLevel();
    else
        return RDRender_NoChange;
}

RDRenderChangeLevel RDNode::GetRenderChangeLevel(const std::string& pName)const
{
    const RDRenderData* pRD = GetRenderData(pName);
    if(pRD)
        return pRD->GetRenderChangeLevel();
    else
        return RDRender_NoChange;
}

RDRenderChangeLevel RDNode::GetMaxRenderChangeLevel(const std::string& pName)const
{
    RDRenderChangeLevel maxC = GetRenderChangeLevel(pName);
    for(size_t i = 0 ;i < GetChildCount();i++)
    {
        RDRenderChangeLevel childMax = GetChild(i)->GetMaxRenderChangeLevel(pName);
        maxC = maxC > childMax ? maxC : childMax;
    }
    return maxC;
}

RDRenderChangeLevel RDNode::GetMaxChangeLevel(const std::string& pName)const
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
        pRenderData->setChangeLevel(nLevel);
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

void RDNode::AddAngleKey(const RDTime& nTime,const float3& vOffsetAngle ,const std::string& strName)
{
    const RDStory& pCurStory = GetRenderData(strName)->GetCurTrigStory();
    RDTime nStoryTime = nTime - pCurStory.GetStartTime();
    RDSection* pSection = GetSection(pCurStory.GetStoryId(),nStoryTime);
    pSection->AddAngleKey(nStoryTime,vOffsetAngle);
}

void RDNode::AddScaleKey(const RDTime& nTime,const float3& vOffsetScale ,const std::string& strName)
{
    const RDStory& pCurStory = GetRenderData(strName)->GetCurTrigStory();
    RDTime nStoryTime = nTime - pCurStory.GetStartTime();
    RDSection* pSection = GetSection(pCurStory.GetStoryId(),nStoryTime);
    pSection->AddScaleKey(nStoryTime,vOffsetScale);
}

void RDNode::AddPosKey(const RDTime& nTime,const float3& vOffsetPos ,const std::string& strName)
{
    const RDStory& pCurStory = GetRenderData(strName)->GetCurTrigStory();
    RDTime nStoryTime = nTime - pCurStory.GetStartTime();
    RDSection* pSection = GetSection(pCurStory.GetStoryId(),nStoryTime);
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

const matrix4x4&     RDNode::GetViewProjMat(const std::string& RDName)
{
    RDCamera* pCamera = GetCamera(RDName);
    return pCamera->GetViewProjMat(RDName);
}

RDCamera *RDNode::GetCamera(const std::string& strName)const
{
    const RDLayer* pLayer = GetLayerNode();
    return pLayer->GetCurCamera(strName);
}

RDSpaceParam RDNode::GetEditSpaceParam(const std::string &strName, const matrix4x4 *pWorldMat) const
{
    RDCamera* pCamera = GetCamera(strName);
    return RDSpaceParam(pWorldMat,&pCamera->GetViewMatrix(strName),&pCamera->GetEditProjMatrix(strName),GetSceneRt(strName));
}

QRectF RDNode::GetSceneRt(const std::string& strName)const
{
    const RDScene* pScene= GetSceneNode();
    return pScene->GetSceneRt(strName);
}

const matrix4x4 &RDNode::GetNodeMatrix(const std::string &strName) const
{
    const RDRenderData* pData = GetRenderData(strName);
    return pData->GetGlobalMatrix();
}

void RDNode::paste(const string &)
{

}

const RDLayer*        RDNode::GetLayerNode()const
{
    const RDLayer* pLayer = dynamic_cast<const RDLayer*>(this);
    if(!pLayer && m_pParent)
        return m_pParent->GetLayerNode();
    return pLayer;
}

void RDNode::Serialize(RDJsonDataStream& buffer, Json::Value &parent)
{
    RDSingleLock locker(m_lock);
    buffer.Serialize(parent,"name",m_strName,std::string());
    //qDebug() << "begin to serialize node :" << m_strName;
    buffer.Serialize(parent,"pos",m_vPos,float3::GetZero());
    buffer.Serialize(parent,"id",m_NodeID);
    buffer.Serialize(parent,"collapse",m_bCollapse,false);

    if(buffer.IsSave() && m_pObj)
    {
        std::string ObjType(typeid(*m_pObj).name());
        buffer.Serialize(parent,"objtype",ObjType);
        Json::Value object(Json::objectValue);
        m_pObj->Serialize(buffer,object);
        parent["obj"] = object;
    }
    else if(!buffer.IsSave())
    {
        std::string ObjType;
        buffer.Serialize(parent,"objtype",ObjType);
        if(ObjType != "")
        {
            m_pObj = CreateObj(ObjType);
            m_pObj->Serialize(buffer,parent["obj"]);
            m_pObj->SetNode(this);
        }
    }

    buffer.Serialize(parent,"sections",m_vecSetctionListMap,[](RDJsonDataStream&buffer, Json::Value& child,QUuid& id,vector<RDSection*>& sections){
        buffer.Serialize(child,"key",id);
        buffer.Serialize(child,"value",sections.begin(),sections.end(),back_inserter(sections),[](RDJsonDataStream&buffer,Json::Value& value,RDSection*& section){
            if(!buffer.IsSave())
                section = new RDSection;
            section->Serialize(buffer,value);
        });
    });
    //serialize child
    buffer.Serialize(parent,"child",m_vecChildObj.begin(),m_vecChildObj.end(),back_inserter(m_vecChildObj),[this](RDJsonDataStream&buffer,Json::Value& child,RDNode*& node){
        std::string childType;
        if(buffer.IsSave())
            childType = typeid(*node).name();
        buffer.Serialize(child,"type",childType);
        if(!buffer.IsSave())
        {
            node = CreateNode(childType);
            node->m_pParent = this;
        }
        node->Serialize(buffer,child);
    });

    //qDebug() << "end to serialize node :" << m_strName;
}

const float3& RDNode::GetDynamicPos(const std::string& pName)const
{
    const RDRenderData* pData = GetRenderData(pName);
    return pData->GetPos();
}

bool    RDNode::isParentCollapse()const
{
    if(!m_pParent)
        return false;
    else if(m_pParent->collapse())
        return true;
    else
        return m_pParent->isParentCollapse();
}

const RDMd5 &RDNode::getNodeMd5() const
{
    static RDMd5 null;
    if(m_pObj)
        return m_pObj->GetObjMd5();
    else
        return null;
}

RDRenderPrivateData* RDNode::GetPrivateData(const std::string& name)const
{
    return GetRenderData(name)->GetPrivateData();
}

RDTime            RDNode::calSectionTime(RDTime time,RDRenderData& pRD)
{
    RDSection* pCurSection = pRD.GetCurSection();
    if(!pCurSection)
        return -1;
    RDTime nSectionTime = time - pRD.getStoryTime() - pCurSection->GetStartTime();
    switch(pCurSection->GetType())
    {
    case RDSectionFinish:
        if(nSectionTime > pCurSection->GetLength())
            nSectionTime = -1;
        break;
    case RDSectionKeep:
        nSectionTime = min(nSectionTime,pCurSection->GetLength());
        break;
    case RDSecionCycle:
        nSectionTime = nSectionTime % pCurSection->GetLength();
        break;
    }
    return nSectionTime;
}

RDSection*		RDNode::FindNearestSection(RDTime& storyTime,const RDRenderData& renderData,RDTime time)
{
    const RDSceneRenderData& pSceneRD = renderData.GetSceneRD();
    auto listTrigStory = pSceneRD.getTrigStoryList();
	for(auto storyIt = listTrigStory.begin(); storyIt != listTrigStory.end(); storyIt++)
	{
		RDStory& story = *storyIt;
		auto pFindRes = m_vecSetctionListMap.find(story.GetStoryId());
		if(pFindRes != m_vecSetctionListMap.end())
		{ 
			auto pSectionList = pFindRes->second;
			RDTime nStoryTime = time - story.GetStartTime();
			for(auto sectionIt = pSectionList.rbegin(); sectionIt != pSectionList.rend();sectionIt++)
			{
				if(nStoryTime >= (*sectionIt)->GetStartTime())
				{
					storyTime = story.GetStartTime();
					return *sectionIt;
				}
			}
		}
	}
	return nullptr;
}

void RDNode::RemoveSection(RDSection* pSection)
{
	std::for_each(m_vecRenderData.begin(),m_vecRenderData.end(),[=](std::pair<const std::string,RDRenderData*>& it){
			if(it.second->GetCurSection() == pSection) it.second->SetCurSection(nullptr);
			});

	for(auto it = m_vecSetctionListMap.begin();it != m_vecSetctionListMap.end(); it++)
	{
		auto vecSection = it->second;
		auto findret = std::find(vecSection.begin(),vecSection.end(),pSection);
		if(findret != vecSection.end())
		{
			if(vecSection.size() == 1)
			   m_vecSetctionListMap.erase(it);
			else
				vecSection.erase(findret);
			SetChangeLevel(RDRender_TransChange);
			return;
		}
	}
}

float RDNode::HitTest(const float3& vScenePt,const std::string& RDName)const
{
	float fDistance = FLT_MAX;
	bool bHit = false;
	if(GetObject() )
	{
		float fTemp = GetObject()->HitTest(vScenePt,*this,RDName);
		if(fTemp > 0)
		{
			fDistance = fTemp;
			bHit = true;
		}
	}

    std::for_each(m_vecChildObj.begin(),m_vecChildObj.end(),
            [&](RDNode* pChild){
            float fTemp = pChild->HitTest(vScenePt,RDName);
			if(fTemp > 0)
			{
				fDistance = std::min(fDistance,fTemp);
				bHit = true;
			}
            });
	return bHit ? fDistance : -1;
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

