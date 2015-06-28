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

#include "rdline.h"

RDLine::RDLine(float fWidth)
{
    m_bChange = true;
    setWidth(fWidth);
}

void RDLine::drawLine()
{
    //glSe
}

void RDLine::updatePt()
{
    if(!m_bChange)
        return;


    m_bChange = false;
}

void RDLine::moveTo(const float3 &vPt)
{
    m_startPt = LinePoint{vPt,m_vColor};
}

void RDLine::lineTo(const float3 &vPt)
{
    m_vPt.push_back(m_startPt);
    m_startPt = LinePoint{vPt,m_vColor};
}
