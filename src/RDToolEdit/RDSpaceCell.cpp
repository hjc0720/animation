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
    m_pPos = CreateVectorWidget(tr("pos"),-10000,10000,0,1);
    m_pAngle = CreateVectorWidget(tr("Angle"),-360,360,2,0.1);
    m_pScale = CreateVectorWidget(tr("Scale"),0.001,100,3,0.1);
}

RDVec3Widget*  RDSpaceCell::CreateVectorWidget(const QString& name,double fMin,double fMax,int decimals,double step)
{
    QGroupBox* pGroup = new QGroupBox(name,this);
    QHBoxLayout* pHLayout = new QHBoxLayout();
    RDVec3Widget* pWidget = new RDVec3Widget(this);
    pWidget->SetDecimals(decimals);
    pWidget->SetRange(fMin,fMax);
    pWidget->SetStep(step);
    pHLayout->addWidget(pWidget);
    pGroup->setLayout(pHLayout);
    AddWidget(pGroup);
    connect(pWidget, SIGNAL(Changed(const float3&)), this, SLOT(CellChange()));
    return pWidget;
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

const float3&    RDSpaceCell::GetPos()const
{
    return m_pPos->value();
}

void    RDSpaceCell::SetAngle(const float3& newPos)
{
    m_pAngle->SetValue(newPos);
}

const float3&    RDSpaceCell::GetAngle()const
{
    return m_pAngle->value();
}

void    RDSpaceCell::SetScale(const float3& newScale) 
{
    m_pScale->SetValue(newScale);
}

const float3&     RDSpaceCell::GetScale() const
{
    return m_pScale->value();
}
