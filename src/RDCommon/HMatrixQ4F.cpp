/*
 * =====================================================================================
 *
 *       Filename:  HMatrixQ4F.cpp
 *
 *    Description:  四维矩阵，采用sse加速
 *
 *        Version:  1.0
 *        Created:  2010年12月19日 19时25分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  黄建超 (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include "HMatrixQ4F.h"
#include <string.h>
#include <cmath>
#include "HMath.h"
#include "HVector4f.h"
#include "HVector3f.h"
//#include <iostream>
//using namespace std;

#ifdef __SSE2__
#include <smmintrin.h>
#endif

inline __m128 CrossProduct(__m128 a, __m128 b)
{
  return _mm_sub_ps(
    _mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2))), 
    _mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1)))
  );
}

const matrix4x4& matrix4x4::CreateViewMat(matrix4x4& mat,const float3& vEyePos,const float3& vUp,const float3& vLookAt)
{
    float4 Eye(vEyePos,0),Up(vUp,0),LookAt(vLookAt,0);
    __m128 eye = _mm_load_ps(Eye.GetData());
    __m128 up = _mm_load_ps(Up.GetData());
    __m128 look = _mm_load_ps(LookAt.GetData());

    __m128 one = _mm_set_ps1(1);
    __m128 vZ = _mm_sub_ps(eye,look);
    vZ = _mm_mul_ps(vZ,_mm_div_ps(one,_mm_sqrt_ps(_mm_dp_ps(vZ,vZ,0xff))));

    __m128 vX = CrossProduct(up,vZ);
    vX = _mm_mul_ps(vX,_mm_div_ps(one,_mm_sqrt_ps(_mm_dp_ps(vX,vX,0xff))));

    __m128 vY = CrossProduct(vZ,vX);

    _mm_store_ps(mat.m[0],vX);
    _mm_store_ps(mat.m[1],vY);
    _mm_store_ps(mat.m[2],vZ);

    __m128 dp1 = _mm_dp_ps(vX,eye,0x71);
    __m128 dp2 = _mm_dp_ps(vY,eye,0x72);
    __m128 dp3 = _mm_dp_ps(vZ,eye,0x74);
    __m128 zero = _mm_setzero_ps();
    dp1 = _mm_sub_ps(zero,dp1);
    dp1 = _mm_sub_ps(dp1,dp2);
    dp1 = _mm_sub_ps(dp1,dp3);
    _mm_store_ps(mat.m[3],dp1);
    mat.m[3][3] = 1;

    return mat;
}

const matrix4x4& matrix4x4::CreateProjectMat(matrix4x4& mat,float l,float r,float t,float b,float zn,float zf)
{
    float r_l = 1 / (r - l);
    float t_b = 1 / (t - b);
    float z = 1 / (zn - zf);
    mat.m[2][0] = (l + r) * (r_l);
    mat.m[2][1] = (t + b) * (t_b);
    mat.m[2][2] = (zn +zf) * z;
    mat.m[2][3] = -1;
    mat.m[3][3] = 0;

    __m128 m1 = _mm_set_ps(z,2 * zf * z,2 * t_b,2 * r_l);
    __m128 m2 = _mm_set_ps1(zn);
    m1 = _mm_mul_ps(m1,m2);

    _mm_store_ss(&mat.m[0][0],m1);
    
    m1 = (__m128)_mm_srli_si128((__m128i)m1,4);
    _mm_store_ss(&mat.m[1][1],m1);
    
    m1 = (__m128)_mm_srli_si128((__m128i)m1,4);
    _mm_store_ss(&mat.m[3][2],m1);

    return mat;
}

matrix4x4::matrix4x4()
{
    Identity();
}

void matrix4x4::Identity()
{
#ifdef __SSE2__
    __m128 fm;
    fm = _mm_setzero_ps();
              
    for(int i = 0; i < 4; i++)
        _mm_store_ps(m[i],fm);
    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;
    m[3][3] = 1;
#else
   memset(m,0,16 * sizeof(float)); 
   m[0][0] = 1;
   m[1][1] = 1;
   m[2][2] = 1;
   m[3][3] = 1;
#endif
}

matrix4x4::matrix4x4(const float3& vec,HMatrixQ4F_TYPE nType)
{
    switch(nType)
    {
    case HMatrixQ4F_POS:
        PosMat(vec.x(),vec.y(),vec.z());
        break;
    case HMatrixQ4F_Rotate:
        RotateMat(vec.x(),vec.y(),vec.z());
        break;
    case HMatrixQ4F_Scale:
        ScaleMat(vec.x(),vec.y(),vec.z());
   }
}
matrix4x4::matrix4x4(float fX,float fY,float fZ,HMatrixQ4F_TYPE nType)
{
    switch(nType)
    {
    case HMatrixQ4F_POS:
        PosMat(fX,fY,fZ);
        break;
    case HMatrixQ4F_Rotate:
        RotateMat(fX,fY,fZ);
        break;
    case HMatrixQ4F_Scale:
        ScaleMat(fX,fY,fZ);
   }
}
// ┌ cosy * cosz + siny * sinx * sinz   siny * sinx * cosz - cosy * sinz    siny * cosx ┐
// │ cosx * sinz                        cosx * cosz                         -sinx       │
// ┕ cosy * sinx * sinz - siny * cosz	siny * sinz + cosy * sinx * cosz    cosy * cosx ┘
//
void matrix4x4::RotateMat(float fX,float fY,float fZ)
{
    Identity();
	float fCosY = cosf(fY), fSinY = sinf(fY);
	float fCosX = cosf(fX), fSinX = sinf(fX);
	float fCosZ = cosf(fZ), fSinZ = sinf(fZ);
//#ifdef __SSE2__
#if 0
    __m128 vZero = _mm_setzero_ps();
    __m128 vFirst =     _mm_set_ss(fCosX);
    __m128 vSecond =    _mm_set_ss(fSinX);
    __m128 vThird =     _mm_set_ss(fCosY);
    __m128 vFour =      _mm_set_ss(fSinY);
    __m128 vFive =      _mm_set_ss(fCosZ);
    __m128 vSix =       _mm_set_ss(fSinZ);
    __m128 vSeven =     _mm_set_ss(1);
    vZero = _mm_sub_ps(vZero,vSecond);
    vFour = _mm_shuffle_ps(vFour,vThird,0);
    vSix = _mm_shuffle_ps(vSix,vFive,0);
    vFirst = _mm_shuffle_ps(vFirst,vFirst,0);
    vSecond = _mm_shuffle_ps(vSecond,vSecond,0);
    vSeven = _mm_shuffle_ps(vSeven,vSeven,0x15);
    vThird = vFour;
    vFive = vSix;
    vSix = _mm_shuffle_ps(vSix,vSix,0x88);
    vFour = _mm_shuffle_ps(vFour,vFive,0x88);
    vThird = _mm_mul_ps(vThird,vSix);
    vFirst = _mm_mul_ps(vFirst,vFour);
    vFive = _mm_setzero_ps();
    vFive = _mm_sub_ps(vFive,vThird);
    vSecond = _mm_mul_ps(vSecond,vThird);
    vFive = _mm_shuffle_ps(vFive,vThird,0xc9);
    vZero = _mm_shuffle_ps(vZero,vFirst,0x44);
    vSecond = _mm_shuffle_ps(vSecond,vSecond,0x36);
    vSecond = _mm_add_ps(vSecond,vFive);
    vFour = vSecond;
    vFirst = _mm_shuffle_ps(vFirst,vZero,0x4e);
    vSecond = _mm_shuffle_ps(vSecond,vZero,0x67);
    vFour = _mm_shuffle_ps(vFour,vZero,0x78);
    
    _mm_store_ps(m[0],vSecond);
    _mm_store_ps(m[1],vFirst);
    _mm_store_ps(m[2],vFour);
    _mm_store_ps(m[3],vSeven);
#else
    float sinysinx = fSinY * fSinX;
    float cosysinx = fCosY * fSinX;
    m[0][0] = fCosY * fCosZ + sinysinx * fSinZ;
    m[0][1] = -fCosY * fSinZ + sinysinx * fCosZ;
    m[0][2] = fSinY * fCosX ;
    //m[0][3] = 0;
    m[1][0] = fCosX * fSinZ ;
    m[1][1] = fCosX * fCosZ ;
    m[1][2] = -fSinX;
    //m[1][3] = 0;
    m[2][0] = cosysinx * fSinZ - fSinY * fCosZ;
    m[2][1] = fSinY * fSinZ + cosysinx * fCosZ;
    m[2][2] = fCosY * fCosX;
    //m[2][3] = 0;
    //m[3][0] = 0;
    //m[3][1] = 0;
    //m[3][2] = 0;
    //m[3][3] = 1;
#endif
}

void matrix4x4::PosMat(float fX,float fY,float fZ)
{
     __m128 fm;
    fm= _mm_setzero_ps();
               
    for(int i = 0; i < 3; i++)
    {
        _mm_store_ps(m[i],fm);
        m[i][i] = 1;
    }
    __m128 pos = _mm_set_ps(1,fZ,fY,fX);
    _mm_store_ps(m[3],pos);
}

void matrix4x4::ScaleMat(float fX,float fY,float fZ)
{ 
    __m128 fm;
    fm = _mm_setzero_ps();
               
    for(int i = 0; i < 4; i++)
    {
        _mm_store_ps(m[i],fm);
    }
    m[0][0] = fX;
    m[1][1] = fY;
    m[2][2] = fZ;
    m[3][3] = 1.f;
}

void matrix4x4::Transpose()
{
    __m128 src[4];
    //_mm_prefetch(m,_MM_HINT_T0);
    for(int i = 0; i < 4; i++)
        src[i] = _mm_load_ps(m[i]);
    _MM_TRANSPOSE4_PS(src[0],src[1],src[2],src[3]);
    for(int i = 0; i < 4; i++)
        _mm_store_ps(m[i],src[i]);
}

matrix4x4& matrix4x4::operator += (const matrix4x4& mat)
{
    for(int i = 0; i < 4; i++)
    {
        __m128 first = _mm_load_ps(m[i]);
        __m128 second = _mm_load_ps(mat.m[i]);
        first = _mm_add_ps(first,second);
        _mm_store_ps(m[i],first);
    }
    return *this;
}

matrix4x4& matrix4x4::operator -= (const matrix4x4& mat)
{
    for(int i = 0; i < 4; i++)
    {
        __m128 first = _mm_load_ps(m[i]);
        __m128 second = _mm_load_ps(mat.m[i]);
        first = _mm_sub_ps(first,second);
        _mm_store_ps(m[i],first);
    }
    return *this;
}

matrix4x4& matrix4x4::operator *= (const matrix4x4& mat)
{
    __m128 dest[4];
    for(int i = 0; i < 4;i++)
        dest[i] = _mm_load_ps(mat.m[i]);

    for(int i = 0; i < 4; i++)
    {
        __m128 src[4];
        src[0] = _mm_set_ps1(m[i][0]);
        src[1] = _mm_set_ps1(m[i][1]);
        src[2] = _mm_set_ps1(m[i][2]);
        src[3] = _mm_set_ps1(m[i][3]);

        src[0] = _mm_mul_ps(src[0],dest[0]);
        src[1] = _mm_mul_ps(src[1],dest[1]);
        src[2] = _mm_mul_ps(src[2],dest[2]);
        src[3] = _mm_mul_ps(src[3],dest[3]);

        src[0] = _mm_add_ps(src[0],src[1]);
        src[0] = _mm_add_ps(src[0],src[2]);
        src[0] = _mm_add_ps(src[0],src[3]);
        _mm_store_ps(m[i],src[0]);
    }
    return *this;
}

bool matrix4x4::Inverse()
{
    float fScale(0.f), fMax(0.f);
    int nCheckCount(0);			// 处理到第几列
    int nRow[4] = { 0, 1, 2, 3 };	// 存储新的行索引
    bool bRow[4] = { false };		// 行是否归一化（已为主导行）
    __m128 tempMat[4];// 缓存
    tempMat[0] = _mm_set_ps(0,0,0,1);
    tempMat[1] = _mm_set_ps(0,0,1,0);
    tempMat[2] = _mm_set_ps(0,1,0,0);
    tempMat[3] = _mm_set_ps(1,0,0,0);

    // 处理
    while (nCheckCount < 4)
    {
        fMax = 0.f;
        // 选取某列中非主导变换行的最大行
        for (int r=0; r<4; r++)
        {
            if (bRow[r])
                continue;
            if (fabsf(m[r][nCheckCount]) > fMax)
            {
                nRow[nCheckCount] = r;
                fMax = fabsf(m[r][nCheckCount]);
            }
        }
        // 防止由于数值过小而出现奇异
        if (fMax == 0)
            return false;

        fMax = 1.f / m[nRow[nCheckCount]][nCheckCount];
        // 逐行处理
        for (int r=0; r<4; r++)
        {
            if (r == nRow[nCheckCount])
            {
                // 归一化本行
                //if (fMax != 1.f)
                {
                    for (int c=nCheckCount + 1; c<4; c++)// 归一化右边
                    {
                        m[r][c] *= fMax;
                    }
                    __m128 rMax = _mm_set_ps1(fMax);
                    tempMat[r] = _mm_mul_ps(tempMat[r],rMax);
                }
                // 设置主导行及已归一标置
                bRow[r] = true;
            }
            else		// 将其他行的该列归 0
            {
                fScale = m[r][nCheckCount];
                if (fScale != 0.f)
                {
                    // 如果主导行未归一则系数要多除主导行的主导列值
                    if (!bRow[nRow[nCheckCount]])
                    {
                        fScale *= fMax;
                    }
                    for (int c=nCheckCount + 1; c<4; c++)// 合并右边
                    {
                        m[r][c] -= m[nRow[nCheckCount]][c] * fScale;
                    }
                    __m128 rMax = _mm_set_ps1(fScale);
                    rMax = _mm_mul_ps(rMax,tempMat[nRow[nCheckCount]]);
                    tempMat[r] = _mm_sub_ps(tempMat[r],rMax);
                }
            }
        }
        nCheckCount++;
    }
    // 交换已变换的矩阵
    for (int r=0; r<4; r++)
    {
        _mm_store_ps(m[r],tempMat[nRow[r]]);
    }
    return true;

    return true;
}

bool matrix4x4::IsIdentity()
{
    for(int i = 0 ; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            bool tmp = false;
            if(i == j)
                tmp = floatIsEqualNear(m[i][j],1);
            else
                tmp = floatIsZero(m[i][j]);
            if(!tmp)
                return false;
        }
    }
    return true;
}


bool matrix4x4::Inverse_Test()
{
    int is[4];
    int js[4];
    //_mm_prefetch(m,_MM_HINT_T0);
    //print();
    for (int k = 0; k < 4; k ++) 
    {// 第一步，全选主元 
        float fMax = 0; 
        for (int i = k; i < 4 ; i ++) 
        { 
            for (int j = k; j < 4; j ++) 
            {
                float f = fabs(m[i][j]); 
                if (f > fMax) 
                {
                    fMax = f; 
                    is[k] = i; 
                    js[k] = j; 
                }
            } 
        } 
        if (fMax == 0) 
            return false;
        if (is[k] != k) //行交换
        { 
            swapLine(k,is[k]);
        } 
        if (js[k] != k) //列交换
        { 
            for(int count=0;count< 4 ;count++)
            {
                swap(count,k,count,js[k]); 
            }
        } 
        //print();
        // 计算逆矩阵 
        // 第二步 归一化运算
        m[k][k] = 1.f / m[k][k];
        // 第三步 消元计算
        float fmkk = m[k][k];
        for (int i = 0; i < 4; i ++) 
        { 
            if(i!= k) 
            { 
                float fm_ik = m[i][k];
                m[i][k] = 0;
                for (int j = 0; j < 4; j ++) 
                { 
                    m[i][j] -= fm_ik * m[k][j] * fmkk;
                } 
            } 
        }
        for (int j = 0; j < 4; j ++) 
            m[k][j] = m[k][j] * fmkk;
    }
    for (int k = 3 ; k >= 0; k --)
    { 
        if (js[k] != k) 
        { 
            swapLine(k,js[k]);
       } 
        if (is[k] != k) 
        { 
            for(int l=0;l<4;l++)
            {
                swap(l,k,l,is[k]);
            }
        } 
    }
    return true;
}

void matrix4x4::ResetPos(const float3 &vPos)
{
    memcpy(m[3],vPos.GetData(),3 * sizeof(float));
}

void matrix4x4::ResetPos(float fX, float fY, float fZ)
{
    __m128 pos = _mm_set_ps(1,fZ,fY,fX);
    _mm_store_ps(m[3],pos);
}

void matrix4x4::swap(int i1,int j1,int i2,int j2)
{
    float fTemp = m[i1][j1];
    m[i1][j1] = m[i2][j2];
    m[i2][j2] = fTemp;
}

void matrix4x4::swapLine(int i1,int i2)
{
    __m128 line1 = _mm_load_ps(m[i1]);
    __m128 line2 = _mm_load_ps(m[i2]);
    _mm_store_ps(m[i1],line2);
    _mm_store_ps(m[i2],line1);
}
matrix4x4::matrix4x4(const matrix4x4& mat)
{
    memcpy(m,mat.m,sizeof(float)<<4);
}

matrix4x4 matrix4x4::toNormalMat()
{
    matrix4x4 matNormal;
    for(int i = 0; i < 3; i++)
    {
        __m128 first = _mm_load_ps(m[i]);
        _mm_store_ps(matNormal.m[i],first);
    }
    matNormal.Transpose();
    matNormal.Inverse();
    return matNormal;
}

//non-member operator overloading
matrix4x4 operator + (const matrix4x4& mat1,const matrix4x4& mat2)
{
    matrix4x4 res(mat1);
    res += mat2;
    return res;
}

matrix4x4 operator - (const matrix4x4& mat1,const matrix4x4& mat2)
{
    matrix4x4 res(mat1);
    res -= mat2;
    return res;
}

matrix4x4 operator * (const matrix4x4& mat1,const matrix4x4& mat2)
{
    matrix4x4 res(mat1);
    res *= mat2;
    return res;
}

