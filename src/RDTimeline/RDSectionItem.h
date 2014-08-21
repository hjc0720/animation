/*
 * =====================================================================================
 *
 *       Filename:  RDSectionItem.h
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
#include <QGraphicsItem>
#include "mac_define.h"

class RDSection;
class RDSectionItem : public QGraphicsItem
{
public:
    RDSectionItem(RDSection* pSection,int nHeight,int nXOffset,int nYOffset);
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void SetSectionType();
    bool    HitTest(RDTime pos);
protected:
    bool        m_bHitTest;
    int         m_nHeight;
    int         m_nXOffset;
    int         m_nYOffset;
    RDSection* m_pSection;
    QImage      m_imgSectionType;
};
