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
#include <QDebug>
#include "RDScetionView.h"
#include <QRect>
#include <QPaintEvent>
#include <QPainter>
#include "mac_define.h"
#include <QLabel>
#include <QComboBox>
#include "RDStory.h"
#include <QPushButton>
#include <QScrollBar>

RDTimelineView::RDTimelineView(RDScene& pScene,QWidget* pParent)
	:QDockWidget(pParent)
	 ,m_pScene(&pScene)
{
    setFeatures(0/*QDockWidget::DockWidgetFloatable*/);
    QWidget* pTimeLineWidget = new QWidget(this);

	QVBoxLayout* pTimeLineVLayout =  new QVBoxLayout;
    pTimeLineWidget->setLayout(pTimeLineVLayout);

	pTimeLineVLayout->addLayout(AddToolBar());

    QHBoxLayout* pTimeLineHLayout = new QHBoxLayout(pTimeLineWidget);
    pTimeLineHLayout->setContentsMargins(0,0,0,0);
    pTimeLineHLayout->setSpacing(0);
	pTimeLineVLayout->addLayout(pTimeLineHLayout);

    m_pHead = new QScrollArea(pTimeLineWidget);
    m_pHead->setWidgetResizable(true);
    m_pHead->setMaximumWidth(250);
	m_pHead->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget* pHeadWidget = new QWidget(m_pHead);
    m_pHead->setWidget(pHeadWidget);

    m_pHeadLayout = new QVBoxLayout(pHeadWidget);
    m_pHeadLayout->setContentsMargins(0,0,0,0);
    m_pHeadLayout->setSpacing(0);
    pHeadWidget->setLayout(m_pHeadLayout);

    RDFillHead(pScene);
    m_pHeadLayout->addStretch();

    m_pSectionView = new RDSectionView(&pScene,&pScene.GetCurStory(DEFAULT_RD), this);
	m_pSectionView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    pTimeLineHLayout->addWidget(m_pHead);
    pTimeLineHLayout->addWidget(m_pSectionView);

    setWidget(pTimeLineWidget);
    connect(m_pSectionView,SIGNAL(FrameChanged(const RDTime&)),this,SIGNAL(FrameChanged(const RDTime&)));
    connect(m_pSectionView,SIGNAL(SectionChanged()),this,SIGNAL(SectionChanged()));

	QScrollBar* pLeftScroll = m_pHead->verticalScrollBar();
	QScrollBar* pRightScroll = m_pSectionView->verticalScrollBar();
    connect(pLeftScroll,SIGNAL(sliderMoved(int)),pRightScroll,SLOT(setValue(int)));
    connect(pRightScroll,SIGNAL(sliderMoved(int)),pLeftScroll,SLOT(setValue(int)));
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
	qDebug() << "RDTimelineView::DelObj";
    int nTotalChildCount = pNewNode.GetTotalChildCount();

	auto it = std::find_if(m_objHead.begin(),m_objHead.end(),[&pNewNode](RDObjHead* pHead)->bool{return pHead->isEqual(&pNewNode);});
	if(it == m_objHead.end())
		return;
	int nIndex = std::distance(m_objHead.begin(),it);

	for(int i = nIndex + nTotalChildCount; i >= nIndex; i--)
	{
		qDebug() << "RDTimelineView::DelObj:" << i;
		QLayoutItem* item = m_pHeadLayout->takeAt(i);
		if(item)
		{
			QWidget* pWidget = item->widget();
			if(pWidget)
				pWidget->deleteLater();
			delete item;
		}
	}

    m_objHead.erase(m_objHead.begin() + nIndex ,m_objHead.begin() + nIndex + nTotalChildCount + 1);
	m_pSectionView->DelNode(&pNewNode);
	//TODO: m_pSectionView support delete one Node
    //m_pSectionView->SetSceneNode(m_pScene);
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

QHBoxLayout*  RDTimelineView::AddToolBar()
{
    QHBoxLayout* pToolBar = new QHBoxLayout();
	pToolBar->addWidget(new QLabel(tr("Story List")));

//add story button
	QPushButton* pAddButton = new QPushButton("+");
	connect(pAddButton,SIGNAL(clicked()),this,SIGNAL(addStory()));
	pToolBar->addWidget(pAddButton);

	//story list
	m_pStoryList = new QComboBox;
	updateStory();
	connect(m_pStoryList,SIGNAL(activated(int)),this,SIGNAL(switchStory(int)));
	pToolBar->addWidget(m_pStoryList);

//delete story button
	QPushButton* pRemoveButton = new QPushButton("-");
	connect(pRemoveButton,SIGNAL(clicked()),this,SIGNAL(removeCurStory()));
	pToolBar->addWidget(pRemoveButton);

	pToolBar->addStretch();
	return pToolBar;
}

void RDTimelineView::updateStory()
{
	m_pStoryList->clear();

	for(size_t i = 0; i < m_pScene->GetStoryCount(); i++)
	{
		auto pStory = m_pScene->GetStory(i);
		m_pStoryList->addItem(QString(pStory->GetStroyName().c_str()));
	}
	trigStory(m_pScene->GetCurStoryIndex(DEFAULT_RD));
}

void RDTimelineView::trigStory(size_t nIndex)
{
	m_pStoryList->setCurrentIndex(nIndex);
	if(m_pSectionView)
		m_pSectionView->SetSceneNode(m_pScene);
}

void RDTimelineView::updateStoryLength()
{
	m_pSectionView->UpdateStoryLength();
}
