// =====================================================================================
// 
//       Filename:  RDSceneRenderData.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/11 21:11:17
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDSceneRenderData.h"
#include "RDScene.h"
#include <string>

RDSceneRenderData::RDSceneRenderData(const std::string& name,RDNode& obj)
    :RDRenderData(name,obj,*this)
     ,m_nWidth(0)
     ,m_nHeight(0)
     ,m_fScale(1)
     ,m_nCurFrame(-1)
{
    RDScene& scene = dynamic_cast<RDScene&>(obj);
    scene.CreateRenderData(*this);
    m_bPlay = false;
    m_nCurStoryIndex = 0;
}

RDSceneRenderData::RDSceneRenderData(const std::string& name,int nWidth,int nHeight,RDNode& obj)
    :RDRenderData(name,obj,*this)
     ,m_nWidth(nWidth)
     ,m_nHeight(nHeight)
     ,m_fScale(1)
     ,m_nCurFrame(-1)
{
    RDScene& scene = dynamic_cast<RDScene&>(obj);
    scene.CreateRenderData(*this);
}

void RDSceneRenderData::SetSceneScale(float fScale)
{
    if(floatIsEqualNear(fScale,m_fScale))
        return;
    Lock();
    m_fScale = fScale;
    setChangeLevel(RDRender_GraphicChange);
    UnLock();
}

void RDSceneRenderData::trigStory(const RDStory& story,RDTime nFrame)
{
	m_trigStory.push_front(story);
	m_trigStory.front().SetStartTime(nFrame);
}

const RDStory& RDSceneRenderData::GetStory(RDTime nTime)const
{
	for(auto it = m_trigStory.begin(); it != m_trigStory.end(); it++)
	{
		if(it->GetStartTime() < nTime)
			return *it;
	}
	return m_trigStory.back();
}

void RDSceneRenderData::removeTrigStory(const QUuid& storyID)
{
	m_trigStory.remove_if([&](const RDStory& pStory)->bool{ return storyID == pStory.GetStoryId();});
}
