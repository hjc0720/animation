// =====================================================================================
// 
//       Filename:  RDScene.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 21:53:19
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDScene.h"
#include <typeinfo>
#include "RDSceneRenderData.h"
#include <iostream>
#include <QMutexLocker>
#include "mac_define.h"
#include "RDResource.h"
#include "RDResourceManager.h"
#include "RDImageResource.h"
#include "RDFileDataStream.h"
#include <QRectF>
#include "RDSpaceConvert.h"
#include "RDStory.h"
#include <QDebug>
#include "RDLayer.h"

using namespace std;
const int g_nSceneVersion = 0;

class RDScenePrivateData : public RDRenderPrivateData
{
public:
    RDScenePrivateData();
    ~RDScenePrivateData();
    void SetResource(RDResource* pResource){m_pResource = pResource;}
    RDResource* GetResource(){return m_pResource;}
    size_t GetCurStoryIndex()const{return m_nCurStoryIndex;}
    void SetCurStoryIndex(size_t nIndex){m_nCurStoryIndex = nIndex;} 
protected:
    RDResource* m_pResource;
    size_t m_nCurStoryIndex;
};

RDScenePrivateData::RDScenePrivateData()
{
    m_pResource = NULL;
    m_nCurStoryIndex = 0;
}

RDScenePrivateData::~RDScenePrivateData()
{
    if(m_pResource)
    {
        RDResourceManager::GetResourceManager()->RemoveResource(&m_pResource->GetMd5());
        m_pResource = NULL;
    }
}
RDScene::RDScene()
    :RDNode()
{
    memset(&m_BackData,0,sizeof(RDSceneData));
}
RDScene::RDScene(const QString& strName)
    :RDNode(strName,float3(0,0,0),NULL)
     //,m_nSceneVersion(g_nSceneVersion)
{
    unsigned long newColor = 0xffffffff;
    m_BackData.pImage = new RDMd5;
    SetBackType(RDScene_Back_Color,&newColor);

    RDStory* pFirstStory = new RDStory;
    m_StoryList.push_back(pFirstStory);
}
RDScene::~RDScene()
{
	SAFE_DELETE(m_BackData.pImage);
    std::list<RDStory*>::iterator it = m_StoryList.begin();
    for(;it != m_StoryList.end();it++)
    {
        RDStory* pStory = *it;
        SAFE_DELETE(pStory);
    }
}
void RDScene::CreateRenderData(RDRenderData& RenderData)
{
    QMutexLocker locker(&m_lock);
    RDScenePrivateData* pPrivateData = new RDScenePrivateData;
    RenderData.SetPrivateData(pPrivateData);
}

//void RDScene::ReleaseRenderData(RDRenderData& RenderData)
//{
    //QMutexLocker locker(&m_lock);
    //RDScenePrivateData* pPrivateData = (RDScenePrivateData* )RenderData.GetPrivateData();
    //SAFE_DELETE(pPrivateData);
    //RenderData.SetPrivateData(0);
//}

void RDScene::SetBackType(RDScene_BackType nType,const void* pData)
{
    QMutexLocker locker(&m_lock);
    m_BackData.m_nBackType = nType;
    switch(nType)
    {
    case RDScene_Back_Color:
        m_BackData.backColor = *(unsigned long*)pData;
        break;
    case RDScene_Back_Picture:
       *m_BackData.pImage = *(RDMd5*)pData;
        break;
    }
    SetChangeLevel(RDRender_GraphicChange);
}

void RDScene::Render(const RDTime& nTime,const QString& pRDName)
{
    QMutexLocker locker(&m_lock);
    RDSceneRenderData& pSceneData = *dynamic_cast<RDSceneRenderData*>(GetRenderData(pRDName));
    if(pSceneData.GetRenderChangeLevel() >= RDRender_GraphicChange)
    {
        pSceneData.m_RenderBuffer.ResizeBuffer(pSceneData.GetNowWidth(),pSceneData.GetNowHeight());
    }
    switch(m_BackData.m_nBackType)
    {
    case RDScene_Back_Color:
        pSceneData.m_RenderBuffer.FillColor(m_BackData.backColor);
        break;
    case RDScene_Back_Picture:
        RenderImage(pSceneData,nTime);
        break;
    default:
        return;
    }
    RDNode::Render(nTime,pRDName);
    //BlendChild(pRDName);
}

