// =====================================================================================
// 
//       Filename:  RDSpaceConvert.cpp
// 
//    Description:  This file is used for convert the object point to scene point,and scene point to buffer point
// 
//        Version:  1.0
//        Created:  07/23/2011 04:18:32 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDSpaceConvert.h"
#include "HVector4f.h"
#include "HVector3f.h"
#include "HMatrixQ4F.h"
#include <cfloat>
#include <algorithm>

void RDSceneToBuffer(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop)
{
    float3 vOffset(fBufferLeft,fBufferTop,0);
    pOut = pIn - vOffset;
    pOut.SetY(-pOut.y());
}
void RDBufferToScene(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop)
{
    float3 vOffset(fBufferLeft,-fBufferTop,0);
    pOut = pIn + vOffset;
    pOut.SetY(-pOut.y());
}

void RDCalBoxMinMax(float& fMin,float& fMax,const float3& vMin,const float3& vMax,const HMatrixQ4F& WorldView)
{
    float3 vBox[8];
    FillBox(vBox,vMin,vMax);
    for(int i = 0; i < 8; i++)
    {
        float fZValue = (vBox[i] * WorldView).z();
        fMin = std::min(fMin,fZValue);
        fMax = std::max(fMax,fZValue);
    }
}

void FillBox(float3 vBox[],const float3& vMin,const float3& vMax)
{
    vBox[0] = vMin;
    vBox[1].Set(vMax.x(),vMin.y(),vMin.z());
    vBox[2].Set(vMax.x(),vMin.y(),vMax.z());
    vBox[3].Set(vMin.x(),vMin.y(),vMax.z());
                               
    vBox[4].Set(vMin.x(),vMax.y(),vMin.z());
    vBox[1].Set(vMax.x(),vMax.y(),vMin.z());
    vBox[2] = vMax;           
    vBox[3].Set(vMin.x(),vMax.y(),vMax.z());
}

/////////////////////////////////////////////////////////////////////////////////////////////////
float3 RDSpaceParam::Convert3DTo2D(const float3 &vPos)
{
    float4 vTmpPos(vPos);
    vTmpPos *= *m_pWorldMat * *m_pViewMat * *m_pProjMat;
    vTmpPos.DividW();

    return float3((vTmpPos.x() + 1) * 0.5f * m_rtViewPort.width() + m_rtViewPort.left(),
                  (1 - vTmpPos.y()) * 0.5f * m_rtViewPort.height() + m_rtViewPort.top(),
                  0);
}

float3 RDSpaceParam::Convert2DTo3D(const float3 &vPoint)
{
    float4 vOut;
    vOut.SetX((vPoint.x() -  m_rtViewPort.left())/m_rtViewPort.width()*2 - 1);
    vOut.SetY(1 - (vPoint.x() - m_rtViewPort.top())/m_rtViewPort.height() * 2) ;
    vOut.SetZ(-1);

    HMatrixQ4F mat = *m_pWorldMat * *m_pViewMat * *m_pProjMat;
    mat.Inverse();
    vOut = vOut * mat;
    vOut.DividW();
    return vOut;
}
