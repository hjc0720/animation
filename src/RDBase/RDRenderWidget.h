// =====================================================================================
// 
//       Filename:  RDRederWidget.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 14:48:13
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDREDERWIDGET_INC
#define  RDREDERWIDGET_INC
#include <QGLWidget>
#include <QSize>
#include <QRectF>
#include "HMath.h"
#include "RDTimer.h"
#include "RDSwapChain.h"
#include "RDDocument.h"
#include "HVector4f.h"

class MainWindow;

class RDRenderWidget :public QGLWidget
{
    Q_OBJECT;
public:
    RDRenderWidget(int nWidth, int nHeight,const QGLFormat& format,QWidget *parent = 0);
    ~RDRenderWidget();
    QSize hintSize()const;
    virtual bool event(QEvent* e);
    RDDocument& GetDocument(){return m_document;}
signals:
	void DelNoded(RDNode&);
public slots:
    //virtual void setVisible(bool visible);
	void deleteSelItems();
	void SetCursor(QCursor& cursor){setCursor(cursor);}
		
protected:
    //void paintEvent(QPaintEvent* event);
    //void resizeEvent(QResizeEvent* event);
    void resizeGL(int w,int h);
    int GetRealPorjWidth(){return floatToInt(m_nProjWidth * m_fScale);}
    int GetRealPorjHeight(){return floatToInt(m_nProjHeight * m_fScale);}
    float3 ClientToScene(const QPoint& pos);
    void initializeGL () ;
    void paintGL();
protected:
    int     m_nProjWidth;
    int     m_nProjHeight;
    int     m_nXOffset;//after scene scale;
    int     m_nYOffset;//after scene scale;
    float   m_fScale;//real show size divide projSize;
    RDDocument m_document;
    //RDTimer m_RenderTimer;
    RDSwapChain m_swapChain;
    QRectF  m_validRt;
};

#endif   // ----- #ifndef rdrederwidget_INC  -----
