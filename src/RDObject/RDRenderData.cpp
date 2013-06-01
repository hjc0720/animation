// =====================================================================================
// 
//       Filename:  RDRenderData.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/11 20:37:23
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDRenderData.h"
#include "RDObject.h"
#include "RDSceneRenderData.h"
#include "RDNode.h"
#include "mac_define.h"
#include "RDScene.h"

RDRenderData::RDRenderData(RDNode& node,const RDSceneRenderData& SceneRenderData)
     :m_RenderBuffer(0,0)
     ,m_ItemBuffer(0,0)
     ,m_nChangeLevel(RDRender_New)
     ,m_nRenderChangeLevel(RDRender_New)
     ,m_Node(node)
     ,m_pPrivateData(0)
      ,m_pCurSection(0)
      ,m_SceneRenderData(SceneRenderData)
      ,m_vScale(1,1,1)
{
    if(m_Node.GetObject())
        m_Node.GetObject()->CreateRenderData(*this);
    m_pParent = NULL;
}

RDRenderData::~RDRenderData()
{
    if(m_Node.GetObject())
        m_Node.GetObject()->ReleaseRenderData(*this);
    SAFE_DELETE(m_pPrivateData);
}
float RDRenderData::GetScale()const
{ 
    return m_SceneRenderData.GetScale();
}
void RDRenderData::SetChangeLevel(RDRenderChangeLevel nLevel)
{
    Lock();
    m_nChangeLevel = nLevel > m_nChangeLevel ? nLevel : m_nChangeLevel;
    UnLock();
}

void RDRenderData::SetRenderChangeLevel(RDRenderChangeLevel nLevel)
{
    m_nRenderChangeLevel = nLevel > m_nRenderChangeLevel ? nLevel : m_nRenderChangeLevel;
}

void RDRenderData::SetScale(float )
{
    SetChangeLevel(RDRender_GraphicChange);
}
const RDObject* RDRenderData::GetObject()const
{ 
    return m_Node.GetObject();
}
RDObject* RDRenderData::GetObject()
{ 
    return m_Node.GetObject();
}
void RDRenderData::Lock()const
{
    m_Node.Lock();
}
void RDRenderData::UnLock()const
{
    m_Node.UnLock();
}
bool RDRenderData::IsPlay()const
{
    return m_SceneRenderData.IsPlay();
}
const RDTime& RDRenderData::GetTime()const
{
    return m_SceneRenderData.GetTime() ;
}
const RDStory* RDRenderData::GetCurStory()const
{
    const RDScene& pScene = dynamic_cast<const RDScene&>(m_SceneRenderData.GetNode());
    return pScene.GetCurStory(m_SceneRenderData);
}
uint RDRenderData::GetSceneWidth()
{
    return m_SceneRenderData.GetWidth();
}
uint RDRenderData::GetSceneHeight()
{
    return m_SceneRenderData.GetHeight();
}
float RDRenderData::GetSceneNowWidth()
{
    return m_SceneRenderData.GetNowWidth();
}
float RDRenderData::GetSceneNowHeight()
{
    return m_SceneRenderData.GetNowHeight();
}
