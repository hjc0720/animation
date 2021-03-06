// =====================================================================================
// 
//       Filename:  RDRenderManager.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  12/04/11 21:02:31
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include <QPointF>
#include "RDRenderManager.h"
#include "RDSceneRenderData.h"
#include "RDScene.h"
#include "RDBuffer.h"
#include <QRectF>
//#include "HVector3f.h"

bool RenderManager::RenderScene(const QPointF& ,const QRectF& ,RDTime nTime)
{
    if( m_RenderName.empty())
        return false;
    RDRenderData* pSceneRD = m_pScene->GetRenderData(m_RenderName);
    pSceneRD->setRenderManager(this);
    if(m_pScene->GetMaxChangeLevel(m_RenderName) == RDRender_NoChange && nTime == pSceneRD->GetTime())
        return false;
    m_pScene->CalFrame(nTime,m_RenderName);
    m_pScene->Render(nTime,m_RenderName);
    return true;
}

void RenderManager::RegistCallback(RDRenderCallbackType eType, RenderManagerCallback &pCallback)
{
    m_Callback[eType] = pCallback;
}

void RenderManager::Call(RDRenderCallbackType eType, RDNode *pLayer)
{
    auto it = m_Callback.find(eType) ;
    if(it == m_Callback.end())
        return;
    it->second(pLayer,this);
}
