// =====================================================================================
// 
//       Filename:  RDCell.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/27/2011 12:01:55 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDCell.h"
#include <QString>
#include <QLabel>
#include "RDMd5.h"
#include <QPushButton>
RDCell::RDCell(const QString& title, QWidget* parent)
    :QWidget(parent)
{
    m_pMainWidget = new QWidget(this);
    m_pMainLayout = new QVBoxLayout(this);
    QPushButton* pButton = new QPushButton(title,this);
    pButton->setCheckable(true);
    pButton->setFlat(true);
    m_pMainLayout->setContentsMargins(0,0,0,0);
    //m_pMainLayOut->addLayout(pButton);
    m_pMainWidget->setLayout(m_pMainLayout);
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pButton);
    pLayout->addWidget(m_pMainWidget);
    pLayout->setContentsMargins(0,0,0,0);
    connect(pButton, SIGNAL(toggled(bool)), this, SLOT(ShowLayout(bool)));
}
void RDCell::ShowLayout(bool checked)
{
    m_pMainWidget->setVisible(!checked);
}
void    RDCell::CellChange()
{
    emit CellChanged(GetCellMd5());
}
