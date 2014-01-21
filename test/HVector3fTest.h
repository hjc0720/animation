/*
 * =====================================================================================
 *
 *       Filename:  HVector3fTest.h
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

#ifndef __HVECTOR3FTEST_H
#define __HVECTOR3FTEST_H
class HVector3fTest : public ::testing::Test 
{
public:
   HVector3fTest();
protected:
   float m_fX;
   float m_fY;
   float m_fZ;
};
#endif

