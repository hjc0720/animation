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
#include "HVector4f.h"

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
    QHBoxLayout* pHLayoutPos = new QHBoxLayout();
    QLabel* pPosLabelX = new QLabel(tr("X"),this);
    m_pPosX = new QLineEdit(this);
    m_pPosX->setValidator(new QDoubleValidator(m_pPosX));

    QLabel* pPosLabelY = new QLabel(tr("Y"),this);
    m_pPosY = new QLineEdit(this);
    m_pPosY->setValidator(new QDoubleValidator(m_pPosY));

    QLabel* pPosLabelZ = new QLabel(tr("Z"),this);
    m_pPosZ = new QLineEdit(this);
    m_pPosZ->setValidator(new QDoubleValidator(m_pPosZ));

    pHLayoutPos->addWidget(pPosLabelX);
    pHLayoutPos->addWidget(m_pPosX);
    pHLayoutPos->addWidget(pPosLabelY);
    pHLayoutPos->addWidget(m_pPosY);
    pHLayoutPos->addWidget(pPosLabelZ);
    pHLayoutPos->addWidget(m_pPosZ);

    pPos->setLayout(pHLayoutPos);
    AddWidget(pPos);
    connect(m_pPosX, SIGNAL(textEdited(const QString &)), this, SLOT(CellChange()));
    connect(m_pPosY, SIGNAL(textEdited(const QString &)), this, SLOT(CellChange()));
    connect(m_pPosZ, SIGNAL(textEdited(const QString &)), this, SLOT(CellChange()));
}
const RDMd5& RDSpaceCell::GetCellMd5()
{
    const static RDMd5 SpaceCellMd5("RDSpaceCell",-1);
    return SpaceCellMd5;
}
void   RDSpaceCell::SetPos(const float3& newPos)
{
    m_pPosX->setText(QString::number(newPos.x()));
    m_pPosY->setText(QString::number(newPos.y()));
    m_pPosZ->setText(QString::number(newPos.z()));
}

void    RDSpaceCell::GetPos(float3& newPos)
{
    newPos.SetX(m_pPosX->text().toFloat());
    newPos.SetY(m_pPosY->text().toFloat());
    newPos.SetZ(m_pPosZ->text().toFloat());
}
