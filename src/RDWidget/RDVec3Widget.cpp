/*
 * =====================================================================================
 *
 *       Filename:  RDVec3Widget.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年03月22日 10时32分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang JianChao (hjc), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "RDVec3Widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "rddoublespinbox.h"
#include <QLabel>

RDVec3Widget::RDVec3Widget(QWidget* parent,bool bHorizon /*= false*/)
    :QWidget(parent)
{
    QBoxLayout* pLayout = nullptr;
    if(bHorizon)
        pLayout = new QHBoxLayout();
    else
        pLayout = new QVBoxLayout();
    QStringList st(QStringList()<<tr("x")<<tr("y")<<tr("z"));
    for(int i = 0; i < 3; i++)
    {
        m_pVector[i] = new RDDoubleSpinBox(this);
        connect(m_pVector[i],SIGNAL(valueChanging(double)),this,SLOT(VectorChanged()));
        QLabel* pLabel = new QLabel(st[i],this);
        QHBoxLayout* pItem = new QHBoxLayout();
        pItem->addWidget(pLabel);
        pItem->addWidget(m_pVector[i]);
        pLayout->addLayout(pItem);
        pLayout->addStretch();
    }
    setLayout(pLayout);
}

RDVec3Widget::~RDVec3Widget()
{
    std::for_each(m_pVector,m_pVector + 3,[&](QDoubleSpinBox* pVector){delete pVector;});
}

void RDVec3Widget::SetMin(double dMin)
{
    std::for_each(m_pVector,m_pVector + 3,[&](QDoubleSpinBox* pVector){pVector->setMinimum(dMin);});
}

void RDVec3Widget::SetMax(double dMax)
{
    std::for_each(m_pVector,m_pVector + 3,[&](QDoubleSpinBox* pVector){pVector->setMaximum(dMax);});
}

void RDVec3Widget::SetRange(double dMin,double dMax)
{
    std::for_each(m_pVector,m_pVector + 3,[&](QDoubleSpinBox* pVector){pVector->setRange(dMin,dMax);});
}

void RDVec3Widget::SetStep(double val)
{
    std::for_each(m_pVector,m_pVector + 3,[&](QDoubleSpinBox* pVector){pVector->setSingleStep(val);});
}

void RDVec3Widget::SetDecimals(int prec)
{
    std::for_each(m_pVector,m_pVector + 3,[&](QDoubleSpinBox* pVector){pVector->setDecimals(prec);});
}

void RDVec3Widget::VectorChanged()
{
    float data[3];
    for(int i = 0; i < 3;i++)
        data[i] = m_pVector[i]->value();

    m_vValue.Set(data[0],data[1],data[2]);
    emit Changed(m_vValue);
}

void RDVec3Widget::SetValue(const float3& value)
{
    m_vValue = value;
    m_pVector[0]->updateValue(m_vValue.x());
    m_pVector[1]->updateValue(m_vValue.y());
    m_pVector[2]->updateValue(m_vValue.z());
}
