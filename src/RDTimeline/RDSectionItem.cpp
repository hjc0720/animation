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

RDSectionItem::RDSectionItem(RDNode* pNode,RDSection* pSection,int nHeight,int nYOffset)
    :m_bHitTest(false)
     ,m_nHeight(nHeight)
     ,m_nYOffset(nYOffset)
     ,m_pNode(pNode)
      ,m_pSection(pSection)
{
    SetSectionType();
    setPos(m_pSection->GetStartTime(),m_nYOffset);
    setFlag(ItemIsMovable);
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
    painter->fillRect(0,0,m_pSection->GetLength(),m_nHeight,line);
    qreal fScale = painter->worldTransform().m11();
    qreal fRealSize = 16 / fScale;
    //qDebug() << fRealSize << m_pSection->GetLength() / 2;
    //qDebug() << "image width" << m_imgSectionType.size();
    if(fRealSize < m_pSection->GetLength() / 2)
    {
        QRectF target(m_pSection->GetLength() - fRealSize,0,fRealSize,m_nHeight);
        //        QRectF target(0,0,m_pSection->GetLength(),m_nHeight);
        painter->drawImage(target,m_imgSectionType);
    }
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
    //qDebug() << "cur start Time:" << m_pSection->GetStartTime();
}

//void RDSectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(event->buttons().testFlag(Qt::LeftButton))
//    {
//        float fHeight = event->scenePos().y();
//        qDebug() << "hit test section " << (size_t)this <<":" << event->scenePos() << "cur height" << m_nYOffset;
//        if(fHeight  > m_nYOffset && fHeight < m_nYOffset + m_nHeight)
//        {
//            RDTime oldPos = event->scenePos().x();
//            if(HitTest(oldPos))
//            {
//                m_bHitTest = true;
//                event->accept();
//            }
//        }
//    }
//
//}

bool    RDSectionItem::HitTest(RDTime pos)
{
    return (pos >= m_pSection->GetStartTime() && pos <= m_pSection->GetEndTime());
}
