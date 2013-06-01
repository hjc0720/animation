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
#include "HMatrixQ4F.h"

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

void RDCalBoxNearFar(float& fNear,float& fFar,const float3& vMin,const float3 vFar,const HMatrixQ4F& WorldView)
{
    
}

void FillBox(float3 vBox[],const float3& vMin,const float3 vFar)
{
}
