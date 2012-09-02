// =====================================================================================
// 
//       Filename:  RDTimeRuler.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/12/11 19:18:25
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDTimeRuler.h"
#include <QStylePainter>
#include <QPaintEvent>
#include <QDebug>

const int LABEL_SIZE = 20;
const int littleMarkDistance = 10;
const int mediumMarkDistance = 5 * littleMarkDistance;
const int bigMarkDistance = 2 * mediumMarkDistance;


 const int FIX_WIDTH = 24; /* widget width in pixel */
 const int LINE_END = (FIX_WIDTH - 3);
 const int END_MARK_LENGTH = (FIX_WIDTH - 8);
 const int BIG_MARK_LENGTH = (END_MARK_LENGTH * 3 / 4);
 const int BIG_MARK_X2 = LINE_END;
 const int BIG_MARK_X1 = (BIG_MARK_X2 - BIG_MARK_LENGTH);
 const int MIDDLE_MARK_LENGTH = (END_MARK_LENGTH / 2);
 const int MIDDLE_MARK_X2 = LINE_END;
 const int MIDDLE_MARK_X1 = (MIDDLE_MARK_X2 - MIDDLE_MARK_LENGTH);
 const int LITTLE_MARK_LENGTH = (MIDDLE_MARK_LENGTH / 2);
 const int LITTLE_MARK_X2 = LINE_END;
 const int LITTLE_MARK_X1 = (LITTLE_MARK_X2 - LITTLE_MARK_LENGTH);

RDTimeRuler::RDTimeRuler(int nScale,QWidget* parent)
	:QWidget(parent)
	 ,m_nScale(nScale)
{
	m_nOffset = 0;
    //setMinimumHeight(20);
	m_nPos = 0;
    m_bSelectTime = false;

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    //setMouseTracking(false);
}
void RDTimeRuler::paintEvent(QPaintEvent * e)
{
	double textSpacing = bigMarkDistance;
    QStylePainter p(this);
    p.setClipRect(e->rect());
    //const int projectEnd = (int)(m_duration * m_factor);
    p.fillRect(0, 0, width(), height(),palette().alternateBase().color() );

   const QRect zoneRect();


    int minval = (e->rect().left() + m_nOffset) / littleMarkDistance - 1;
    const int maxval = (e->rect().right() + m_nOffset) / littleMarkDistance + 1;
    if (minval < 0) minval = 0;

    //double f, fend;
    double f;
    const int offsetmax = maxval * littleMarkDistance;

    p.setPen(palette().dark().color());

    // draw time labels
    int offsetmin = (e->rect().left() + m_nOffset) / textSpacing;
    offsetmin = offsetmin * textSpacing;
    for (f = offsetmin; f < offsetmax; f += textSpacing) {
		double dSecond = f * m_nScale / 1000000000;
        QString lab = QString("%1").arg(dSecond);
        p.drawText(f - m_nOffset + 2, LABEL_SIZE, lab);
    }

    offsetmin = (e->rect().left() + m_nOffset) / littleMarkDistance;
    offsetmin = offsetmin * littleMarkDistance;
    // draw the little marks
    //fend = m_nScale * littleMarkDistance;
    //if (fend > 5) 
	for (f = offsetmin - m_nOffset; f < offsetmax - m_nOffset; f += littleMarkDistance)
		p.drawLine((int)f, LITTLE_MARK_X1, (int)f, LITTLE_MARK_X2);

    offsetmin = (e->rect().left() + m_nOffset) / mediumMarkDistance;
    offsetmin = offsetmin * mediumMarkDistance;
    // draw medium marks
    //fend = m_nScale  * mediumMarkDistance;
    //if (fend > 5) 
	for (f = offsetmin - m_nOffset - mediumMarkDistance; f < offsetmax - m_nOffset + mediumMarkDistance; f += mediumMarkDistance)
		p.drawLine((int)f, MIDDLE_MARK_X1, (int)f, MIDDLE_MARK_X2);

    offsetmin = (e->rect().left() + m_nOffset) / bigMarkDistance;
    offsetmin = offsetmin * bigMarkDistance;
    // draw big marks
    //fend = m_nScale  * bigMarkDistance;
    //if (fend > 5) 
	for (f = offsetmin - m_nOffset; f < offsetmax - m_nOffset; f += bigMarkDistance)
		p.drawLine((int)f, BIG_MARK_X1, (int)f, BIG_MARK_X2);

    // draw pointer
    QPolygon pa(3);
    const int value  = m_nPos / m_nScale - m_nOffset;
    pa.setPoints(3, value - 6, 8, value + 6, 8, value, 16);
    p.setBrush(palette().highlight());
    p.drawPolygon(pa);
    //qDebug() << "time pos" <<value;
}

void RDTimeRuler::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton && m_bSelectTime)
    {
        m_nPos += (event->x() - m_nInitPos) * m_nScale;
        m_nInitPos = event->x();
        update();
        emit FrameChanged(m_nPos);
        //qDebug() << m_nPos;
    }
    m_bDrag = true;
}
void RDTimeRuler::mousePressEvent(QMouseEvent* event)
{
    if(event->button() & Qt::LeftButton)
    {
        int nTimePosInPixel = m_nPos / m_nScale - m_nOffset;
        if(event->x() >= nTimePosInPixel - 3 && event->x() <= nTimePosInPixel + 3)
        {
            m_bSelectTime = true;
            m_nInitPos = event->x();
        }
        m_bDrag = false;
    }
}
void RDTimeRuler::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() & Qt::LeftButton)
    {
        if(!m_bSelectTime && !m_bDrag)
        {
            m_nPos = (event->x() + m_nOffset) * m_nScale;
            update();
            emit FrameChanged(m_nPos);
            qDebug() << " time pos" << m_nPos << event->x() << m_nOffset << m_nScale;
        }
        m_bSelectTime = false;
    }
}
