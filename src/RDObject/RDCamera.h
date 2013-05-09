/*
 * =====================================================================================
 *
 *       Filename:  RDCamera.h
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

#ifndef __RD_CAMERA_H
#define __RD_CAMERA_H
#include "RDNode.h"

enum RDProjectType
{
    OrthoProject,
    PerspectiveProject,
};

class RDCamera :public RDNode
{
    RDCamera(const QString& strName,size_t nWidth,size_t nHeight,RDProjectType nType);
protected:
    RDProjectType   m_nProjType;
    float   m_fAspect;
//    float3  m_vEyePos;
    float3  m_vUp;
    float3  m_vLookAt;
};
#endif

