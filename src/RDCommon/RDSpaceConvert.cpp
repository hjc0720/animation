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
