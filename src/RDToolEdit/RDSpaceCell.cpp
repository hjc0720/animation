// =====================================================================================
// 
//       Filename:  RDSpaceCell.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/04/2011 09:45:17 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDSpaceCell.h"
#include "RDMd5.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QGroupBox>
#include "HVector3f.h"
#include "RDVec3Widget.h"

RDSpaceCell* RDSpaceCell::m_pCell = NULL;

RDSpaceCell* RDSpaceCell::GetSpaceCell()
{
    if(!m_pCell)
        m_pCell = new RDSpaceCell(0);
    return m_pCell;
}
RDSpaceCell::RDSpaceCell(QWidget* parent)
    :RDCell(tr("space"),parent)
{
    QGroupBox* pPos = new QGroupBox(tr("pos"),this);
    QHBoxLayout* pHLayout = new QHBoxLayout(this);
    m_pPos = new RDVec3Widget(this);
    m_pPos->SetDecimals(0);
    m_pPos->SetRange(-10000,10000);
    m_pPos->SetStep(1);
    pHLayout->addWidget(m_pPos);
    pPos->setLayout(pHLayout);
    AddWidget(pPos);
    connect(m_pPos, SIGNAL(Changed(const float3&)), this, SLOT(CellChange()));
}

const RDMd5& RDSpaceCell::GetCellMd5()
{
    const static RDMd5 SpaceCellMd5("RDSpaceCell",-1);
    return SpaceCellMd5;
}
void   RDSpaceCell::SetPos(const float3& newPos)
{
    m_pPos->SetValue(newPos);
}

void    RDSpaceCell::GetPos(float3& newPos)
{
    newPos = m_pPos->value();
}
