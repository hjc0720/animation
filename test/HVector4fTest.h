/*
 * =====================================================================================
 *
 *       Filename:  HVector4fTest.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年03月05日 15时19分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang Jianchao (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __HVECTOR4FTEST_H
#define __HVECTOR4FTEST_H
class HVector4fTest : public ::testing::Test 
{
public:
   HVector4fTest();
protected:
   float m_fX;
   float m_fY;
   float m_fZ;
   float m_fW;
};
#endif