void RDScene::RenderImage(RDSceneRenderData& pSceneData,unsigned long )
{
    RDScenePrivateData* pData = (RDScenePrivateData*)pSceneData.GetPrivateData();
    RDImageResource* pResource = dynamic_cast<RDImageResource*>(pData->GetResource());
    //Render Image;
    //const RDTexture* pImage = pResource->GetBuffer();
    //QRectF dst(0,0,pSceneData.GetNowWidth(),pSceneData.GetNowHeight());
    //QRectF src(0,0,pImage->GetWidth(),pImage->GetHeight());
    //pSceneData.m_RenderBuffer.Draw(dst,*pResource->GetBuffer(),src);
}

void RDScene::CalFrame(const RDTime& nTime,const QString& pRDName)
{
    QMutexLocker locker(&m_lock);
    RDRenderData& RenderData = *GetRenderData(pRDName);
    RenderData.ResetDirty();
    RDSceneRenderData& pSceneData = dynamic_cast<RDSceneRenderData&>(RenderData);
    if(RenderData.GetChangeLevel() >= RDRender_GraphicChange)
    {
        if(m_BackData.m_nBackType == RDScene_Back_Picture)
        {
            RDScenePrivateData* pData = (RDScenePrivateData*)pSceneData.GetPrivateData();
            RDResource* pResource = pData->GetResource();
            RDResourceManager* pManager = RDResourceManager::GetResourceManager();
            if(pResource && pResource->GetMd5() != *m_BackData.pImage)
            {
                pManager->RemoveResource(&pData->GetResource()->GetMd5());
                pData->SetResource(NULL);
            }
            if(!pData->GetResource())
            {
                RDImageResource* pResource = dynamic_cast<RDImageResource*>(pManager->AddResource(*m_BackData.pImage));
                pData->SetResource(pResource);
            }
        }
        RenderData.UnionDirty(QRectF(0,0,pSceneData.GetNowWidth(),pSceneData.GetNowHeight()));
    }
    for(size_t i = 0; i < GetChildCount(); i++)
    {
        RDNode* pChildNode = GetChild(i);
        pChildNode ->CalFrame(nTime,pRDName);
        RenderData.UnionDirty(pChildNode->GetRenderData(pRDName)->GetDirty());
        //qDebug() << "Scene Dirty" << RenderData.GetChild(i).GetDirty();
    }
    if(RenderData.GetChangeLevel() > RDRender_NoChange)
    {
        RenderData.UnionDirty(RenderData.GetScaleBound());
    }
    QRectF newBound;
    for(size_t i = 0; i < GetChildCount(); i++)
    {
        newBound |= GetChild(i)->GetRenderData(pRDName)->GetBound();
    }
    RenderData.SetBound(newBound);


    pSceneData.SetTime(nTime);

    RenderData.SetRenderChangeLevel(RenderData.GetChangeLevel());
    RenderData.ResetChangeLevel();
}

void RDScene::CreateSceneNodeMap()
{
    m_NodeMap.clear();
    for(size_t i = 0; i < m_vecChildObj.size(); i++) 
    {
        RDNode* pChild = m_vecChildObj[i];
        m_NodeMap[pChild->GetNodeID()] = pChild;
    }
}
void RDScene::AddSceneNodeMap(const RDNode& pAddNode)
{
    m_NodeMap[pAddNode.GetNodeID()] = &pAddNode;
}
void RDScene::RemoveSceneNodeMap(const RDNode& pRemoveNode)
{
    //m_NodeMap[pRemoveNode.GetNodeID()] = NULL;
    m_NodeMap.erase(pRemoveNode.GetNodeID());
}

