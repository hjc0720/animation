/*
 * =====================================================================================
 *
 *       Filename:  RDBox.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年04月22日 21时05分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang JianChao (hjc), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  RDBOX_INC
#define  RDBOX_INC
#include "HVector3f.h"
#include <array>
#include <cfloat>

class RDBox
{
public:
    RDBox():m_vMin(FLT_MAX,FLT_MAX,FLT_MAX),m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX){};
    RDBox(const float3& vMin,const float3& vMax):m_vMin(vMin),m_vMax(vMax){};
    const float3& GetMin()const{return m_vMin;}
    const float3& GetMax()const{return m_vMax;}
    void TransBox(const matrix4x4& worldMat);
    void UnionBox(const RDBox& box,const matrix4x4& worldMat);

protected:
    void CreateBox(std::array<float3,8>& stPt)const;
protected:
    float3 m_vMin;
    float3 m_vMax;
};

#endif   /* ----- #ifndef RDBox_INC  ----- */
