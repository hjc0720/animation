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
    RDCamera(){};
    RDCamera(const std::string& strName,uint nHeight,RDProjectType nType);
    virtual void Serialize(RDJsonDataStream& buffer,Json::Value& parent,bool bSave)override;
    virtual void CalFrame(const RDTime& nTime,const std::string& pRDName) override;
    void    UpdateProject(const std::string& pRDName,QRectF& rt,float fZNear,float fFar);

    const matrix4x4&    GetViewMatrix(const std::string& pRDName);
    const matrix4x4&    GetRenderProjMatrix(const std::string& pRDName);
    const matrix4x4&    GetEditProjMatrix(const std::string &pRDName);
    virtual const matrix4x4&    GetViewProjMat(const std::string& pRDName)override;
protected:
    virtual RDRenderData*  CreateRenderData(const std::string& pName)override;
protected:
    RDProjectType   m_nProjType;
    float3  m_vUp;
    float3  m_vLookAt;
};
#endif

