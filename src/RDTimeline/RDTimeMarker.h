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
#include <QDebug>
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
    RDTimeMarker (int nHeight,int nViewHeight,double dScale);                             /* constructor */
    /* ====================  ACCESSORS     ======================================= */
    void SetHeight(int nHeight){m_nHeight = nHeight;}
    void SetTime(const RDTime& nCurTime){m_nCurTime = nCurTime; setPos(m_nCurTime,0);}
    const RDTime& GetTime()const {return m_nCurTime ;}
    void SetScale(double dScale){m_dScale = 1 / dScale; update();}

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
    double      m_dScale;

}; /* -----  end of class RDTimeMarker  ----- */

#endif   /* ----- #ifndef RDTimeMarker_INC  ----- */
