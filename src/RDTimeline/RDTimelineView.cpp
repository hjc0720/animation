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
#include <QRect>
#include <QPaintEvent>
#include <QPainter>

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
    m_pHead->setMaximumWidth(250);

    QVBoxLayout* pLeft = new QVBoxLayout();
    pLeft->setContentsMargins(0,0,0,0);
    pLeft->setSpacing(0);
    //    pLeft->addSpacing(20);
    pLeft->addWidget(m_pHead);


    m_pSectionView = new RDSectionView(&pScene,this);
    QVBoxLayout* pRight = new QVBoxLayout();
    pRight->setContentsMargins(0,0,0,0);
    pRight->setSpacing(0);
    //pRight->addSpacing(20);
    pRight->addWidget(m_pSectionView);

    pTimeLineHLayout->addLayout(pLeft);
    pTimeLineHLayout->addLayout(pRight);

    setWidget(pTimeLineWidget);
    connect(m_pSectionView,SIGNAL(FrameChanged(const RDTime&)),this,SIGNAL(FrameChanged(const RDTime&)));
    connect(m_pSectionView,SIGNAL(SectionChanged()),this,SIGNAL(SectionChanged()));
}

void RDTimelineView::RDFillHead(RDNode& pNode)
{
    InsertObj(pNode,-1);
	for(size_t i = 0; i < pNode.GetChildCount();i++)
	{
		RDFillHead(*pNode.GetChild(i));
	}
}

void RDTimelineView::UpdateBackground(int nStartIndex)
{
    bool bDark = (nStartIndex + 1) % 2;
    for(int i = nStartIndex; i < m_pHeadLayout->count(); i++)
    {
        QLayoutItem* item = m_pHeadLayout->itemAt(i);
        RDObjHead* pHead = dynamic_cast<RDObjHead* >(item->widget());
        if(pHead)
        {
            pHead->SetBackGround(bDark);
            bDark = !bDark;
        }
    } 
}

void RDTimelineView::InsertObj(RDNode& pNewNode)
{
    int nIndex = GetHeadIndex(pNewNode);
    InsertObj(pNewNode,nIndex);
    m_pSectionView->SetSceneNode(m_pScene);
    UpdateBackground(nIndex + 1);
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
	int nIndex = GetHeadIndex(pNewNode);
    int nTotalChildCount = pNewNode.GetTotalChildCount();

    auto pDelObj = [](RDObjHead* pHead){delete pHead;};

    for_each(m_objHead.begin() + nIndex ,m_objHead.begin() + nIndex + nTotalChildCount,pDelObj);
    m_objHead.erase(m_objHead.begin() + nIndex ,m_objHead.begin() + nIndex + nTotalChildCount + 1);

    m_pSectionView->SetSceneNode(m_pScene);
    UpdateBackground(nIndex );
}

void RDTimelineView::collapseNode(RDNode& pNode)
{
    int nTotalChildCount = pNode.GetTotalChildCount();
	int nIndex = GetHeadIndex(pNode);
    auto pSetVisible = [](RDObjHead* pHead){pHead->updateVisible();};
    for_each(m_objHead.begin() + nIndex + 1,m_objHead.begin() + nIndex + nTotalChildCount + 1,pSetVisible);

    m_pSectionView->SetSceneNode(m_pScene);
    UpdateBackground(nIndex + 1);
}

void RDTimelineView::InsertObj(RDNode& pNewNode,int nIndex)
{
    RDObjHead* pHead = new RDObjHead(pNewNode,this);
    connect(pHead, SIGNAL(collapseNode(RDNode&)), this, SLOT(collapseNode(RDNode&)));

    int nRealCount = nIndex >= 0 ? nIndex : m_objHead.size();
    pHead->SetBackGround((nRealCount + 1) % 2);

    if(nIndex >= 0)
    {
        m_pHeadLayout->insertWidget(nIndex,pHead);
        m_objHead.insert(m_objHead.begin() + nIndex,pHead);
    }
    else
    {
        m_pHeadLayout->addWidget(pHead);
        m_objHead.push_back(pHead);
    }
}
