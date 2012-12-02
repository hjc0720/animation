// =====================================================================================
// 
//       Filename:  RDTimeline.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  30/11/11 21:22:53
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDTimelineView.h"
#include "RDObjHead.h"
#include <QVBoxLayout>
#include "RDScene.h"
#include "RDNode.h"
#include <QScrollArea>
#include "RDTimeRuler.h"
#include <QDebug>
#include "RDScetionView.h"

RDTimelineView::RDTimelineView(RDScene& pScene,QWidget* pParent)
	:QDockWidget(pParent)
	 ,m_pScene(&pScene)
{
    setFeatures(0/*QDockWidget::DockWidgetFloatable*/);
	QWidget* pTimeLineWidget = new QWidget(this);

	QHBoxLayout* pTimeLineHLayout = new QHBoxLayout(pTimeLineWidget);
	pTimeLineHLayout->setContentsMargins(0,0,0,0);
    pTimeLineHLayout->setSpacing(0);
	pTimeLineWidget->setLayout(pTimeLineHLayout);

	m_pHead = new QScrollArea(pTimeLineWidget);
	pTimeLineHLayout->addWidget(m_pHead);
	QWidget* pHeadWidget = new QWidget(m_pHead);
	m_pHeadLayout = new QVBoxLayout(pHeadWidget);
	m_pHeadLayout->setContentsMargins(0,0,0,0);
	m_pHeadLayout->setSpacing(0);
	pHeadWidget->setLayout(m_pHeadLayout);
	RDFillHead(pScene);
	m_pHeadLayout->addStretch();
    m_pHead->setWidgetResizable(true);
	m_pHead->setWidget(pHeadWidget);

    QVBoxLayout* pLeft = new QVBoxLayout();
	pLeft->setContentsMargins(0,0,0,0);
    pLeft->setSpacing(0);
//    pLeft->addSpacing(20);
    pLeft->addWidget(m_pHead);


	//m_pRuler = new RDTimeRuler(10000000,this);
    m_pSectionView = new RDSectionView(&pScene,this);
    QVBoxLayout* pRight = new QVBoxLayout();
	pRight->setContentsMargins(0,0,0,0);
    pRight->setSpacing(0);
//    pRight->addWidget(m_pRuler);
    //pRight->addSpacing(20);
    pRight->addWidget(m_pSectionView);

	pTimeLineHLayout->addLayout(pLeft);
	pTimeLineHLayout->addLayout(pRight);

	setWidget(pTimeLineWidget);
    connect(m_pSectionView,SIGNAL(FrameChanged(const RDTime&)),this,SIGNAL(FrameChanged(const RDTime&)));
}

void RDTimelineView::RDFillHead(RDNode& pNode)
{
	RDObjHead* pHead = new RDObjHead(pNode,this);
	m_pHeadLayout->addWidget(pHead);
	for(size_t i = 0; i < pNode.GetChildCount();i++)
	{
		RDFillHead(*pNode.GetChild(i));
	}
}

void RDTimelineView::InsertObj(RDNode& pNewNode)
{
	int nIndex = GetHeadIndex(pNewNode);
	RDObjHead* pHead = new RDObjHead(pNewNode,this);
	m_pHeadLayout->insertWidget(nIndex,pHead);
    m_pSectionView->SetSceneNode(m_pScene);
}

int RDTimelineView::GetHeadIndex(const RDNode& pNode)
{
	const RDNode* pParent = pNode.GetParent();
	int nIndex = 0;
	if(!pParent)
		return 0;
	nIndex += GetHeadIndex(*pParent);
	for(size_t i = 0;i < pParent->GetChildCount();i++)
	{
		if(pParent->GetChild(i) == &pNode)
			return nIndex + 1;
		nIndex += pParent->GetChild(i)->GetTotalChildCount() + 1;
	}
	return nIndex;
}
void RDTimelineView::DelObj(RDNode& pNewNode)
{
	for(int i = pNewNode.GetChildCount() - 1; i >=0; i++)
	{
		DelObj(*pNewNode.GetChild(i));
	}	
	int nIndex = GetHeadIndex(pNewNode);
	QLayoutItem* item = m_pHeadLayout->takeAt(nIndex);
	if(item)
	{
        QWidget *wid = item->widget();
        delete item;
        if (wid) wid->deleteLater();
	}
}
