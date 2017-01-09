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
#include "RDSection.h"

class RDSection;
class QFormLayout;
template <typename T> class RDKey;

class RDKeyItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit RDKeyItem(int nHeight, RDTime time,const std::list<RDSingleKey>& list,  QGraphicsItem* parent = nullptr);
    QRectF boundingRect()const;

    // QGraphicsItem interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void TimeMoved(RDTime srcTime,RDTime dstTime);
    void valueChanged(RDTime nTime);
    // QGraphicsItem interface
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
    void    restoreValue(const std::vector<QVariant>& initValue);
    void    addKeyEdit(QFormLayout* pLayout,const std::string& type,RDKey<float3>* pKey);
protected:
    int         m_nHeight;
    RDTime      m_nTime;
    float       m_fRealWidth;
    std::list<RDSingleKey> m_keylist;

    static const float value ;
};

#endif // RDKEYITEM_H
