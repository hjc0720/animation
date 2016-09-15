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
#include "RDSection.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <sstream>
#include <QClipboard>
#include <QApplication>
#include "RDNode.h"


const float RDKeyItem::value = 6 ;
RDKeyItem::RDKeyItem(int nHeight, RDTime time, const std::list<RDSingleKey> &list, QGraphicsItem* parent)
    :QGraphicsObject(parent)
    ,m_nHeight(nHeight)
    ,m_nTime(time)
    ,m_keylist(list)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
}

QRectF RDKeyItem::boundingRect() const
{
    return QRectF(-m_fRealWidth,value,2*m_fRealWidth,m_nHeight-2*value);
}

void RDKeyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
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


    if(isSelected())
    {
        painter->setBrush(Qt::yellow);
    }
    else
    {
        painter->setPen(Qt::green);
    }
    painter->drawPolygon(pt,4);

    //qDebug() << pt[0] << pt[1] << pt[2] << pt[3];
}


void RDKeyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    RDTime offsetPos = event->scenePos().x() - event->lastScenePos().x();
    emit TimeMoved(m_nTime,m_nTime+offsetPos);
    m_nTime+=offsetPos;
    setPos(m_nTime,0);
}


void RDKeyItem::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy))
    {
        std::stringstream  ss;
        RDJsonDataStream json(ss, true );
        auto selectItems = scene()->selectedItems();
        Json::Value keyValue;
        for(QGraphicsItem* item: selectItems)
        {
            RDKeyItem* pItem = dynamic_cast<RDKeyItem*>(item);
            if(pItem)
            {
                Json::Value child;
                json.Serialize(child,"time",pItem->m_nTime);
                json.Serialize(child,"keys",pItem->m_keylist.begin(),pItem->m_keylist.end(),pItem->m_keylist.begin(),[](RDJsonDataStream&buffer, Json::Value& parent,RDSingleKey& value){
                   buffer.Serialize(parent,"type",value.type);
                   value.key->Serialize(buffer,parent);
                });
                keyValue.append(child);
            }
        }

        if(keyValue.size() != 0)
        {
            json.getRoot()["keyvalue"]=keyValue;
            json.getRoot()["type"]=Json::Value(RDKeyValue);
            json.close();
            //to copyboard
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText( ss.str().data());
        }
        event->accept();
    }
    else
        QGraphicsObject::keyPressEvent(event);
}
