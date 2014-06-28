// =====================================================================================
// 
//       Filename:  RDObjHead.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/12/11 19:44:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDObjHead.h"
#include <QHBoxLayout>
#include "RDNode.h"
#include <QLabel>
#include <QDebug>
#include <QPainter>

RDObjHead::RDObjHead(RDNode& pNode,QWidget* parent)
	:QWidget(parent)
	,m_NodeUuid(pNode.GetNodeID())
	 ,m_pNode(&pNode)
{
	static const int nSpace = 10;
	m_pObjName = new QLabel(pNode.GetName(),this);
    setMinimumHeight(RDTRACK_HEIGTH );
    setMaximumHeight(RDTRACK_HEIGTH);
	int nSpaceCount = CalSpaceCount(pNode);
	
	QHBoxLayout* pLayout = new QHBoxLayout;
	pLayout->addSpacing(nSpace * nSpaceCount);
	pLayout->addWidget(m_pObjName);
	pLayout->setContentsMargins(0,0,0,0);
	setLayout(pLayout);
    setAutoFillBackground(true);
}

int RDObjHead::CalSpaceCount(const RDNode& pNode)
{
	if(pNode.GetParent())
		return 1 + CalSpaceCount(*pNode.GetParent());
	else
		return 0;
}

//
void RDObjHead::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    p.drawRect(rect().adjusted(0,0,-1,-1));
    QWidget::paintEvent(e);
}
