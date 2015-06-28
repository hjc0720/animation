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

#ifndef RDLINE_H
#define RDLINE_H

#include <vector>
#include "HVector3f.h"
#include "HVector4f.h"
#include <cstdint>

struct LinePoint
{
    float3 vPos;
    float4 vColor;
};

class RDLine
{
public:
    RDLine(float fWidth);
    void setColor(uint32_t color){m_vColor = float4(color);}
    void setWidth(float fWidth){m_fWidth = fWidth;}
    void moveTo(const float3& vPt);
    void lineTo(const float3& vPt);
    void clear(){m_vPt.clear();}
    void drawLine();
protected:
    void updatePt();
protected:
    std::vector<LinePoint> m_vPt;
    float4 m_vColor;
    LinePoint  m_startPt;
    bool    m_bChange;
    float       m_fWidth;

};

#endif // RDLINE_H
