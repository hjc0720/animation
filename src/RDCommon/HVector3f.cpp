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
#include "HVector4f.h"
#include <cmath>
#include <smmintrin.h>

const float3 vZero;
const float3& float3::GetZero()
{
    return vZero;
}

float3::float3()
{
    __m128 zero = _mm_setzero_ps();
    _mm_storel_pi((__m64*)m_data,zero);
    _mm_store_ss(m_data + 2,zero);
}

float3::float3(const float2& src)
{
    __m128 zero = _mm_setzero_ps();
    __m128 vec = _mm_loadl_pi(zero,(__m64*)&src);
    _mm_storel_pi((__m64*)m_data,vec);
    _mm_store_ss(m_data + 3,vec);
}

float3::float3(const float3& src)
{
    if(this == &src)
        return;
    memcpy(m_data,src.m_data,sizeof(m_data));
}

float3::float3(const float4& src)
{
    memcpy(m_data,src.GetData(),sizeof(m_data));
}

float3::float3(float fx,float fy,float fz)
{
    Set(fx,fy,fz);
}

void float3::Set(float fx,float fy,float fz)
{
    __m128 fm = _mm_set_ps(0,fz,fy,fx);
    __m128 l = _mm_movehl_ps(fm,fm);
    _mm_storel_pi((__m64*)m_data,fm);
    _mm_store_ss(m_data + 2,l);
}

float float3::Mode ()const
{
    return sqrtf(Mode2());
}

float float3::Mode2() const
{
    __m128 high = _mm_load_ss(m_data+2);
    high = _mm_movelh_ps(high,high);
    __m128 vec = _mm_loadl_pi(high,(__m64*)m_data);
    __m128 rt = _mm_dp_ps(vec,vec,0x77);
    float fRt;
    _mm_store_ss(&fRt,rt);
    return fRt;
}		// -----  end of method float3::Mode  -----

void float3::Normalize ()
{
    __m128 high = _mm_load_ss(m_data+2);
    high = _mm_movelh_ps(high,high);
    __m128 vec = _mm_loadl_pi(high,(__m64*)m_data);
    __m128 mode = _mm_dp_ps(vec,vec,0x77);
    mode = _mm_sqrt_ps(mode);
    vec = _mm_div_ps(vec,mode);
    high = _mm_movehl_ps(vec,vec);
    _mm_storel_pi((__m64*)m_data,vec);
    _mm_store_ss(m_data + 2, high);
}		// -----  end of method float3::Normalize  -----

float3& float3::operator = (const float4& m)
{
    memcpy(m_data,m.GetData(),3 * sizeof(float));
    return *this;
}

float3& float3::operator += (const float3& m)
{
    __m128 high = _mm_load_ss(m_data+2);
    high = _mm_movelh_ps(high,high);
    __m128 vec1 = _mm_loadl_pi(high,(__m64*)m_data);

    high = _mm_load_ss(m.GetData() + 2);
    high = _mm_movelh_ps(high,high);
    __m128 vec2 = _mm_loadl_pi(high,(__m64*)m.GetData());
    
    __m128 rt = _mm_add_ps(vec1,vec2);

    high = _mm_movehl_ps(rt,rt);
    _mm_storel_pi((__m64*)m_data,rt);
    _mm_store_ss(m_data + 2, high);
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

float3& float3::operator *= (const matrix4x4& m)
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
    float4 src1(y() * vSrc.z(),z() * vSrc.x(),x() * vSrc.y(),0);
    float4 src2(z() * vSrc.y(),x() * vSrc.z(),y() * vSrc.x(),0);
    *this = src1 - src2;
    return *this;
}

float float3::operator ^(const float3 &vSrc)
{
    float4 vec1(*this,0),vec2(vSrc,0);
    return vec1 ^ vec2;
}
