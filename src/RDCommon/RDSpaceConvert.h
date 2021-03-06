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
class matrix4x4;
void RDSceneToBuffer(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop);
void RDBufferToScene(float3& pOut,const float3& pIn,float fBufferLeft,float fBufferTop);

struct RDRay;
class RDSpaceParam
{
public:
    RDSpaceParam();
    RDSpaceParam(const matrix4x4* pWorld,const matrix4x4* pViewMat,const matrix4x4* pProjMat,const QRectF& viewPort);
    
    void SetViewPort(const QRectF& viewPort){m_rtViewPort = viewPort;}
    void SetWorldMat(const matrix4x4* pWorldMat){m_pWorldMat = pWorldMat;}
    void SetViewMat(const matrix4x4* pViewMat){m_pViewMat = pViewMat;}
    void SetProjMat(const matrix4x4* pProjMat){m_pProjMat = pProjMat;}
    float GetZValue()const{return m_fZValue;}
    
    float3 Convert3DTo2D(const float3& vPos)const;
    float3 Convert2DTo3D(const float3& vPoint) const;
    float3 Convert2DTo3D(const float3& vPoint,float fZValue) const;
	float3 ConvertWorldToView(const float3& vPos)const;

    bool HitSphere(const float3& vPt,float fRadius,float3& vHitPt);
    bool HitTriangle(float3& vHitPt,const float3& vPt,const float3& vP0,const float3& vPt1,const float3& vPt2,bool bCull = true)const;
protected:
    void GetRay(const float3& vPt,RDRay& ray) const;
protected:
    const matrix4x4* m_pWorldMat;
    const matrix4x4* m_pViewMat;
    const matrix4x4* m_pProjMat;
    QRectF m_rtViewPort;
    float  m_fZValue;
    
    float3 m_vEyePos;
    //float2 m_vNearFar;
};

#endif   // ----- #ifndef RD_SPACE_CONVERT_INC  -----
