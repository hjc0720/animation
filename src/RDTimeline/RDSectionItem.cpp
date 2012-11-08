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

RDSectionItem::RDSectionItem(const RDSection* pSection,int nHeight,int nXOffset,int nYOffset)
    :m_pSection(pSection)
     ,m_nHeight(nHeight)
     ,m_nXOffset(nXOffset)
     ,m_nYOffset(nYOffset)
{

}

void RDSectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QLinearGradient line(0,m_nHeight / 2,0,0);
    line.setSpread(QGradient::ReflectSpread);
    line.setColorAt(0,QColor(0,0,255));
    line.setColorAt(1,QColor(255,255,255));
    QBrush brush(line);
    painter->fillRect(m_pSection->GetStartTime() + m_nXOffset,m_nYOffset,
            m_pSection->GetLength(),m_nHeight,line);
}

QRectF RDSectionItem::boundingRect()const
{
    float fLeft = m_pSection->GetStartTime() + m_nXOffset;
    return QRectF(fLeft,m_nYOffset,fLeft + m_pSection->GetLength(),m_nYOffset + m_nHeight);
}
