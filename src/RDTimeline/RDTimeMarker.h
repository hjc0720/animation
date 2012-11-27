/*
 * =====================================================================================
 *
 *       Filename:  RDTimeMarker.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月28日 07时12分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  RDTIMEMARKER_INC
#define  RDTIMEMARKER_INC

#include <QGraphicsItem>
#include "mac_define.h"
/*
 * =====================================================================================
 *        Class:  RDTimeMarker
 *  Description:  
 * =====================================================================================
 */
class RDTimeMarker : public QGraphicsItem
{
public:
    /* ====================  LIFECYCLE     ======================================= */
    RDTimeMarker (int nHeight,int nViewHeight);                             /* constructor */
    /* ====================  ACCESSORS     ======================================= */
    void SetHeight(int nHeight){m_nHeight = nHeight;}
    void SetTime(RDTime& nCurTime){m_nCurTime = nCurTime;}
    const RDTime& GetTime()const {return m_nCurTime ;}

    QRectF boundingRect()const;
    /* ====================  MUTATORS      ======================================= */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /* ====================  OPERATORS     ======================================= */

protected:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */
    QPolygon    m_tri;
    RDTime      m_nCurTime;
    int         m_nHeight;

}; /* -----  end of class RDTimeMarker  ----- */

#endif   /* ----- #ifndef RDTimeMarker_INC  ----- */
