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
#include "HVector3f.h"
#include "HVector4f.h"
#include <QRectF>
class float3;
class HMatrixQ4F;
void RDSceneToBuffer(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop);
void RDBufferToScene(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop);

void FillBox(float3 vBox[],const float3& vMin,const float3& vMax);
void RDCalBoxMinMax(float& fNear,float& fFar,const float3& vMin,const float3& vMax,const HMatrixQ4F& WorldView);

struct RDRay;
class RDSpaceParam
{
public:
    RDSpaceParam();
    RDSpaceParam(const HMatrixQ4F* pWorld,const HMatrixQ4F* pViewMat,const HMatrixQ4F* pProjMat,const QRectF& viewPort);
    
    void SetViewPort(const QRectF& viewPort){m_rtViewPort = viewPort;}
    void SetWorldMat(const HMatrixQ4F* pWorldMat){m_pWorldMat = pWorldMat;}
    void SetViewMat(const HMatrixQ4F* pViewMat){m_pViewMat = pViewMat;}
    void SetProjMat(const HMatrixQ4F* pProjMat){m_pProjMat = pProjMat;}
    
    float3 Convert3DTo2D(const float3& vPos);
    float3 Convert2DTo3D(const float3& vPoint);

    bool HitSphere(const float3& vPt,float fRadius,float3& vHitPt);
protected:
    void GetRay(const float3& vPt,RDRay& ray);
protected:
    const HMatrixQ4F* m_pWorldMat;
    const HMatrixQ4F* m_pViewMat;
    const HMatrixQ4F* m_pProjMat;
    QRectF m_rtViewPort;
    
    float3 m_vEyePos;
    //float2 m_vNearFar;
};

#endif   // ----- #ifndef RD_SPACE_CONVERT_INC  -----
