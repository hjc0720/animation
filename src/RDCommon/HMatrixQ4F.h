/*
 * =====================================================================================
 *
 *       Filename:  HMatrixQ4F.h
 *
 *    Description:  四维矩阵，采用sse加速
 *
 *        Version:  1.0
 *        Created:  2010年12月19日 19时24分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  黄建超 (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  HMATRIXQ4F_INC
#define  HMATRIXQ4F_INC

#ifdef __GTEST__
#include "gtest/gtest_prod.h"
#endif

class float3;
////////////////////////////////////////////////////////////////////////////////
//常量定义
enum HMatrixQ4F_TYPE
{
    HMatrixQ4F_POS,
    HMatrixQ4F_Rotate,
    HMatrixQ4F_Scale,
};

class HMatrixQ4F
{
public:
    static const HMatrixQ4F& CreateProjectMat(HMatrixQ4F& mat,float l,float r,float t,float b,float zn,float zf);
    static const HMatrixQ4F& CreateViewMat(HMatrixQ4F& mat,const float3& vEyePos,const float3& vUp,const float3& vLookAt);
public:
    HMatrixQ4F();//生成单位阵
    HMatrixQ4F(const HMatrixQ4F& mat);
    HMatrixQ4F(float fX,float fY,float fZ,HMatrixQ4F_TYPE nType);

    void Transpose();
    bool Inverse();
    bool IsIdentity();
    bool Inverse_Test();
    const float* GetLine(int i)const{return m[i];}
    const float* data()const{return m[0];}

public:
    //运算符
    HMatrixQ4F& operator += (const HMatrixQ4F& m);
    HMatrixQ4F& operator -= (const HMatrixQ4F& m);
    HMatrixQ4F& operator *= (const HMatrixQ4F& m);// m could be the same of this
protected:
    void Identity();
    //欧拉角旋转,y,x,z
    void RotateMat(float fX,float fY,float fZ);
    void PosMat(float fX,float fY,float fZ);
    void ScaleMat(float fX,float fY,float fZ);
    void swap(int i1,int j1,int i2,int j2);
    void swapLine(int i1,int i2);
    //void print();

protected:
    float m[4][4] __attribute__ ((aligned (16)));


#ifdef __GTEST__
    FRIEND_TEST(HMatrixQ4F_Test, Constructor);
    FRIEND_TEST(HMatrixQ4F_Test, Rotate);
    FRIEND_TEST(HMatrixQ4F_Test, Pos);
    FRIEND_TEST(HMatrixQ4F_Test, Scale);
    FRIEND_TEST(HMatrixQ4F_Test, Transpose);
    FRIEND_TEST(HMatrixQ4F_Test, Add);
    FRIEND_TEST(HMatrixQ4F_Test, Sub);
    FRIEND_TEST(HMatrixQ4F_Test, Mul);

    FRIEND_TEST(HMatrixQ4F_Test, ConstructorTime);
    FRIEND_TEST(HMatrixQ4F_Test, RotateTime);
    FRIEND_TEST(HMatrixQ4F_Test, PosTime);
    FRIEND_TEST(HMatrixQ4F_Test, ScaleTime);
    FRIEND_TEST(HMatrixQ4F_Test, TransposeTime);
#endif
};
//运算操作符

HMatrixQ4F operator + (const HMatrixQ4F& mat1,const HMatrixQ4F& mat2);
HMatrixQ4F operator - (const HMatrixQ4F& mat1,const HMatrixQ4F& mat2);
HMatrixQ4F operator * (const HMatrixQ4F& mat1,const HMatrixQ4F& mat2);



#endif   /* ----- #ifndef hmatrixq4f_INC  ----- */
