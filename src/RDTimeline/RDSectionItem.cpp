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

RDSectionItem::RDSectionItem(RDSection* pSection,int nHeight,int nXOffset,int nYOffset)
    :m_pSection(pSection)
     ,m_nHeight(nHeight)
     ,m_nXOffset(nXOffset)
     ,m_nYOffset(nYOffset)
{

}

void RDSectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLinearGradient();
    QBrush brush(QColor(21,277,225),Qt::LinearGradientPattern);
}

QRectF RDSectionItem::boundingRect()const
{
    return QRectF(m_pSection->GetStartTime() + m_nXOffset,m_nYOffset,m_pSection->GetLength(),m_nHeight);
}
