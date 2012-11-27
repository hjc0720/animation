/*
 * =====================================================================================
 *
 *       Filename:  RDStoryItem.h
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

#ifndef  RDSTORYITEM_INC
#define  RDSTORYITEM_INC

#include <QGraphicsItem>
#include "mac_define.h"

class RDStory;
class RDStoryItem : public QGraphicsItem
{
public:
    RDStoryItem(const RDStory* pStory,int nHeight);
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    void SetStoryType();
protected:
    const RDStory* m_pStory;
    int         m_nHeight;
};

#endif   /* ----- #ifndef RDStoryItem_INC  ----- */
