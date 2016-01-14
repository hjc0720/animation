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

class RDKeyItem : public QGraphicsItem
{
public:
    explicit RDKeyItem(int nHeight, RDTime time, QGraphicsItem* parent = nullptr);
    QRectF boundingRect()const;

    // QGraphicsItem interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    int         m_nHeight;
    RDTime      m_nTime;
    static const float value ;
    float       m_fRealWidth;
};

#endif // RDKEYITEM_H
