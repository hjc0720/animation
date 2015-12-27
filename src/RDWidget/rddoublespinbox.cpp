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

RDDoubleSpinBox::RDDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
    m_bUpdateValue = false;
    connect(this, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [this](double d){
        if(m_bUpdateValue)
            return;
        emit valueChanging(d);
    });

    setAccelerated(true);
}

void RDDoubleSpinBox::updateValue(double fValue)
{
    m_bUpdateValue = true;
    setValue(fValue);
    m_bUpdateValue = false;
}


RDSpinBox::RDSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    m_bUpdateValue = false;

    connect(this, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [this](int d){
        if(m_bUpdateValue)
            return;
        emit valueChanging(d);
    });
    setAccelerated(true);
}

void RDSpinBox::updateValue(int fValue)
{
    m_bUpdateValue = true;
    setValue(fValue);
    m_bUpdateValue = false;
}
