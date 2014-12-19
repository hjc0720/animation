/*
 * =====================================================================================
 *
 *       Filename:  RDSectionItem.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/04/12 17:15:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <RDSectionItem.h>
#include <QBrush>
#include "RDSection.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "RDScetionView.h"
#include "RDNode.h"
#include "RDSectionScene.h"
#include <QKeyEvent>
#include <functional>

RDSectionItem::RDSectionItem(RDNode* pNode,RDSection* pSection,int nHeight,int nYOffset)
    :m_bHitTest(false)
     ,m_nHeight(nHeight)
     ,m_nYOffset(nYOffset)
     ,m_pNode(pNode)
      ,m_pSection(pSection)
{
    SetSectionType();
    setPos(m_pSection->GetStartTime(),m_nYOffset);
    setFlags(ItemIsMovable | ItemIsSelectable);
//    setAcceptedMouseButtons(Qt::LeftButton);
}

void RDSectionItem::SetSectionType()
{
    switch(m_pSection->GetType() )
    {
    case RDSectionFinish:
        m_imgSectionType.load(":/stop");
        break;
    case RDSectionKeep:
        m_imgSectionType.load(":/keep");
        break;
    case RDSecionCycle:
        m_imgSectionType.load(":/cycle");
        break;
    }
    //qDebug() << "image initialization type" << m_pSection->GetType() ;
}

void RDSectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * , QWidget *)
{
    QLinearGradient line(0,m_nHeight / 2,0,0);
    line.setSpread(QGradient::ReflectSpread);
    line.setColorAt(0,QColor(0,0,255));
    line.setColorAt(1,QColor(255,255,255));
    QBrush brush(line);
    painter->fillRect(0,1,m_pSection->GetLength(),m_nHeight - 1,line);

	if(isSelected())
	{
		painter->setPen(QPalette::Highlight);
		painter->drawRect(0,0,m_pSection->GetLength(),m_nHeight);
	}

    qreal fScale = painter->worldTransform().m11();
    qreal fRealSize = 16 / fScale;
    //qDebug() << fRealSize << m_pSection->GetLength() / 2;
    //qDebug() << "image width" << m_imgSectionType.size();
    if(fRealSize < m_pSection->GetLength() / 2)
    {
        QRectF target(m_pSection->GetLength() - fRealSize,0,fRealSize,m_nHeight);
        painter->drawImage(target,m_imgSectionType);
    }

	std::set<RDTime>  keyTimeSet;
	for(int i = RDSectionPos; i < RDSectionCount;i++)
	{
		const RDKeyList<float3>& list = m_pSection->getKeyList(static_cast<RDSectionKeyType>(i));
		list.GetTime(keyTimeSet);
	}
	
    painter->setPen(Qt::yellow);
	using namespace std::placeholders;
	std::for_each(keyTimeSet.begin(),keyTimeSet.end(),std::bind(std::mem_fn(&RDSectionItem::DrawKey),this,_1,fScale,painter));
}

void RDSectionItem::DrawKey(RDTime time,float fScale,QPainter *painter)
{
	const float value = 6;
	float width = (m_nHeight - value * 2) / 2 / fScale;
	QPointF pt[4];

	pt[0].setX(time);
	pt[0].setY(value);

	pt[1].setX(time - width);
	pt[1].setY(m_nHeight / 2.f );

	pt[2].setX(time);
	pt[2].setY(m_nHeight - value);

	pt[3].setX(time + width);
	pt[3].setY(m_nHeight / 2.f);

	qDebug() << pt[0] << pt[1] << pt[2] << pt[3];
	painter->drawPolygon(pt,4);
}


QRectF RDSectionItem::boundingRect()const
{
    return QRectF(0,0,m_pSection->GetLength(),m_nHeight);
}

void RDSectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    RDTime offsetPos = event->scenePos().x() - event->lastScenePos().x();
    event->accept();
    update();
    m_pSection->MovSection(offsetPos);
    m_pNode->SetChangeLevel(RDRender_TransChange);
	setPos(m_pSection->GetStartTime(),m_nYOffset);
    RDSectionScene* pScene = dynamic_cast<RDSectionScene*>(scene());
    if(pScene)
        pScene->SectionChange();
    qDebug() << "cur start Time:" << m_pSection->GetStartTime();
}

void	RDSectionItem::removeSection()
{
	m_pNode->RemoveSection(m_pSection);
}