void RDScene::BlendChild(const QString& pRDName)
{
    RDSceneRenderData& pSceneData = *dynamic_cast<RDSceneRenderData*>(GetRenderData(pRDName));
    for(size_t i = 0; i < m_vecChildObj.size();i++)
    {
        RDNode* pNode = m_vecChildObj[i];
        RDRenderData* pRenderData = pNode->GetRenderData(pRDName);
        RDBuffer& pChildBuffer = GetChild(i)->GetRenderData(pRDName)->m_RenderBuffer;
        float3 bufferPos;
        RDSceneToBuffer(bufferPos,pRenderData->GetPos() * pSceneData.GetScale(),-pSceneData.GetNowWidth()/ 2,pSceneData.GetNowHeight()/ 2);
        QRectF dst(bufferPos.x(),bufferPos.y(),pChildBuffer.GetWidth(),pChildBuffer.GetHeight());
        QRectF src(0,0,pChildBuffer.GetWidth(),pChildBuffer.GetHeight());
        pSceneData.m_RenderBuffer.Draw(dst,pChildBuffer,src);
    }
}

bool RDScene::TriggerStory(const RDTime& nFrame,RDSceneRenderData& pSceneData)
{
    RDStory* pStory = 0;
    RDScenePrivateData* pData = (RDScenePrivateData*)pSceneData.GetPrivateData();
    if(pSceneData.IsPlay()){
        int nIndex = pData->GetCurStoryIndex();
        RDStory* pStory = GetStory(nIndex + 1);
        if(pStory)
        {
            pStory->SetStartTime(nFrame,true);
            pData->SetCurStoryIndex(nIndex + 1);
        }
    }
    else{
        RDTime nStartFrame = 0;
        int i = 0;
        std::list<RDStory*>::iterator it = m_StoryList.begin();
        for(;it != m_StoryList.end();it++)
        {
            i++;
            nStartFrame += (*it)->GetStoryLength();
            if(nStartFrame > nFrame)
            {
                pData->SetCurStoryIndex(i);
                return true;
            }
        }
    }
    return pStory != 0;
}
RDStory* RDScene::GetStory(size_t nIndex)
{
    if(nIndex >= m_StoryList.size())
        return NULL;
    std::list<RDStory*>::iterator it = m_StoryList.begin();
    for(size_t i = 0; i < nIndex; i++)
        it++;
    return *it;
}
const RDStory* RDScene::GetStory(size_t nIndex)const
{
    if(nIndex >= m_StoryList.size())
        return NULL;
    auto it = m_StoryList.cbegin();
    for(size_t i = 0; i < nIndex; i++)
        it++;
    return *it;
}
const RDStory* RDScene::GetCurStory(const RDSceneRenderData& pSceneData)const
{
    return GetStory(GetCurStoryIndex(pSceneData));
}
size_t RDScene::GetCurStoryIndex(const RDSceneRenderData& pSceneData)const
{
    const RDScenePrivateData* pData = (const RDScenePrivateData*)pSceneData.GetPrivateData();
    return pData->GetCurStoryIndex();
}

RDRenderData*  RDScene::CreateRenderData(const QString& pName)
{
    RDRenderData* pRenderData = new RDSceneRenderData(*this);
    m_vecRenderData[pName] = pRenderData;
    return pRenderData;
}

void RDScene::SetWidthHeight(int nWidth,int nHeight)
{
    for(auto it = m_vecRenderData.begin();it != m_vecRenderData.end(); it++)
    {
        RDSceneRenderData* pSceneRD = dynamic_cast<RDSceneRenderData*>(it->second);
        if(pSceneRD)
            pSceneRD->SetWidthHeight(nWidth,nHeight);
    }
}

const RDStory* RDScene::GetStory(const RDTime& nFrame,bool bPlay)const
{
	RDStory* pCurStory = NULL;
	for(auto it = m_StoryList.cbegin(); it != m_StoryList.cend(); it++)
	{
		if(pCurStory && pCurStory->GetStartTime(bPlay) > nFrame)
			break;
		pCurStory = *it;
	}
    return pCurStory;
}

