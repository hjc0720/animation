/*
 * =====================================================================================
 *
 *       Filename:  HVector4fTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年03月05日 15时20分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang Jianchao (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include "gtest/gtest.h"
#include "HVector4fTest.h"
#include "HMath.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "HVector4f.h"

#define TEST_COUNT 10000000

HVector4fTest::HVector4fTest()
{
    srand(time(NULL));
    m_fX = rand();
    m_fY = rand();
    m_fZ = rand();
    m_fW = rand();

}

TEST_F(HVector4fTest,ConstructorTime)
{
    for(int i = 0; i < TEST_COUNT; i++)
    {
        float4 vec;
    }
}

TEST_F(HVector4fTest,SetTime)
{
    for(int i = 0; i < TEST_COUNT; i++)
    {
        float4 vec(i,i,i,i);
    }
}

#define PAI 3.1415926

TEST_F(HVector4fTest,Set)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    float fW = rand();
    
    float4 vec(fX,fY,fZ,fW);
    EXPECT_FLOAT_EQ(fX,vec.x());
    EXPECT_FLOAT_EQ(fY,vec.y());
    EXPECT_FLOAT_EQ(fZ,vec.z());
    EXPECT_FLOAT_EQ(fW,vec.w());
}

TEST_F(HVector4fTest,ModeTime)
{
    float4 vec;
    for(int i = 0; i < TEST_COUNT; i++)
    {
        vec.Mode();
    }
}

TEST_F(HVector4fTest,Mode)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    float fW = rand();
 
    float4 vec(fX,fY,fZ,fW);
    float fResult = fX * fX + fY * fY + fZ * fZ + fW * fW;
    EXPECT_FLOAT_EQ(fResult,vec.Mode() * vec.Mode());
}

TEST_F(HVector4fTest,NormalizeTime)
{
    float4 vec(m_fX,m_fY,m_fZ,m_fW);
    for(int i = 0; i < TEST_COUNT; i++)
    {
        vec.Normalize();
    }
}

TEST_F(HVector4fTest,Normalize)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    float fW = rand();

    float4 vec(fX,fY,fZ,fW);

    vec.Normalize();
    EXPECT_NEAR(1,vec.Mode(),FLOAT0);
}

TEST_F(HVector4fTest,AddTime)
{
    float4 vec1(m_fX,m_fY,m_fZ,m_fW);
    float4 vec2(m_fX,m_fY,m_fZ,m_fW);
   for(int i = 0; i < TEST_COUNT; i++)
    {
        vec1 += vec2;
    }
}

TEST_F(HVector4fTest,Add)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    float fW = rand();

    float4 vec1(fX,fY,fZ,fW);
    float4 temp(vec1);
    fX = rand();
    fY = rand();
    fZ = rand();
    fW = rand();

    float4 vec2(fX,fY,fZ,fW);
    vec1 += vec2;
    EXPECT_FLOAT_EQ(vec1.x(),vec2.x() + temp.x());
    EXPECT_FLOAT_EQ(vec1.y(),vec2.y() + temp.y());
    EXPECT_FLOAT_EQ(vec1.z(),vec2.z() + temp.z());
}

TEST_F(HVector4fTest,DividWTime)
{
    float4 vec1(m_fX,m_fY,m_fZ,m_fW);
   for(int i = 0; i < TEST_COUNT; i++)
    {
        vec1.DividW();
    }
}

TEST_F(HVector4fTest,DividW)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    float fW = rand();

    float4 vec1(fX,fY,fZ,fW);
    float4 temp(vec1);

    vec1.DividW();

    EXPECT_FLOAT_EQ(vec1.x(),temp.x() / temp.w());
    EXPECT_FLOAT_EQ(vec1.y(),temp.y() / temp.w());
    EXPECT_FLOAT_EQ(vec1.z(),temp.z() / temp.w());
    EXPECT_FLOAT_EQ(vec1.w(),temp.w() / temp.w());
}

TEST_F(HVector4fTest,colorConvert)
{
    unsigned int color = 0xffff33ff;
    float4 vColor(color);

    EXPECT_FLOAT_EQ(vColor.x(),1);
    EXPECT_FLOAT_EQ(vColor.y(),0.2);
    EXPECT_FLOAT_EQ(vColor.z(),1);
    EXPECT_FLOAT_EQ(vColor.w(),1);
}
