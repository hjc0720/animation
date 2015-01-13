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
/////////////////////////////////////////////////////////////////////////////////////////////////
struct RDRay
{
    float3 vStart;
    float3 vDir;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief RDSpaceParam::Convert3DTo2D
/// \param vPos
/// \return 2D pos
float3 RDSpaceParam::Convert3DTo2D(const float3 &vPos) const
{
    float4 vTmpPos(vPos);
    vTmpPos = vTmpPos * *m_pWorldMat * *m_pViewMat * *m_pProjMat;
    vTmpPos.DividW();

    return float3((vTmpPos.x() + 1) * 0.5f * m_rtViewPort.width() + m_rtViewPort.left(),
                  (1 - vTmpPos.y()) * 0.5f * m_rtViewPort.height() + m_rtViewPort.top(),
                  0);
}

float3 RDSpaceParam::Convert2DTo3D(const float3 &vPoint) const
{
    float4 vOut;
    vOut.SetX((vPoint.x() -  m_rtViewPort.left())/m_rtViewPort.width()*2 - 1);
    vOut.SetY(1 - (vPoint.y() - m_rtViewPort.top())/m_rtViewPort.height() * 2) ;
    vOut.SetZ(-1);

    matrix4x4 mat = *m_pWorldMat * *m_pViewMat * *m_pProjMat;
    mat.Inverse();
    vOut *= mat;
    vOut.DividW();
    return vOut;
}

float3 RDSpaceParam::Convert2DTo3D(const float3 &vPoint, float fZValue) const
{
    if(fZValue >= 0)
        return m_vEyePos;
    float4 vOut;
    vOut.SetX((vPoint.x() -  m_rtViewPort.left())/m_rtViewPort.width()*2 - 1);
    vOut.SetY(1 - (vPoint.y() - m_rtViewPort.top())/m_rtViewPort.height() * 2) ;
    vOut.SetZ(-1);

    matrix4x4 mat(*m_pProjMat);
    mat.Inverse();
    vOut *= mat;
    vOut.DividW();

    vOut = vOut * fZValue / vOut.z();
    vOut.SetW(1);
    matrix4x4 worldInv(*m_pWorldMat * *m_pViewMat);
    worldInv.Inverse();
    return vOut * worldInv;
}

bool RDSpaceParam::HitSphere(const float3 &vPt, float fRadius, float3 &vHitPt)
{
    RDRay ray;
    GetRay(vPt,ray);
    float fB = 2 * ray.vStart ^ ray.vDir;
    float fC = ray.vStart.Mode2() - fRadius * fRadius;
    float det = fB * fB - 4 * fC;
    if(det < 0)
        return false;
    float t = (-fB - sqrtf(det)) * 0.5;
    if(t > 0)
    {
        vHitPt = ray.vStart + ray.vDir * t;
    }
    else
    {
        t = (-fB + sqrtf(det)) * 0.5;
        vHitPt = ray.vStart + ray.vDir * t;
    }
    return true;
}

bool RDSpaceParam::HitTriangle(float3 &vHitPt, const float3 &vPt, const float3 &vPt0, const float3 &vPt1, const float3 &vPt2, bool bCull) const
{
    RDRay ray;
    GetRay(vPt,ray);

    float3 edg1(vPt1 - vPt0);
    float3 edg2(vPt2 - vPt0);
    float3 vec(ray.vDir * edg2 );
    float det = edg1 ^ vec;
    if(bCull && det > 0)
        return false;
    float sign =  det < 0 ? -1.f : 1.f;
    det = sign * det;

    float3 tVec(sign * (ray.vStart - vPt0));
    float u = tVec ^ vec;
    if(u < 0 || u > det)
        return false;

    float3 qVec( tVec * edg1 );
    float v = ray.vDir ^ qVec;
    if(v < 0 || u + v > det)
        return false;
    float t = edg2 ^ qVec;
    vHitPt = ray.vStart + t / det * ray.vDir;
    return true;
}

void RDSpaceParam::GetRay(const float3 &vPt, RDRay &ray)const
{
    float3 vNearPt = Convert2DTo3D(vPt);
    matrix4x4 worldInv(*m_pWorldMat);
    worldInv.Inverse();
    ray.vStart = m_vEyePos * worldInv;
    ray.vDir = vNearPt - ray.vStart ;
    ray.vDir.Normalize();
}


RDSpaceParam::RDSpaceParam()
    :m_pWorldMat(nullptr)
    ,m_pViewMat(nullptr)
    ,m_pProjMat(nullptr)
    ,m_fZValue(0)
{
}

RDSpaceParam::RDSpaceParam(const matrix4x4 *pWorld, const matrix4x4 *pViewMat, const matrix4x4 *pProjMat, const QRectF &viewPort)
    :m_pWorldMat(pWorld)
    ,m_pViewMat(pViewMat)
    ,m_pProjMat(pProjMat)
    ,m_rtViewPort(viewPort)
{
    matrix4x4 viewInv(*pViewMat);
    viewInv.Inverse();
    m_vEyePos = float3::GetZero() * viewInv;
    m_fZValue = -m_rtViewPort.width() * m_pProjMat->Get(0,0) * 0.5;
}

float3 RDSpaceParam::ConvertWorldToView(const float3& vPos)const
{
    matrix4x4 matWorldViewInv(*m_pWorldMat * *m_pViewMat);
    matWorldViewInv.Inverse();
	return vPos * matWorldViewInv;
}
