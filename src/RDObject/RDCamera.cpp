/*
 * =====================================================================================
 *
 *       Filename:  RDCamera.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/25/13 22:48:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "RDCamera.h"
#include "HMatrixQ4F.h"
#include "RDScene.h"
#include "RDSceneRenderData.h"
#include "RDStory.h"
#include "RDSection.h"

class RDCameraRenderData :public RDRenderData
{
public:
    RDCameraRenderData(RDNode& node,const RDSceneRenderData& SceneRenderData)
        :RDRenderData(node,SceneRenderData)
    { }
    void CalViewMat()
    {
        HMatrixQ4F::CreateViewMat(m_matView,GetPos(),m_vUp,m_vLookAt);
    }
    void CalProjectMat(QRectF& rt,bool bEditProj)
    {
        uint nHeight = GetSceneHeight();

        m_fZero = nHeight / m_vScale.x(); 

        QRectF rtNear(rt.topLeft() * m_fZNear / m_fZero,rt.bottomRight() * m_fZNear / m_fZero);
        HMatrixQ4F::CreateProjectMat(bEditProj?m_matEditProj:m_matRenderProj,rtNear.left(),rtNear.right(),rtNear.top(),rtNear.bottom(),m_fZNear,m_fZFar);
    }
    void UpdateMatrix()
    {
        m_matRenderViewProj = m_matView * m_matRenderProj;
        m_matEditViewProj = m_matView * m_matEditProj;
    }
public:
    float       m_fZNear;
    float       m_fZFar;
    float       m_fZero;

    //float3      m_vEyePos;
    float3      m_vLookAt;
    float3      m_vUp;

    HMatrixQ4F  m_matView;
    HMatrixQ4F  m_matRenderProj;
    HMatrixQ4F  m_matEditProj;
    HMatrixQ4F  m_matEditViewProj;
    HMatrixQ4F  m_matRenderViewProj;
};

RDCamera::RDCamera(const QString& strName,uint nHeight,RDProjectType nType)
    :RDNode(strName,float3(0,0,(float)nHeight),nullptr)
     ,m_nProjType(nType)
     ,m_vUp(0,1,0)
     ,m_vLookAt(0,0,0)
{
}

RDRenderData*  RDCamera::CreateRenderData(const QString& pName)
{
    QMutexLocker locker(&m_lock);
    RDScene* pScene = GetSceneNode();
    RDCameraRenderData* pRenderData = new RDCameraRenderData(*this,*dynamic_cast<RDSceneRenderData*>(pScene->GetRenderData(pName)));
    pRenderData->m_vLookAt = m_vLookAt;
    pRenderData->m_vUp = m_vUp;

    m_vecRenderData[pName] = pRenderData;
    return pRenderData;
}

void RDCamera::CalFrame(const RDTime& nTime,const QString& pRDName) 
{
    QMutexLocker locker(&m_lock);
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    UpdateSection(nTime,*pRenderData);
    if(!pRenderData->GetCurSection())
        return;
    RDTime nSectionTime = nTime - pRenderData->GetCurStory()->GetStartTime(pRenderData->IsPlay()) - pRenderData->GetCurSection()->GetStartTime();
    if(nSectionTime == pRenderData->GetSectionTime() && GetMaxChangeLevel(pRDName) == RDRender_NoChange)
        return;

    qDebug() << "cal Camera frame: section time:"<<nSectionTime;
    if(CalSpaceVector(nSectionTime,*pRenderData))
    {
        pRenderData->CalViewMat();
        SetChangeLevel(RDRender_TransChange);
    }

    pRenderData->SetRenderChangeLevel(pRenderData->GetChangeLevel());
    pRenderData->ResetChangeLevel();
    pRenderData->SetSectionTime(nSectionTime);
}

void    RDCamera::UpdateProject(const QString& pRDName,QRectF& rt,float fZNear,float fFar)
{
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    pRenderData->m_fZNear = fZNear;
    pRenderData->m_fZFar = fFar;
    pRenderData->CalProjectMat(rt,false);
    QRectF sceneRT(0,0,pRenderData->GetSceneWidth(),pRenderData->GetSceneHeight());
    pRenderData->CalProjectMat(sceneRT,true);
    pRenderData->UpdateMatrix();
}

const HMatrixQ4F&    RDCamera::GetViewMatrix(const QString& pRDName)
{
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    return pRenderData->m_matView;
}
const HMatrixQ4F&    RDCamera::GetViewProjMat(const QString& pRDName)
{
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    return pRenderData->m_matRenderViewProj;
}
