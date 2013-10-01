/*
 * =====================================================================================
 *
 *       Filename:  HMatrixTest.cpp
 *
 *    Description:  测试HMatrix类
 *
 *        Version:  1.0
 *        Created:  2010年12月23日 21时42分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  黄建超 (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "gtest/gtest.h"
#include "HMatrixTest.h"
#include "HMath.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "HMatrixQ4F.h"

#define TEST_COUNT 10000000

TEST_F(HMatrixQ4F_Test,ConstructorTime)
{
    for(int i = 0; i < TEST_COUNT; i++)
    {
        matrix4x4 testMat;
 }
}
TEST_F(HMatrixQ4F_Test,Constructor)
{
    for(int i = 0; i < 1; i++)
    {
        matrix4x4 testMat;
       for(int j = 0; j < 4;j++)
            EXPECT_FLOAT_EQ(1,testMat.m[j][j]);
   }
}

TEST_F(HMatrixQ4F_Test,RotateTime)
{
        matrix4x4 testMat;
    for(int i = 0; i < TEST_COUNT; i++)
    {
        testMat.RotateMat(0,0,0);
    }
}

#define PAI 3.1415926

TEST_F(HMatrixQ4F_Test,Rotate)
{
    matrix4x4 testMat;
    matrix4x4 resMat;
    for(int i = 0; i < 36; i++)
    {
        float fx = i / PAI/ 180;
        for(int j = 0; j < 36; j++)
        {
            float fy = j / PAI/ 180;
            for(int k = 0; k < 36;k++)
            {
                float fz = k / PAI/ 180;
                resMat.RotateMat(fx,fy,fz);
                testMat.m[0][0] = cosf(fz) * cosf(fy) + sinf(fy) * sinf(fx) * sinf(fz);
                testMat.m[0][1] = -sinf(fz) * cosf(fy) + sinf(fy) * sinf(fx) * cosf(fz);
                testMat.m[0][2] = cosf(fx) * sinf(fy) ;
                testMat.m[1][0] = sinf(fz) * cosf(fx);
                testMat.m[1][1] = cosf(fz) * cosf(fx) ;
                testMat.m[1][2] = -sinf(fx) ;
                testMat.m[2][0] = -cosf(fz) * sinf(fy) + cosf(fy) * sinf(fx) * sinf(fz);
                testMat.m[2][1] = sinf(fy) * sinf(fz) + cosf(fy) * sinf(fx) * cosf(fz);
                testMat.m[2][2] = cosf(fx) * cosf(fy);
                for(int n = 0; n < 4;n++)
                    for(int m = 0; m < 4;m++)
                    {
                        ASSERT_NEAR(resMat.m[n][m],testMat.m[n][m],FLOAT0);
                        //if(resMat.m[n][m]!=testMat.m[n][m])
                        //{
                        //    ADD_FAILURE() << n << "\t"<<m<<"\t"<<resMat.m[n][m] << "\t" <<testMat.m[n][m];
                        //}
                    }

            }
        }
  }
}

TEST_F(HMatrixQ4F_Test,PosTime)
{
    matrix4x4 testMat;
    for(int i = 0; i < TEST_COUNT; i++)
    {
        testMat.PosMat(i,i / 2,i / 3);
    }
}

TEST_F(HMatrixQ4F_Test,Pos)
{
    matrix4x4 testMat;
    for(int i = 0; i < 100; i++)
    {
        testMat.PosMat(i,i / 2.f,i / 3.f);
        ASSERT_NEAR(testMat.m[3][0],i,FLOAT0);
        ASSERT_NEAR(testMat.m[3][1],i / 2.f,FLOAT0);
        ASSERT_NEAR(testMat.m[3][2],i / 3.f,FLOAT0);
    }
}

TEST_F(HMatrixQ4F_Test,ScaleTime)
{
    matrix4x4 testMat;
    for(int i = 0; i < TEST_COUNT; i++)
    {
        testMat.ScaleMat(i,i / 2,i / 3);
    }
}

TEST_F(HMatrixQ4F_Test,Scale)
{
    matrix4x4 testMat;
    for(int i = 0; i < 100; i++)
    {
        testMat.ScaleMat(i,i / 2.f,i / 3.f);
        ASSERT_NEAR(testMat.m[0][0],i,FLOAT0);
        ASSERT_NEAR(testMat.m[1][1],i / 2.f,FLOAT0);
        ASSERT_NEAR(testMat.m[2][2],i / 3.f,FLOAT0);
    }
}

TEST_F(HMatrixQ4F_Test,TransposeTime)
{
    matrix4x4 testMat;
    for(int i = 0; i < TEST_COUNT; i++)
    {
        testMat.Transpose();
    }
}

TEST_F(HMatrixQ4F_Test,Transpose)
{
    matrix4x4 testMat;
    matrix4x4 srcMat;
    srand(time(NULL));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            testMat.m[i][j] = rand();
            srcMat.m[i][j] = testMat.m[i][j];
        }
    }
    testMat.Transpose();
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            EXPECT_FLOAT_EQ(testMat.m[i][j],srcMat.m[j][i]);
        }
    }
}

TEST_F(HMatrixQ4F_Test,AddTime)
{
    matrix4x4 testMat;
    matrix4x4 srcMat;
    for(int i = 0; i < TEST_COUNT; i++)
        testMat += srcMat;
}

TEST_F(HMatrixQ4F_Test,Add)
{
    matrix4x4 testMat;
    matrix4x4 srcMat;
    matrix4x4 destMat;
    srand(time(NULL));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            destMat.m[i][j] = rand();
            srcMat.m[i][j] = rand();
            testMat.m[i][j] = srcMat.m[i][j] ;
        }
    }
    srcMat += destMat;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            EXPECT_FLOAT_EQ(testMat.m[i][j] + destMat.m[i][j],srcMat.m[i][j]);
        }
    }
}

TEST_F(HMatrixQ4F_Test,SubTime)
{
    matrix4x4 testMat;
    matrix4x4 srcMat;
    for(int i = 0; i < TEST_COUNT; i++)
        testMat -= srcMat;
}

TEST_F(HMatrixQ4F_Test,Sub)
{
    matrix4x4 testMat;
    matrix4x4 srcMat;
    matrix4x4 destMat;
    srand(time(NULL));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            destMat.m[i][j] = rand();
            srcMat.m[i][j] = rand();
            testMat.m[i][j] = srcMat.m[i][j] ;
        }
    }
    srcMat -= destMat;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            EXPECT_FLOAT_EQ(testMat.m[i][j] - destMat.m[i][j],srcMat.m[i][j]);
        }
    }
}
TEST_F(HMatrixQ4F_Test,MulTime)
{
    matrix4x4 testMat;
    matrix4x4 srcMat;
    for(int i = 0; i < TEST_COUNT; i++)
        testMat *= srcMat;
}

TEST_F(HMatrixQ4F_Test,Mul)
{
    matrix4x4 testMat;
    matrix4x4 srcMat;
    matrix4x4 destMat;
    srand(time(NULL));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            destMat.m[i][j] = rand();
            srcMat.m[i][j] = rand();
            testMat.m[i][j] = srcMat.m[i][j] ;
        }
    }
    srcMat *= destMat;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            float result = testMat.m[i][0] * destMat.m[0][j] + testMat.m[i][1] * destMat.m[1][j] + testMat.m[i][2] * destMat.m[2][j] + testMat.m[i][3] * destMat.m[3][j] ;

            EXPECT_FLOAT_EQ(result,srcMat.m[i][j]);
        }
    }
}

TEST_F(HMatrixQ4F_Test,IsIdentityTime)
{
    matrix4x4 testMat;
    bool test = false;
    for(int i = 0; i < TEST_COUNT; i++)
        test = testMat.IsIdentity();
}

TEST_F(HMatrixQ4F_Test,IsIdentity)
{
    matrix4x4 testMat;
    matrix4x4 srcMat(0.30,0.29,0.000040,HMatrixQ4F_POS);
    matrix4x4 destMat(0.30,0.29,0.000040,HMatrixQ4F_Rotate);
    matrix4x4 scaleMat(0.30,0.29,0.000040,HMatrixQ4F_Scale);
    matrix4x4 lastMat ;
    lastMat *= srcMat;
    lastMat *= destMat;
    lastMat *= scaleMat;
    EXPECT_TRUE(testMat.IsIdentity());
    EXPECT_FALSE(srcMat.IsIdentity());
    EXPECT_FALSE(destMat.IsIdentity());
    EXPECT_FALSE(scaleMat.IsIdentity());
    EXPECT_FALSE(lastMat.IsIdentity());
}

TEST_F(HMatrixQ4F_Test,InverseTime)
{
    matrix4x4 testMat(0.30,0.29,0.000040,HMatrixQ4F_Rotate);
    for(int i = 0; i < TEST_COUNT; i++)
        testMat.Inverse();
}

TEST_F(HMatrixQ4F_Test,Inverse)
{
    srand(time(NULL));
    float fX,fY,fZ;
    fX = rand();
    fY = rand();
    fZ = rand();
 
    matrix4x4 testMat(fX,fY,fZ,HMatrixQ4F_Rotate);
    matrix4x4 testMat1(fX,fY,fZ,HMatrixQ4F_Rotate);
    matrix4x4 srcMat(fX,fY,fZ,HMatrixQ4F_Rotate);
    testMat.Inverse();
    srcMat *= testMat ;
    EXPECT_TRUE(srcMat.IsIdentity());
}

//TEST_F(HMatrixQ4F_Test,InverseTime1)
//{
    //HMatrixQ4F testMat(0.30,0.29,0.000040,HMatrixQ4F_Rotate);
    //for(int i = 0; i < TEST_COUNT; i++)
        //testMat.Inverse_Test(false);
//}

//TEST_F(HMatrixQ4F_Test,Inverse1)
//{
    //srand(time(NULL));
    //float fX,fY,fZ;
    //fX = rand();
    //fY = rand();
    //fZ = rand();
 
    //HMatrixQ4F testMat(fX,fY,fZ,HMatrixQ4F_Rotate);
    //HMatrixQ4F srcMat(fX,fY,fZ,HMatrixQ4F_Rotate);
    //testMat.Inverse_Test(false);
    //srcMat *= testMat ;
    //EXPECT_TRUE(srcMat.IsIdentity());
//}
