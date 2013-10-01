//email : hjc0720@gmail.com
//animation software

#ifndef SPACECONVERTTEST_H
#define SPACECONVERTTEST_H
#include "RDSpaceConvert.h"
#include "HMatrixQ4F.h"

class SpaceConvertTest :public ::testing::Test
{
public:
    SpaceConvertTest();
protected:
    RDSpaceParam param;
    matrix4x4 WorldMat;
    matrix4x4 ViewMat;
    matrix4x4 ProjMat;

};

#endif // SPACECONVERTTEST_H
