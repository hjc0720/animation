// =====================================================================================
// 
//       Filename:  RDColorWidget.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  14/05/11 20:12:27
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDCOLORWIDGET_INC
#define  RDCOLORWIDGET_INC
#include <QWidget>
#include <QColor>

class RDColorWidget: public QWidget
{
    Q_OBJECT
public:
    RDColorWidget(QWidget* parent);
    RDColorWidget(QColor& color,QWidget* parent);
    ~RDColorWidget();
    QSize sizeHint() const;
    void SetColor(const QColor& color){m_Color = color;update();}
    const QColor& GetColor()const{return m_Color;}
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent * event);
signals:
    void ColorChanging(const QColor& color);
    void ColorChanged(const QColor& color);
    void ColorCancel(const QColor& oldColor);
protected slots:
    void ChangingColor(const QColor& color);
    void ChangedColor(const QColor& color);
protected:
    QColor m_Color;
};
#endif   // ----- #ifndef rdcolorwidget_INC  -----
