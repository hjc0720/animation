/*
 * =====================================================================================
 *
 *       Filename:  RDStoryItem.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月23日 20时11分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "RDStoryItem.h"
#include "RDStory.h"
#include "math.h"
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "RDScetionView.h"
#include "RDSectionScene.h"

RDStoryItem::RDStoryItem(const RDStory* pStory,int nHeight)
    :m_pStory(pStory)
     ,m_nHeight(nHeight)
{
    SetStoryType();
    setPos(m_pStory->GetStartTime(),0);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF RDStoryItem::boundingRect()const
{
    return QRectF(0,0,m_pStory->GetStoryLength(),m_nHeight);
}

void RDStoryItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *widget)
{
    qreal fScale = painter->worldTransform().m11();
    QRectF rect(boundingRect());
    float fOnePixel = 1 / fScale;
    painter->fillRect(rect.adjusted(fOnePixel,-1,-fOnePixel,1),widget->palette().button() );
    painter->drawRect(boundingRect() );
    double dPixelSize = 10;
    double dLineSize = dPixelSize / fScale;
    double dSecond = dLineSize * 10 / SECOND_TO_TIME;
    double dMsecond = dLineSize * 10 / MSECOND_TO_TIME;
    int nType = dSecond < 0.1 ? 0 : 1;
    if(nType)
        dLineSize = round(dSecond * 10) / 100 * SECOND_TO_TIME;
    else
        dLineSize = round(dMsecond) / 10 * MSECOND_TO_TIME;
    double dBigLine = m_nHeight - m_nHeight;
    double dMedLine = m_nHeight - (m_nHeight >> 1);
    double dSmallLine = m_nHeight - (m_nHeight >> 2);
    
    double dWidth = m_pStory->GetStoryLength();
    double dLen = 0;
    int nIndex = 0;

    painter->setPen(widget->palette().dark().color());
    while(dLen < dWidth)
    {
        double dEndLine = dSmallLine;
        if(nIndex % 10 == 0)
            dEndLine = dBigLine;
        else if(nIndex % 5 == 0)
            dEndLine = dMedLine; 

        painter->drawLine(QPointF(dLen,m_nHeight), QPointF(dLen,dEndLine));
        dLen += dLineSize;
        nIndex++;
    }
    //drawText
    double dTexLen = 0;
    painter->scale(1/fScale,1);
    while(dTexLen < dWidth)
    {
        QString lab;
        if(nType)
        {
            double dNowScond = dTexLen / SECOND_TO_TIME;
            lab = QString("%1 s").arg(dNowScond);
        }
        else
        {
            double dNowScond = dTexLen / MSECOND_TO_TIME;
            lab = QString("%1 ms").arg(dNowScond);
        }
        painter->drawText((dTexLen + 1) * fScale, 20, lab);
        dTexLen += dLineSize * 10;
//        qDebug() << "tex pos"<< dTexLen << dNowScond << "s";
    }
}

void RDStoryItem::SetStoryType()
{

}

void	RDStoryItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    RDSectionScene* pScene = dynamic_cast<RDSectionScene*>(scene());
    if(pScene && event->button() == Qt::LeftButton)
    {
        RDTime pos = event->scenePos().x();
        pScene->ChangeFrame(pos);
        event->accept();
    }
}

void RDStoryItem::updateLength()
{
	prepareGeometryChange();
}
