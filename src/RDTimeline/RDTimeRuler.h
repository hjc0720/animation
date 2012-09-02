// =====================================================================================
// 
//       Filename:  RDTimeRuler.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/12/11 19:18:32
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDTIMERULER_INC
#define  RDTIMERULER_INC
#include <QWidget>
#include "mac_define.h"

enum RULER_MOVE { RULER_CURSOR = 0, RULER_START = 1, RULER_MIDDLE = 2, RULER_END = 3 };
enum MOUSE_MOVE { NO_MOVE = 0, HORIZONTAL_MOVE = 1, VERTICAL_MOVE = 2 };

class RDTimeRuler : public QWidget
{
    Q_OBJECT
public:
	RDTimeRuler(int nScale,QWidget* parent);
    virtual void paintEvent(QPaintEvent * /*e*/);
    virtual QSize sizeHint()const{return QSize(800,20);}
signals:
    void    FrameChanged(const RDTime& nFrame);
protected:
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
protected:
	RDTime 	m_nScale; //time / pixel
	int		m_nOffset;
	RDTime		m_nPos;

    int m_zoneStart;
    int m_zoneEnd;
    int m_duration;
    QColor m_bgColor;
    QColor m_cursorColor;
    QColor m_zoneColor;
    double m_factor;
    RULER_MOVE m_moveCursor;
    QAction *m_editGuide;
    QAction *m_deleteGuide;
    int m_clickedGuide;
    /** Used for zooming through vertical move */
    QPoint m_clickPoint;
    int m_rate;
    int m_startRate;
    MOUSE_MOVE m_mouseMove;

    bool m_bSelectTime;
    bool m_bDrag;
    int  m_nInitPos;

};
#endif   // ----- #ifndef rdtimeruler_INC  -----
