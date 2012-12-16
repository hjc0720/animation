// =====================================================================================
// 
//       Filename:  RDScetionView.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/11/2012 05:52:40 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
//
#include "RDScetionView.h"
#include <QGraphicsScene>
#include <algorithm>
#include <QResizeEvent>
#include "mac_define.h"
#include "RDScene.h"
#include "RDStory.h"
#include "RDSection.h"
#include <QDebug>
#include "RDSectionItem.h"
#include "RDStoryItem.h"
#include "RDTimeMarker.h"

RDSectionView::RDSectionView(RDScene* pScene,QWidget* pWidget )
    :QGraphicsView(pWidget)
    ,m_nScale(1)
{
    SetSceneNode(pScene);
    SetScale(m_pScene->GetSceneLength() / 1920);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void RDSectionView::resizeEvent(QResizeEvent* event)
{
    RDSectionScene* pScene = dynamic_cast<RDSectionScene*>(scene());
    if(pScene && pScene->GetTimeMarker())
        pScene->GetTimeMarker()->SetHeight(height());
    QGraphicsView::resizeEvent(event);
}

void RDSectionView::wheelEvent(QWheelEvent *event)
{
    int nScale = m_nScale;
    if(event->delta() > 0)
        nScale *= (event->delta() / 8);
    else
        nScale /= -(event->delta() / 8);
    nScale = std::max(nScale,MSECOND_TO_TIME / 100);
    qDebug() << "scale" << nScale;
    SetScale(nScale);
    event->accept();
}

void RDSectionView::SetSceneNode(RDScene* pScene)
{
    m_pScene = pScene;
    qreal fWidth = m_pScene->GetSceneLength() ;/// m_nScale;
    qreal fHeight = (m_pScene->GetTotalChildCount() + 1) * RDTRACK_HEIGTH;
    QGraphicsScene* pOldScene = scene();
    SAFE_DELETE(pOldScene);

    RDSectionScene* pGraphicScene = new RDSectionScene(0,0,fWidth,fHeight);
    connect(pGraphicScene,SIGNAL(FrameChanged(const RDTime&)),this,SIGNAL(FrameChanged(const RDTime&)));
    setScene(pGraphicScene);

    size_t nStoryCount = pScene->GetStoryCount();
    for(size_t i = 0; i < nStoryCount; i++)
    {
        int nIndex = 1;
        const RDStory* pStory = pScene->GetStory(i);
        AddChildNodeSection(nIndex,pScene,pStory->GetStoryId());

        RDStoryItem* pItem = new RDStoryItem(pStory,RDTRACK_HEIGTH);
        scene()->addItem(pItem);
    }

    RDTimeMarker* pTimeMarker = new RDTimeMarker(RDTRACK_HEIGTH,height(),1.0 / m_nScale);
    pGraphicScene->SetTimeMarker(pTimeMarker);
    scene()->addItem(pTimeMarker);
}

void RDSectionView::AddChildNodeSection(int& nIndex,RDNode* pNode,const QUuid& idStory)
{
    size_t nChildCount = pNode->GetChildCount();
    for(size_t  i = 0; i < nChildCount; i++)
    {
        RDNode* pChildNode = pNode->GetChild(i);
        for(size_t j = 0; j < pChildNode->GetSectionCount(idStory); j++)
        {
            const RDSection* pSection = pChildNode->GetSection(idStory,j);
            RDSectionItem* pItem = new RDSectionItem(pSection,RDTRACK_HEIGTH,0,nIndex * RDTRACK_HEIGTH);
            scene()->addItem(pItem);
        }
        nIndex++;
        AddChildNodeSection(nIndex,pChildNode,idStory);
    }
}

void RDSectionView::SetScale(const RDTime& nScale) 
{
    scale(m_nScale / (float)nScale ,1);
    m_nScale = nScale;
    RDSectionScene* pScene = dynamic_cast<RDSectionScene*>(scene());
    if(pScene && pScene->GetTimeMarker())
        pScene->GetTimeMarker()->SetScale(1.0 / m_nScale);
}
