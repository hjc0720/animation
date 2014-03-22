// =====================================================================================
// 
//       Filename:  RDPropertySheet.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/27/2011 10:38:25 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDPropertySheet.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "mac_define.h"
#include "RDCell.h"
#include <QDebug>

RDPropertySheet::RDPropertySheet(int width,QWidget* parent /*= 0*/)
    :QDockWidget(parent)
{
    setFeatures(0/*QDockWidget::DockWidgetFloatable*/);
    setMinimumWidth(width);
    m_pMainWidget = new QScrollArea();
    m_pMainWidget->setWidgetResizable(true);
    QWidget* pWidget = new QWidget();
    m_pLayOut = new QVBoxLayout(pWidget);
    m_pLayOut->setContentsMargins(0,0,0,0);
    pWidget->setLayout(m_pLayOut);
    m_pMainWidget->setWidget(pWidget);
    setWidget(m_pMainWidget);
}

RDPropertySheet::~RDPropertySheet()
{
}
void RDPropertySheet::PropertyChanged(RDCell** pCellArray,int nCount)
{
    for(int i = m_pLayOut->count() - 1; i >= 0 ;i--)
    {
        QLayoutItem* pItem = m_pLayOut->itemAt(i);
        if(pItem->widget())
            pItem->widget()->setVisible(false);
        m_pLayOut->removeItem(pItem);
        SAFE_DELETE(pItem);
    }
    for(int i = 0; i < nCount;i++)
    {
        m_pLayOut->addWidget(pCellArray[i]);
        pCellArray[i]->setVisible(true);
    }
    m_pLayOut->addStretch();
}
