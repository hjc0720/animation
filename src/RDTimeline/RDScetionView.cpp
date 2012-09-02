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
#include <QDebug>


#define RDTRACK_HEIGTH 20

RDSectionView::RDSectionView(int nScale,RDScene* pScene,QWidget* pWidget )
    :QGraphicsView(pWidget)
	 ,m_nScale(nScale)
{
	SetScene(pScene);
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
void RDSectionView::SetScene(RDScene* pScene)
{
	m_pScene = pScene;
	qreal fWidth = m_pScene->GetSceneLength() ;/// m_nScale;
	qreal fHeight = (m_pScene->GetTotalChildCount() + 1) * RDTRACK_HEIGTH;
	QGraphicsScene* pGraphicScene = new QGraphicsScene(0,0,fWidth,fHeight);
	//size_t nStoryCount = pScene->GetStoryCount();
	const RDStory* pStory = pScene->GetStory(0);
	pGraphicScene->addRect(0,0,pStory->GetStoryLength() ,RDTRACK_HEIGTH);
	qDebug() << "story length:"<<pStory->GetStoryLength()<<m_nScale;
	QGraphicsScene* pOldScene = scene();
	setScene(pGraphicScene);
	SAFE_DELETE(pOldScene);
}