RDTime RDScene::GetSceneLength()const
{
	auto it = m_StoryList.rbegin();
	RDStory* pStory = *it;
	return pStory->GetStoryLength() + pStory->GetStartTime(false);
}

void RDScene::RefreshStoryLength()
{
	RDTime nNowTime = 0;
	for(auto it = m_StoryList.begin();it != m_StoryList.end(); it++)
	{
		RDStory* pStory = *it;
		RDTime nOldLen = pStory->GetStoryLength();
		RDTime nNewLen = max(nOldLen,GetSectionMaxLength(pStory->GetStoryId()));
		pStory->SetStoryLength(nNewLen);
		pStory->SetStartTime(nNowTime,false);
		nNowTime += nNewLen;
	}
}

void RDScene::AddChild(RDNode& pChild)
{
    try{
        RDLayer& pLayer = dynamic_cast<RDLayer&>(pChild);
        RDNode::AddChild(pLayer);
    }catch(bad_cast)
    {
        return;
    }
}

RDFileDataStream& operator << (RDFileDataStream& buffer,const RDScene& scene)
{
    QMutexLocker locker(&scene.m_lock);
	qDebug() <<"begin to save scene" ;
    buffer << *(RDNode*)&scene;
    int nVersion = 0;
    buffer << nVersion;
    buffer.writeRawData( (char*)&scene.m_BackData.m_nBackType,sizeof(scene.m_BackData.m_nBackType));
    switch(scene.m_BackData.m_nBackType)
    {
    case RDScene_Back_Color:
        buffer.writeRawData( (char*)&scene.m_BackData.backColor,sizeof(scene.m_BackData.backColor));
        //buffer << scene.m_BackData.backColor;
        break;
    case RDScene_Back_Picture:
        {
            buffer.writeRawData( (char*)scene.m_BackData.pImage,sizeof(RDMd5));
            RDImageResource* pResource = dynamic_cast<RDImageResource*>(RDResourceManager::GetResourceManager()->AddResource(*scene.m_BackData.pImage));
            if(pResource)
            {
                buffer.SaveResource(pResource->GetPath());
                RDResourceManager::GetResourceManager()->RemoveResource(scene.m_BackData.pImage);
            }
        }
        break;
    }
    //story list
    int nCount = (int)scene.m_StoryList.size();
    buffer << nCount;
    for(auto it = scene.m_StoryList.begin(); it != scene.m_StoryList.end(); it++)
    {
        RDStory* pStory = *it;
        buffer << *pStory;
    }
	qDebug() <<"end to save scene" ;
    return buffer;
}

RDFileDataStream& operator >> (RDFileDataStream& buffer,RDScene& scene)
{
    QMutexLocker locker(&scene.m_lock);
    buffer >> *(RDNode*)&scene;
    int nVersion = 0;
    buffer >> nVersion ;
    buffer.readRawData( (char*)&scene.m_BackData.m_nBackType,sizeof(scene.m_BackData.m_nBackType));
    switch(scene.m_BackData.m_nBackType)
    {
    case RDScene_Back_Color:
        buffer.readRawData( (char*)&scene.m_BackData.backColor,sizeof(scene.m_BackData.backColor));
        //buffer >> scene.m_BackData.backColor;
        break;
    case RDScene_Back_Picture:
        if(!scene.m_BackData.pImage)
            scene.m_BackData.pImage = new RDMd5;
        buffer.readRawData( (char*)scene.m_BackData.pImage,sizeof(RDMd5));
        break;
    }
    //story list
    int nCount = 0;
    buffer >> nCount;
    for(int i = 0 ; i < nCount; i++)
    {
        RDStory* pStory = new RDStory;
        buffer >> *pStory;
        scene.m_StoryList.push_back(pStory);
    }
    return buffer;
}
