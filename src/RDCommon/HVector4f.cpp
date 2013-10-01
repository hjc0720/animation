/*
 * =====================================================================================
 *
 *       Filename:  float4.cpp
 *
 *    Description:  4D float vector
 *
 *        Version:  1.0
 *        Created:  2011年03月03日 07时52分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang Jianchao, hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "HVector4f.h"
#include "HVector3f.h"
#include <cmath>
#include <string.h>
#include "HMatrixQ4F.h"
#include <smmintrin.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4::float4()
{
    __m128 fm = _mm_setzero_ps();
    _mm_store_ps(m_data,fm);
    m_data[3] = 1;
}
float4::float4(const float4& src)
{
    if(this == &src)
        return;
    __m128 tmp = _mm_load_ps(src.m_data);
    _mm_store_ps(m_data,tmp);
}
float4::float4(float x,float y,float z,float w)
{
    __m128 fm = _mm_set_ps(w,z,y,x);
    _mm_store_ps(m_data,fm);
}

float4::float4(unsigned int dwColor)
{
    __m128i zero = _mm_setzero_si128();
    __m128i color = _mm_cvtsi32_si128((int)dwColor);
    __m128i color16 = _mm_unpacklo_epi8(color,zero);
    __m128i color32 = _mm_unpacklo_epi16(color16,zero);
    __m128 vec = _mm_cvtepi32_ps(color32);
    __m128 scale = _mm_set_ps1(1.f/255.f);
    vec = _mm_mul_ps(vec,scale);
    _mm_store_ps(m_data,vec);
}

float float4::Mode()const
{
    __m128 data = _mm_load_ps(m_data);
    __m128 ret = _mm_dp_ps(data,data,0xf1);
    float fMode;
    _mm_store_ss(&fMode,ret);
    return sqrtf(fMode);
}

float float4::Mode2() const
{
    __m128 data = _mm_load_ps(m_data);
    __m128 ret = _mm_dp_ps(data,data,0xf1);
    float fMode;
    _mm_store_ss(&fMode,ret);
    return fMode;
}

void float4::Normalize()
{
    float fMode = Mode();
    if(fMode == 0)
        return;
    else
        fMode = 1 / fMode;
    *this *= fMode;
}

void float4::Set(float x, float y, float z, float w)
{
    __m128 fm;
    fm = _mm_set_ps(w,z,y,x);
    _mm_store_ps(m_data,fm);
}

float4& float4::operator = (const float4& other)
{
    __m128 src = _mm_load_ps(other.m_data);
    _mm_store_ps(m_data,src);
    return *this;
}

float4& float4::operator += (const float4& m)
{
    __m128 src = _mm_load_ps(m_data);
    __m128 dst = _mm_load_ps(m.m_data);
    src = _mm_add_ps(src,dst);
    _mm_store_ps(m_data,src);
    return *this;
}
float4& float4::operator -= (const float4& m)
{
    __m128 src = _mm_load_ps(m_data);
    __m128 dst = _mm_load_ps(m.m_data);
    src = _mm_sub_ps(src,dst);
    _mm_store_ps(m_data,src);
    return *this;
}
float4& float4::operator *= (const matrix4x4& m)// m could be the same of this
{
    __m128 dst = _mm_setzero_ps();
    for(int i = 0; i < 4; i++)
    {
        __m128 v;
        __m128 mat;
        v = _mm_set_ps1(m_data[i]);
        mat = _mm_load_ps(m.GetLine(i));
        v = _mm_mul_ps(v,mat);
        dst = _mm_add_ps(dst,v);
    }
    _mm_store_ps(m_data,dst);
    return *this;
}
float4& float4::operator *= (float fScale)
{
    __m128 vec = _mm_load_ps(m_data);
    __m128 scale = _mm_set_ps1(fScale);
    vec = _mm_mul_ps(vec,scale);
    _mm_store_ps(m_data,vec);
    return *this;
}

float4& float4::operator /= (float fScale)
{
    if(fScale == 0)
        return *this;
    fScale = 1 / fScale;
    *this *= fScale;
    return *this;
}
float4::float4(const float3& v,float w /*= 1*/)
{
    __m128 src = _mm_load_ps(v.GetData());
    _mm_store_ps(m_data,src);
    m_data[3] = w;
}
void float4::DividW()
{
    if(m_data[3] == 0)
        return;
    __m128 vec = _mm_load_ps(m_data);
    __m128 w = _mm_set_ps1(1 / m_data[3]);
    vec = _mm_mul_ps(vec,w);
    _mm_store_ps(m_data,vec);
}

float float4::operator ^ (const float4& v2)
{
    __m128 vec1 = _mm_load_ps(GetData());
    __m128 vec2 = _mm_load_ps(v2.GetData());
    __m128 ret = _mm_dp_ps(vec1,vec2,0xf1);
    float sum = 0;
    _mm_store_ss(&sum,ret);
    return sum;
}

bool float4::operator == (const float4& m)
{
    __m128 src = _mm_load_ps(m_data);
    __m128 dst = _mm_load_ps(m.m_data);
    __m128 ret = _mm_cmpeq_ps(src,dst);
    return _mm_movemask_ps(ret) == 0xf;
}
