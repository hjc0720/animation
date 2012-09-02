// =====================================================================================
// 
//       Filename:  RDColorWidget.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  14/05/11 20:12:15
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDColorWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QColorDialog>
#include <QDebug>

RDColorWidget::RDColorWidget(QWidget* parent)
    :QWidget(parent)
     ,m_Color(0,0,0)
{
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
}
RDColorWidget::RDColorWidget(QColor& color,QWidget* parent)
    :QWidget(parent)
     ,m_Color(color)
{
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
}
QSize RDColorWidget::sizeHint() const
{
    return QSize(16,16);
}
RDColorWidget::~RDColorWidget()
{

}
void RDColorWidget::paintEvent(QPaintEvent* event)
{
    //qDebug() << "color widget paint Event";
    QPainter painter(this);
    painter.setClipRect(rect());
    painter.setClipRect(event->rect(),Qt::IntersectClip);
    QRect target(rect());
    target.adjust(1,1,-1,-1);
    QBrush nowBrush(m_Color);
    painter.setBrush(nowBrush);
    painter.drawRect(target);
}
void RDColorWidget::mousePressEvent(QMouseEvent * event)
{
    if(event->button() != Qt::LeftButton)
        return QWidget::mousePressEvent(event);
    QColor oldColor(m_Color);
    QColorDialog dlg;
    connect(&dlg,SIGNAL(colorSelected(const QColor&)),this,SLOT(ChangedColor(const QColor&)));
    connect(&dlg,SIGNAL(currentColorChanged(const QColor&)),this,SLOT(ChangingColor(const QColor&)));
    if(dlg.exec() != QDialog::Accepted)
    {
        m_Color = oldColor;
        update();
        emit ColorCancel(m_Color);
    }
}
void RDColorWidget::ChangingColor(const QColor& color)
{
    m_Color = color;
    update();
    emit ColorChanging(m_Color);
}
void RDColorWidget::ChangedColor(const QColor& color)
{
    m_Color = color;
    update();
    emit ColorChanged(m_Color);
}
