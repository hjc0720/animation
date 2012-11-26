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

RDSectionItem::RDSectionItem(const RDSection* pSection,int nHeight,int nXOffset,int nYOffset)
    :m_pSection(pSection)
     ,m_nHeight(nHeight)
     ,m_nXOffset(nXOffset)
     ,m_nYOffset(nYOffset)
{
    SetSectionType();
    setPos(m_nXOffset + m_pSection->GetStartTime(),m_nYOffset);
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
    qDebug() << "image initialization type" << m_pSection->GetType() ;
}

void RDSectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * , QWidget *)
{
    QLinearGradient line(0,m_nHeight / 2,0,0);
    line.setSpread(QGradient::ReflectSpread);
    line.setColorAt(0,QColor(0,0,255));
    line.setColorAt(1,QColor(255,255,255));
    QBrush brush(line);
    painter->fillRect(m_pSection->GetStartTime() ,0,
            m_pSection->GetLength(),m_nHeight,line);
    qreal fScale = painter->worldTransform().m11();
    qreal fRealSize = 16 / fScale;
    qDebug() << fRealSize << m_pSection->GetLength() / 2;
    qDebug() << "image width" << m_imgSectionType.size();
    if(fRealSize < m_pSection->GetLength() / 2)
    {
        QRectF target(m_pSection->GetStartTime() + m_pSection->GetLength() - fRealSize,0,fRealSize,m_nHeight);
        //        QRectF target(0,0,m_pSection->GetLength(),m_nHeight);
        painter->drawImage(target,m_imgSectionType);
        qDebug() << "image save";
    }
}

QRectF RDSectionItem::boundingRect()const
{
    float fLeft = m_pSection->GetStartTime() + m_nXOffset;
    return QRectF(fLeft,m_nYOffset,m_pSection->GetLength(),m_nHeight);
}
