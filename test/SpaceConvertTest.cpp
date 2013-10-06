//email : hjc0720@gmail.com
//animation software

#include "gtest/gtest.h"
#include "SpaceConvertTest.h"
#include <vector>
using namespace std;


SpaceConvertTest::SpaceConvertTest()
{
    WorldMat.ResetPos(0,0,100);
    matrix4x4::CreateViewMat(ViewMat,float3(0,0,1080),float3(0,1,0),float3(0,0,0));
    matrix4x4::CreateProjectMat(ProjMat,-96,96,54,-54,108,10000);
}

TEST_F(SpaceConvertTest,T3D_2D)
{
    RDSpaceParam param(&WorldMat,&ViewMat,&ProjMat,QRect(0,0,1920,1080));

    vector<float3> vSrc;
    vector<float3> vDst;
    vSrc.push_back(float3(0,0,0));
    vDst.push_back(float3(960,540,0));
    vSrc.push_back(float3(-960,540,-100));
    vDst.push_back(float3(0,0,0));
    vSrc.push_back(float3(960,540,-100));
    vDst.push_back(float3(1920,0,0));
    vSrc.push_back(float3(960,-540,-100));
    vDst.push_back(float3(1920,1080,0));
    for(size_t i = 0; i < vSrc.size(); i++)
    {
        float3 v2D = param.Convert3DTo2D(vSrc[i]);
        //printf("%d\n",i);
        EXPECT_FLOAT_EQ(vDst[i].x(),v2D.x());
        EXPECT_FLOAT_EQ(vDst[i].y(),v2D.y());
    }
}

TEST_F(SpaceConvertTest,T2D_3D)
{
    RDSpaceParam param(&WorldMat,&ViewMat,&ProjMat,QRect(0,0,1920,1080));

    vector<float3> vSrc;
    vector<float3> vDst;
    vSrc.push_back(float3(0,0,0));
    vDst.push_back(float3(960,540,0));
    vSrc.push_back(float3(-96,54,-100));
    vDst.push_back(float3(0,0,0));
    vSrc.push_back(float3(96,54,-100));
    vDst.push_back(float3(1920,0,0));
    vSrc.push_back(float3(96,-54,-100));
    vDst.push_back(float3(1920,1080,0));
    vSrc.push_back(float3(-96,-54,-100));
    vDst.push_back(float3(0,1080,0));

    for(size_t i = 0; i < vSrc.size(); i++)
    {
        float3 v3D = param.Convert2DTo3D(vDst[i]);
        //printf("%d\n",i);
        EXPECT_NEAR(vSrc[i].x(),v3D.x(),0.0001);
        EXPECT_NEAR(vSrc[i].y(),v3D.y(),0.0001);
    }
}

TEST_F(SpaceConvertTest,Tri)
{
    RDSpaceParam param(&WorldMat,&ViewMat,&ProjMat,QRect(0,0,1920,1080));
    float3 vHitPt;
    bool bHit = param.HitTriangle(vHitPt,float3(960,540,0),float3(0,100,0),float3(100,-100,0),float3(-100,-100,0));
    EXPECT_TRUE(bHit);
    EXPECT_EQ(vHitPt.x(),0);
    EXPECT_EQ(vHitPt.y(),0);
}
