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


#define RDTRACK_HEIGTH 20

RDSectionView::RDSectionView(int nScale,RDScene* pScene,QWidget* pWidget )
    :QGraphicsView(pWidget)
	 ,m_nScale(nScale)
{
	SetSceneNode(pScene);
	scale(1.0 / m_nScale,1);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}
//void RDSectionView::resizeEvent(QResizeEvent* event)
//{
	//RDTime nSceneLength = m_pScene->GetSceneLength();
	//size_t nCount = m_pScene->GetTotalChildCount();
	//QRectF rt(0,0,nSceneLength / m_nScale,(nCount + 1) * RDTRACK_HEIGTH);
	//rt.setRight(std::max(rt.width(),(qreal)event->size().width()));
	//rt.setBottom(std::max(rt.height(),(qreal)event->size().height()));
	//setSceneRect(rt);
//}	
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
        const RDStory* pStory = pScene->GetStory(i);
        pGraphicScene->addRect(pStory->GetStartTime(false),0,pStory->GetStoryLength() ,RDTRACK_HEIGTH);
        int nIndex = 1;
        AddChildNodeSection(nIndex,pScene,pStory->GetStoryId());
    }
//	qDebug() << "story length:"<<pStory->GetStoryLength()<<m_nScale;
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
//            scene()->addRect(pSection->GetStartTime(),nIndex * RDTRACK_HEIGTH,pSection->GetLength() ,RDTRACK_HEIGTH);
        }
        nIndex++;
        AddChildNodeSection(nIndex,pChildNode,idStory);
    }
}
