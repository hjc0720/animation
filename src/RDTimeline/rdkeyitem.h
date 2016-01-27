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

#ifndef RDKEYITEM_H
#define RDKEYITEM_H

#include <QGraphicsItem>
#include "mac_define.h"

class RDSection;

class RDKeyItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit RDKeyItem(int nHeight, RDTime time, QGraphicsItem* parent = nullptr);
    QRectF boundingRect()const;

    // QGraphicsItem interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void TimeMoved(RDTime srcTime,RDTime dstTime);
protected:
    int         m_nHeight;
    RDTime      m_nTime;
    float       m_fRealWidth;

    static const float value ;
    // QGraphicsItem interface
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // RDKEYITEM_H
