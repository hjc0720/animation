/*
 * =====================================================================================
 *
 *       Filename:  RDBox.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年04月22日 21时05分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang JianChao (hjc), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "RDBox.h"

void RDBox::TransBox(const matrix4x4& worldMat)
{
    std::array<float3,8> arPt;
    CreateBox(arPt);
    m_vMin.Set(FLT_MAX);
    m_vMax.Set(-FLT_MAX);
    for(int i = 0; i < 8; i++)
    {
        arPt[i] *= worldMat;
        m_vMin.Min(arPt[i]);
        m_vMax.Max(arPt[i]);
    }
}

void RDBox::CreateBox(std::array<float3,8>& arPt)const
{
    arPt[0].Set(m_vMin.x(),m_vMax.y(),m_vMin.z());
    arPt[1].Set(m_vMax.x(),m_vMax.y(),m_vMin.z());
    arPt[2].Set(m_vMin.x(),m_vMax.y(),m_vMax.z());
    arPt[3] = m_vMax;//Set(m_vMax.x(),m_vMax.y(),m_vMax.z());
    arPt[4] = m_vMin;//Set(m_vMin.x(),m_vMin.y(),m_vMin.z());
    arPt[5].Set(m_vMax.x(),m_vMin.y(),m_vMin.z());
    arPt[6].Set(m_vMin.x(),m_vMin.y(),m_vMax.z());
    arPt[7].Set(m_vMax.x(),m_vMin.y(),m_vMax.z());
}

void RDBox::UnionBox(const RDBox& box,const matrix4x4& worldMat)
{
    std::array<float3,8> arPt;
    box.CreateBox(arPt);
    for(int i = 0; i < 8; i++)
    {
        arPt[i] *= worldMat;
        m_vMin.Min(arPt[i]);
        m_vMax.Max(arPt[i]);
    }
}
