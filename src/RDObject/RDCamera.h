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
public:
    RDCamera(const QString& strName,uint nHeight,RDProjectType nType);
    virtual void CalFrame(const RDTime& nTime,const QString& pRDName) ;
    void    UpdateProject(const QString& pRDName,QRectF& rt,float fZNear,float fFar);

    const matrix4x4&    GetViewMatrix(const QString& pRDName);
    virtual const matrix4x4&    GetViewProjMat(const QString& pRDName);
protected:
    virtual RDRenderData*  CreateRenderData(const QString& pName);
protected:
    RDProjectType   m_nProjType;
//    float3  m_vEyePos;
    float3  m_vUp;
    float3  m_vLookAt;
};
#endif

