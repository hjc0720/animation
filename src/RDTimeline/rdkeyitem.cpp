/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "rdkeyitem.h"
#include <QPainter>
#include <QPointF>

const float RDKeyItem::value = 6 ;
RDKeyItem::RDKeyItem(int nHeight,RDTime time,QGraphicsItem* parent)
    :QGraphicsItem(parent)
    ,m_nHeight(nHeight)
    ,m_nTime(time)
{
}

QRectF RDKeyItem::boundingRect() const
{
    return QRectF(-m_fRealWidth,value,m_fRealWidth,m_nHeight-value);
}

void RDKeyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::yellow);

    qreal fScale = painter->worldTransform().m11();
    m_fRealWidth = (m_nHeight - value * 2) / 2 / fScale;
    QPointF pt[4];

    pt[0].setX(0);
    pt[0].setY(value);

    pt[1].setX(- m_fRealWidth);
    pt[1].setY(m_nHeight / 2.f );

    pt[2].setX(0);
    pt[2].setY(m_nHeight - value);

    pt[3].setX(m_fRealWidth);
    pt[3].setY(m_nHeight / 2.f);

    //qDebug() << pt[0] << pt[1] << pt[2] << pt[3];
    painter->drawPolygon(pt,4);
}
