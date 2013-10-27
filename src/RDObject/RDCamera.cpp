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
        matrix4x4::CreateViewMat(m_matView,GetPos(),m_vUp,m_vLookAt);
    }
    void CalProjectMat(QRectF& rt,bool bEditProj)
    {
        uint nHeight = GetSceneHeight();

        m_fZero = nHeight / m_vScale.x(); 

        QRectF rtNear(rt.topLeft() * m_fZNear / m_fZero,rt.bottomRight() * m_fZNear / m_fZero);
        matrix4x4::CreateProjectMat(bEditProj?m_matEditProj:m_matRenderProj,rtNear.left(),rtNear.right(),-rtNear.top(),-rtNear.bottom(),m_fZNear,m_fZFar);
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

    matrix4x4  m_matView;
    matrix4x4  m_matRenderProj;
    matrix4x4  m_matEditProj;
    matrix4x4  m_matEditViewProj;
    matrix4x4  m_matRenderViewProj;
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
    const RDScene* pScene = GetSceneNode();
    RDCameraRenderData* pRenderData = new RDCameraRenderData(*this,*dynamic_cast<const RDSceneRenderData*>(pScene->GetRenderData(pName)));
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
    pRenderData->m_fZNear = fabs(fZNear);
    pRenderData->m_fZFar = fabs(fFar);
    pRenderData->CalProjectMat(rt,false);
    QRectF sceneRT(pRenderData->GetSceneWidth()/(-2.f),pRenderData->GetSceneHeight()/(-2.f),
                   pRenderData->GetSceneWidth(),pRenderData->GetSceneHeight());
    pRenderData->CalProjectMat(sceneRT,true);
    pRenderData->UpdateMatrix();
}

const matrix4x4&    RDCamera::GetViewMatrix(const QString& pRDName)
{
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    return pRenderData->m_matView;
}

const matrix4x4 &RDCamera::GetRenderProjMatrix(const QString &pRDName)
{
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    return pRenderData->m_matRenderProj;
}

const matrix4x4 &RDCamera::GetEditProjMatrix(const QString &pRDName)
{
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    return pRenderData->m_matEditProj;
}

const matrix4x4&    RDCamera::GetViewProjMat(const QString& pRDName)
{
    RDCameraRenderData* pRenderData = dynamic_cast<RDCameraRenderData*>( GetRenderData(pRDName));
    return pRenderData->m_matRenderViewProj;
}
