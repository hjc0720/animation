/*
 * =====================================================================================
 *
 *       Filename:  HVector3f.h
 *
 *    Description:  3D float vector
 *
 *        Version:  1.0
 *        Created:  2011年03月02日 22时28分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang Jianchao (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __HVECTOR_3F__
#define __HVECTOR_3F__
#include <cstring>

class matrix4x4;
class float4;

class float3
{
public:
    static const float3& GetZero();
public:
    float3();
    float3(const float3& src);
    float3(const float4& src);
    float3(float fx,float fy,float fz);
    float Mode()const;
    float Mode2()const;
    void Normalize();
	
    inline const float* GetData()const{return m_data;}
    void Set(float fx,float fy,float fz);

    inline float GetX()const{return m_data[0];}
    inline float GetY()const{return m_data[1];}
    inline float GetZ()const{return m_data[2];}
    inline float x()const{return m_data[0];}
    inline float y()const{return m_data[1];}
    inline float z()const{return m_data[2];}

    inline void SetX(float fX ){m_data[0] = fX;}
    inline void SetY(float fY ){m_data[1] = fY;}
    inline void SetZ(float fZ ){m_data[2] = fZ;}
    
public:
    //operator overriding
    float3& operator = (const float4& m);
    float3& operator += (const float3& m);
    float3& operator -= (const float3& m);
    float3& operator /= (float fScale);
    float3& operator *= (const matrix4x4& m);
    float3& operator *= (float fScale);
    float3& operator *= (const float3& vSrc);
    bool operator == (const float3& vSrc);
    bool operator != (const float3& vSrc);
    float operator ^ (const float3& vSrc);
protected:
    float m_data[4] __attribute__ ((aligned (16)));
};

//operator
inline bool float3::operator == (const float3& vSrc)
{
    return (m_data[0] == vSrc.m_data[0] &&
            m_data[1] == vSrc.m_data[1] &&
            m_data[2] == vSrc.m_data[2] );
}

inline bool float3::operator != (const float3& vSrc)
{
    return !(*this == vSrc);
}

inline float3 operator + (const float3& v1,const float3& v2)
{
    float3 ret(v1);
    ret += v2;
    return ret;
}
inline float3 operator - (const float3& v1,const float3& v2)
{
    float3 ret(v1);
    ret -= v2;
    return ret;
}
inline float3 operator / (const float3& v1,float fScale)
{
    float3 ret(v1);
    ret /= fScale;
    return ret;
}
inline float3 operator * (const float3& v1,const matrix4x4& m)
{
    float3 ret(v1);
    ret *= m;
    return ret;
}
inline float3 operator * (const float3& v1,float fScale)
{
    float3 ret(v1);
    ret *= fScale;
    return ret;
}
inline float3 operator * (float fScale,const float3& v1)
{
    float3 ret(v1);
    ret *= fScale;
    return ret;
}
inline float3 operator * (const float3& v1,const float3& v2)
{
    float3 ret(v1);
    ret *= v2;
    return ret;
}

#endif
