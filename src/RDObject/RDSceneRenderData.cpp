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
    SetChangeLevel(RDRender_GraphicChange);
    UnLock();
}
