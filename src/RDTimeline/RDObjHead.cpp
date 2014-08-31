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
#include <QMouseEvent>

RDObjHead::RDObjHead(RDNode& pNode,QWidget* parent)
	:QWidget(parent)
     ,m_strName(pNode.GetName())
	,m_NodeUuid(pNode.GetNodeID())
	 ,m_pNode(&pNode)
{
    setMinimumHeight(RDTRACK_HEIGTH );
    setMaximumHeight(RDTRACK_HEIGTH);
	m_nSpace = CalSpaceCount(pNode);
	
    setAutoFillBackground(true);
    updateVisible();
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

    int nSpaceDis = 10;
    int nTexYPos = RDTRACK_HEIGTH - 8;
    if(m_pNode->GetChildCount() > 0)
    {
        if(m_pNode->collapse())
            p.drawText(QPoint(m_nSpace * nSpaceDis,nTexYPos),QString("+"));
        else
            p.drawText(QPoint(m_nSpace * nSpaceDis,nTexYPos),QString("-"));
    }

    p.drawText(QPoint(m_nSpace * nSpaceDis + 10,nTexYPos),QString(m_strName.c_str()));
    QWidget::paintEvent(e);
}

void RDObjHead::mousePressEvent ( QMouseEvent * event ) 
{
    if(m_pNode->GetChildCount() == 0)
        return;
    if(event->button() == Qt::LeftButton)
    {
        int nSpaceDis = 10;
        QRect rt(m_nSpace * nSpaceDis,1,9,RDTRACK_HEIGTH - 2);
        if(rt.contains(event->pos()))
        {
            m_pNode->setCollapse(!m_pNode->collapse());
            emit collapseNode(*m_pNode);
            event->accept();
            update();
        }
    }
}

void RDObjHead::updateVisible()
{
    setVisible(!m_pNode->isParentCollapse());
}
