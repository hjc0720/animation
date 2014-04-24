// =====================================================================================
// 
//       Filename:  RDSection.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  31/07/11 22:12:13
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDSection.h"
#include "RDFileDataStream.h"
#include "RDKeyList.h"
#include <algorithm>
using namespace std;

RDSection::RDSection()
{
    m_nStartTime = 0;
    m_nLength = 0;
   m_nType = RDSectionKeep;
}
RDSection::RDSection(const RDTime& nStartFrame,const RDTime& nLength)
{
    m_nType = RDSectionKeep;
    m_nStartTime = nStartFrame;
    m_nLength = nLength;
}

void RDSection::AddAngleKey(const RDTime& nStoryTime,const float3& value)
{
    RDTime nSectionTime = nStoryTime - m_nStartTime;
    m_RotateKey.AddKey(nSectionTime,value);
    m_nLength = max(m_nLength,nSectionTime);
}

void RDSection::AddPosKey(const RDTime& nStoryTime,const float3& value)
{
    RDTime nSectionTime = nStoryTime - m_nStartTime;
    m_PosKey.AddKey(nSectionTime,value);
    m_nLength = max(m_nLength,nSectionTime);
}

float3 RDSection::GetPosVector(const RDTime& nSectionTime)
{
    return m_PosKey.GetKeyValue(nSectionTime);
}

float3 RDSection::GetAngleVector(const RDTime& nSectionTime)
{
    return m_RotateKey.GetKeyValue(nSectionTime);
}

// =====================================================================================
RDFileDataStream& operator << (RDFileDataStream& buffer,const RDSection& Section)
{
    buffer << Section.m_PosKey;
    buffer << Section.m_RotateKey;
    buffer << Section.m_ScaleKey;
    buffer << Section.m_nStartTime;
    buffer << Section.m_nLength;
    buffer << Section.m_nType;
    return buffer;
}
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDSection& Section)
{
    buffer >> Section.m_PosKey;
    buffer >> Section.m_RotateKey;
    buffer >> Section.m_ScaleKey;
    buffer >> Section.m_nStartTime;
    buffer >> Section.m_nLength;
    int nType;
    buffer >> nType;
    Section.m_nType = (RDSectionOutType)nType;
    return buffer;
}
