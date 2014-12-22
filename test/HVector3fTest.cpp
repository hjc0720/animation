/*
 * =====================================================================================
 *
 *       Filename:  HVector3fTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年01月21日 22时29分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang JianChao (hjc), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "gtest/gtest.h"
#include "HVector3fTest.h"
#include "HVector3f.h"
#include "HVector4f.h"

#define TEST_COUNT 10000000

HVector3fTest::HVector3fTest()
{
    srand(time(NULL));
    m_fX = rand();
    m_fY = rand();
    m_fZ = rand();
}

TEST_F(HVector3fTest,ConstructorTime)
{
    for(int i = 0; i < TEST_COUNT; i++)
    {
        float3 vec;
    }
}

TEST_F(HVector3fTest,SetTime)
{
    for(int i = 0; i < TEST_COUNT; i++)
    {
        float3 vec(i,i,i);
    }
}

TEST_F(HVector3fTest,Set)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    
    float3 vec(fX,fY,fZ);
    EXPECT_FLOAT_EQ(fX,vec.x());
    EXPECT_FLOAT_EQ(fY,vec.y());
    EXPECT_FLOAT_EQ(fZ,vec.z());
}

TEST_F(HVector3fTest,Copy)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    
    float3 tmp(fX,fY,fZ);
    float3 vec(tmp);
    EXPECT_FLOAT_EQ(fX,vec.x());
    EXPECT_FLOAT_EQ(fY,vec.y());
    EXPECT_FLOAT_EQ(fZ,vec.z());
}

TEST_F(HVector3fTest,add)
{
    float fX = rand();
    float fY = rand();
    float fZ = rand();
    float3 vec1(fX,fY,fZ);

    float fX1= rand();
    float fY1= rand();
    float fZ1= rand();
    float3 vec2(fX1,fY1,fZ1);
    vec1 += vec2;
    EXPECT_FLOAT_EQ(fX + fX1,vec1.x());
    EXPECT_FLOAT_EQ(fY + fY1,vec1.y());
    EXPECT_FLOAT_EQ(fZ + fZ1,vec1.z());
}

TEST_F(HVector3fTest,float2to)
{
	float2 v={(float)rand(),(float)rand()};
	float3 vTest(v);
    EXPECT_FLOAT_EQ(v.x,vTest.x());
    EXPECT_FLOAT_EQ(v.y,vTest.y());
    EXPECT_FLOAT_EQ(0,vTest.z());
}
