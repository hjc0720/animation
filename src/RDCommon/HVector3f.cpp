/*
 * =====================================================================================
 *
 *       Filename:  HVector3f.cpp
 *
 *    Description:  3D float vector
 *
 *        Version:  1.0
 *        Created:  2011年03月02日 22时27分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang Jianchao (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include "HVector3f.h"
#include <string.h>
#include "HVector4f.h"
#include <cmath>
#include <smmintrin.h>

float3::float3()
{
    memset(m_data,0,3 * sizeof(float));
}

float3::float3(const float3& src)
{
    if(this == &src)
        return;
    memcpy(m_data,src.m_data,3 * sizeof(float));
}

float3::float3(const float4& src)
{
    memcpy(m_data,src.GetData(),3 * sizeof(float));
    m_data[3] = 0;
}

float3::float3(float fx,float fy,float fz)
{
    Set(fx,fy,fz);
}

void float3::Set(float fx,float fy,float fz)
{
    __m128 fm;
    fm = _mm_set_ps(0,fz,fy,fx);
    _mm_store_ps(m_data,fm);
}

float float3::Mode ()const
{
	float sum = 0;
    for(int i  = 0; i < 3; i++)
        sum += m_data[i] * m_data[i];
    return sqrtf(sum);
}		// -----  end of method float3::Mode  -----

void float3::Normalize ()
{
	float fMode = Mode();
    if(fMode == 0)
        return;
    else
        fMode = 1 / fMode;
    *this *= fMode;
}		// -----  end of method float3::Normalize  -----

float3& float3::operator = (const float4& m)
{
    memcpy(m_data,m.GetData(),3 * sizeof(float));
    return *this;
}
float3& float3::operator += (const float3& m)
{
    float4 dst(*this);
    float4 src(m);
    *this = dst + src;
    return *this;
}
float3& float3::operator -= (const float3& m)
{
    float4 dst(*this);
    float4 src(m);
    *this = dst - src;
    return *this;
}

float3& float3::operator /= (float fScale)
{
    float4 dst(*this);
    *this = dst / fScale;
    return *this;
}

float3& float3::operator *= (const HMatrixQ4F& m)
{
    float4 dst(*this);
    *this = dst * m;
    return *this;
}

float3& float3::operator *= (float fScale)
{
    float4 dst(*this);
    *this = dst * fScale;
    return *this;
}
float3& float3::operator *= (const float3& vSrc)
{	
    float4 src1(GetY() * vSrc.GetZ(),GetZ() * vSrc.GetX(),GetX() * vSrc.GetY(),0);
    float4 src2(GetZ() * vSrc.GetY(),GetX() * vSrc.GetZ(),GetY() * vSrc.GetX(),0);
    *this = src1 - src2;
    return *this;
}


//float operator * (const float3& v1,const float3& v2)
//{
//    float4 vec1(v1,0),vec2(v2,0);
//    return vec1 * vec2;
//}
