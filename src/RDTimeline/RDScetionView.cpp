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
#include "RDSectionScene.h"

RDSectionView::RDSectionView(RDScene* pScene,const RDStory* pStory,QWidget* pWidget )
    :QGraphicsView(pWidget)
     ,m_pStory(pStory)
    ,m_nScale(1)
{
    SetSceneNode(pScene);
    SetScale(m_pStory->GetStoryLength() / 1920);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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
    qreal fWidth = m_pStory->GetStoryLength() ;/// m_nScale;
    qreal fHeight = (m_pScene->GetTotalChildCount() + 1) * RDTRACK_HEIGTH;
    QGraphicsScene* pOldScene = scene();
    SAFE_DELETE(pOldScene);

    RDSectionScene* pGraphicScene = new RDSectionScene(0,0,fWidth,fHeight);
    connect(pGraphicScene,SIGNAL(FrameChanged(const RDTime&)),this,SIGNAL(FrameChanged(const RDTime&)));
    connect(pGraphicScene,SIGNAL(SectionChanged()),this,SIGNAL(SectionChanged()));
    setScene(pGraphicScene);

	int nIndex = 1;
	const RDStory* pStory = &pScene->GetCurStory(DEFAULT_RD);
	AddChildNodeSection(nIndex,pScene,pStory->GetStoryId());

	RDStoryItem* pItem = new RDStoryItem(pStory,RDTRACK_HEIGTH);
	scene()->addItem(pItem);

    RDTimeMarker* pTimeMarker = new RDTimeMarker(RDTRACK_HEIGTH,height(),1.0 / m_nScale);
    pGraphicScene->SetTimeMarker(pTimeMarker);
    scene()->addItem(pTimeMarker);
}

void RDSectionView::AddChildNodeSection(int& nIndex,RDNode* pNode,const QUuid& idStory)
{
    if(pNode->collapse())
        return;
    size_t nChildCount = pNode->GetChildCount();
    for(size_t  i = 0; i < nChildCount; i++)
    {
        RDNode* pChildNode = pNode->GetChild(i);
        for(size_t j = 0; j < pChildNode->GetSectionCount(idStory); j++)
        {
            RDSection* pSection = pChildNode->GetSection(idStory,j);
            RDSectionItem* pItem = new RDSectionItem(pChildNode,pSection,RDTRACK_HEIGTH,nIndex * RDTRACK_HEIGTH);
            //qDebug() << "cur section" << (size_t)pItem <<":"<< nIndex << pNode->GetName();
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

void RDSectionView::DelNode(RDNode* pNode)
{
	if(pNode == nullptr)
		return;

    RDSectionScene* pScene = dynamic_cast<RDSectionScene*>(scene());
	pScene->delNode(pNode);
}
