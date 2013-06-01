// =====================================================================================
// 
//       Filename:  RDSpaceConvert.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/23/2011 04:20:47 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RD_SPACE_CONVERT_INC
#define  RD_SPACE_CONVERT_INC
class float3;
class HMatrixQ4F;
void RDSceneToBuffer(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop);
void RDBufferToScene(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop);

void FillBox(float3 vBox[],const float3& vMin,const float3 vFar);
void RDCalBoxNearFar(float& fNear,float& fFar,const float3& vMin,const float3 vFar,const HMatrixQ4F& view,const HMatrixQ4F& world);
#endif   // ----- #ifndef RD_SPACE_CONVERT_INC  -----
