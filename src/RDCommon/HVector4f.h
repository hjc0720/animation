/*
 * =====================================================================================
 *
 *       Filename:  float4.h
 *
 *    Description:  4D float vector
 *
 *        Version:  1.0
 *        Created:  2011年03月03日 07时52分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang Jianchao (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __4D_VECTOR_F_H_
#define __4D_VECTOR_F_H_

class matrix4x4;
class float3;


struct float2
{
    float x;
    float y;
};

class float4
{
public:
    float4();
    float4(const float4& src);
    float4(float fx,float y,float z,float w);
    float4(const float3& v,float w = 1);
    float4(unsigned int color);

    const float* GetData()const{return m_data;}
    float Mode()const;
    float Mode2()const;
    void Normalize();

    float x()const{return m_data[0];}
    float y()const{return m_data[1];}
    float z()const{return m_data[2];}
    float w()const{return m_data[3];}

    void SetX(float f){m_data[0] = f;}
    void SetY(float f){m_data[1] = f;}
    void SetZ(float f){m_data[2] = f;}
    void SetW(float f){m_data[3] = f;}
    void Set(float x, float y,float z, float w );

    void DividW();
public:
    //operator overriding
    float4& operator += (const float4& m);
    float4& operator -= (const float4& m);
    float4& operator *= (const matrix4x4& m);
    float4& operator *= (float fScale);
    float4& operator /= (float fScale);
    bool operator == (const float4& m);
    bool operator != (const float4& m)
    {
        return !(*this == m);
    }
    float4& operator = (const float4& other);
    float operator ^ (const float4& vSrc);
protected:
    float m_data[4] __attribute__ ((aligned (16)));
};
//operator
//inline operator
inline float4 operator + (const float4& v1,const float4& v2)
{
    float4 ret(v1);
    ret += v2;
   return ret;
}
inline float4 operator - (const float4& v1,const float4& v2)
{
    float4 ret(v1);
    ret -= v2;
   return ret;
}
inline float4 operator * (const float4& v1,const matrix4x4& mat)
{
    float4 ret(v1);
    ret *= mat;
   return ret;
}

inline float4 operator * (float fScale,const float4& v)
{
    float4 ret(v);
    ret *= fScale;
    return ret;
}

inline float4 operator * (const float4& v,float fScale)
{
    float4 ret(v);
    ret *= fScale;
    return ret;
}
inline float4 operator / (const float4& v,float fScale)
{
    float4 ret(v);
    ret /= fScale;
    return ret;
}
#endif
