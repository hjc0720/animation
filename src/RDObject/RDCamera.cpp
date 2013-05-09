/*
 * =====================================================================================
 *
 *       Filename:  RDCamera.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/25/13 22:48:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "RDCamera.h"
#include "HMatrixQ4F.h"

class RDCameraPrivateData :public RDRenderData
{
public:
    float       m_fZNear;
    float       m_fZFar;

    float3      m_vEyePos;
    float3      m_vLookAt;
    float3      m_vUp;

    HMatrixQ4F  m_matView;
    HMatrixQ4F  m_matProj;
    HMatrixQ4F  m_matViewProj;
};

RDCamera::RDCamera(const QString& strName,size_t nWidth,size_t nHeight,RDProjectType nType)
    :RDNode(strName,float3(0,0,(float)nHeight),nullptr)
     ,m_nProjType(nType)
     ,m_vUp(0,1,0)
     ,m_vLookAt(0,0,0)
{
    m_fAspect = nWidth / (float)nHeight;
}
