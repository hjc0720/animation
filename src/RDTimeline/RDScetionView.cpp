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


#define RDTRACK_HEIGTH 20

RDSectionView::RDSectionView(RDScene* pScene,QWidget* pWidget )
    :QGraphicsView(pWidget)
     ,m_pTimeMarker(nullptr)
{
	SetSceneNode(pScene);
    m_nScale = m_pScene->GetSceneLength() / 1920;
	scale(1.0 / m_nScale,1);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void RDSectionView::resizeEvent(QResizeEvent* event)
{
    if(m_pTimeMarker)
        m_pTimeMarker->SetHeight(height());
    QGraphicsView::resizeEvent(event);
}	

void RDSectionView::SetSceneNode(RDScene* pScene)
{
	m_pScene = pScene;
	qreal fWidth = m_pScene->GetSceneLength() ;/// m_nScale;
	qreal fHeight = (m_pScene->GetTotalChildCount() + 1) * RDTRACK_HEIGTH;
	QGraphicsScene* pOldScene = scene();
	SAFE_DELETE(pOldScene);

	QGraphicsScene* pGraphicScene = new QGraphicsScene(0,0,fWidth,fHeight);
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

    m_pTimeMarker = new RDTimeMarker(RDTRACK_HEIGTH,height());
    scene()->addItem(m_pTimeMarker);
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
