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
#include "rdkeyitem.h"
#include <jsoncpp/json/json.h>
#include <sstream>
#include <QClipboard>
#include <QApplication>


RDSectionItem::RDSectionItem(RDNode* pNode,RDSection* pSection,int nHeight,int nYOffset)
     :m_nHeight(nHeight)
     ,m_nYOffset(nYOffset)
     ,m_pNode(pNode)
      ,m_pSection(pSection)
{
    SetSectionType();
    setPos(m_pSection->GetStartTime(),m_nYOffset);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
//    setAcceptedMouseButtons(Qt::LeftButton);

    createKeyItem();
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
}

QRectF RDSectionItem::boundingRect()const
{
    return QRectF(0,0,m_pSection->GetLength(),m_nHeight);
}

void RDSectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    RDTime offsetPos = event->scenePos().x() - event->lastScenePos().x();
    m_pSection->MovSection(offsetPos);
    m_pNode->SetChangeLevel(RDRender_TransChange);
    setPos(m_pSection->GetStartTime(),m_nYOffset);
    //    update();
    emit changed();
    qDebug() << "cur start Time:" << m_pSection->GetStartTime();
}

void	RDSectionItem::removeSection()
{
    m_pNode->RemoveSection(m_pSection);
}

void RDSectionItem::itemChange()
{
    removeKeyItem();
    createKeyItem();
}

void RDSectionItem::KeyTimeChanged(RDTime srcTime,RDTime dstTime)
{
    m_pSection->moveKey(srcTime,dstTime);
    m_pNode->SetChangeLevel(RDRender_TransChange);
    emit changed();
}

void RDSectionItem::KeyChanged(RDTime )
{
    m_pNode->SetChangeLevel(RDRender_TransChange);
    emit changed();
}

void RDSectionItem::createKeyItem()
{
    std::set<RDTime> keyTimeSet(m_pSection->getKeyTimeSet());
    for(RDTime time : keyTimeSet)
    {
        RDKeyItem* pKey = new RDKeyItem(m_nHeight,time,m_pSection->getKeyList(time),this);
        pKey->setPos(time,0);
        connect(pKey,SIGNAL(TimeMoved(RDTime,RDTime)),this,SLOT(KeyTimeChanged(RDTime,RDTime)));
        connect(pKey,SIGNAL(valueChanged(RDTime)),this,SLOT(KeyChanged(RDTime)));
        m_vecKeyItem.push_back(pKey);
    }
}

void RDSectionItem::removeKeyItem()
{
    for(RDKeyItem* pItem : m_vecKeyItem)
    {
        SAFE_DELETE(pItem);
    }
    m_vecKeyItem.clear();
}


void RDSectionItem::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy))
    {
        std::stringstream ss;
        RDJsonDataStream json(ss,true);
        m_pSection->Serialize(json,json.getRoot());
        json.getRoot()["clipboard_type"] = Json::Value("section");
        json.close();
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(ss.str().c_str());
        event->accept();
    }
    else if(event->matches(QKeySequence::Paste))
    {
        QClipboard *clipboard = QApplication::clipboard();
        std::stringstream ss;
        ss << clipboard->text().toStdString();
        RDJsonDataStream json(ss,false);
        if(json.getRoot()["clipboard_type"].asString() == "section")
        {
            RDSection tmp;
            tmp.Serialize(json,json.getRoot());

            std::set<RDTime> times = tmp.getKeyTimeSet();
            for(auto it = times.begin(); it != times.end(); it++)
            {
                m_pSection->AddPosKey(*it,tmp.GetPosVector(*it));
                m_pSection->AddAngleKey(*it,tmp.GetAngleVector(*it));
                m_pSection->AddScaleKey(*it,tmp.GetScaleVector(*it));
            }
            event->accept();
            itemChange();

            m_pNode->SetChangeLevel(RDRender_TransChange);
            emit changed();
        }
        else
            event->ignore();

    }
}
