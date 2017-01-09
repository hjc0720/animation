/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "rddoublespinbox.h"

RDDoubleSpinBox::RDDoubleSpinBox(bool bDyanmicRange,QWidget *parent) :
    QDoubleSpinBox(parent)
  ,m_bDynamicRange(bDyanmicRange)
{
    if(m_bDynamicRange)
        setRange(-1000,1000);
    m_bUpdateValue = false;
    connect(this, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [this](double d){
        if(m_bUpdateValue)
            return;
        updateRange(d);
        emit valueChanging(d);
    });

    setAccelerated(true);
}

void RDDoubleSpinBox::updateValue(double fValue)
{
    m_bUpdateValue = true;

    updateRange(fValue);
    setValue(fValue);
    m_bUpdateValue = false;
}

void RDDoubleSpinBox::updateRange(double dValue)
{
    if(!m_bDynamicRange)
        return;

    double dMin = std::min(minimum(),dValue);
    double dMax = std::max(maximum(),dValue);

    double dNewMin = std::min(dMin,2 * dValue - dMax);
    double dNewMax = std::max(dMax,2 * dValue - dMin);

    setRange(dNewMin,dNewMax);
}


RDSpinBox::RDSpinBox(bool bDyanmicRange,QWidget *parent) :
    QSpinBox(parent)
  ,m_bDynamicRange(bDyanmicRange)
{
    m_bUpdateValue = false;

    connect(this, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [this](int d){
        if(m_bUpdateValue)
            return;

        updateRange(d);
        emit valueChanging(d);
    });
    setAccelerated(true);
}

void RDSpinBox::updateValue(int nValue)
{
    m_bUpdateValue = true;
    updateRange(nValue);
    setValue(nValue);
    m_bUpdateValue = false;
}

void RDSpinBox::updateRange(int nValue)
{
    if(!m_bDynamicRange)
        return;

    int nMin = std::min(minimum(),nValue);
    int nMax = std::max(maximum(),nValue);

    int nNewMin = std::min(nMin,2 * nValue - nMax);
    int nNewMax = std::max(nMax,2 * nValue - nMin);

    setRange(nNewMin,nNewMax);
}
