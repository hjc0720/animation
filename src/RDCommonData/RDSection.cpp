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

void RDSection::AddScaleKey(const RDTime& nStoryTime,const float3& value)
{
    RDTime nSectionTime = nStoryTime - m_nStartTime;
    m_ScaleKey.AddKey(nSectionTime,value);
    m_nLength = max(m_nLength,nSectionTime);
}

void RDSection::delPosKey(const RDTime &nSectionTime)
{
    delKey(nSectionTime,RDSectionPos);
}

void RDSection::delAngleKey(const RDTime &nSectionTime)
{
    m_RotateKey.delKey(nSectionTime);
    delKey(nSectionTime,RDSectionAngle);
}

void RDSection::delScaleKey(const RDTime &nSectionTime)
{
    delKey(nSectionTime,RDSectionScale);
}

void RDSection::delKey(RDTime nSectionTime)
{
    for(int eType = RDSectionPos; eType != RDSectionCount; ++eType)
    {
        delKey(nSectionTime,static_cast<RDSectionKeyType>(eType));
    }
}

void RDSection::moveKey(RDTime nSrcTime, RDTime nDstTime)
{
    for(int eType = RDSectionPos; eType != RDSectionCount; ++eType)
    {
        auto& keylist = getKeyList(static_cast<RDSectionKeyType>(eType));
        keylist.moveKey(nSrcTime,nDstTime);
    }
}

void RDSection::AddPosKey(const RDTime& nStoryTime,const float3& value)
{
    RDTime nSectionTime = nStoryTime - m_nStartTime;
    m_PosKey.AddKey(nSectionTime,value);
    m_nLength = max(m_nLength,nSectionTime);
}

float3 RDSection::GetPosVector(RDTime nSectionTime)
{
    return m_PosKey.GetKeyValue(nSectionTime);
}

float3 RDSection::GetAngleVector(RDTime nSectionTime)
{
    return m_RotateKey.GetKeyValue(nSectionTime);
}

float3 RDSection::GetScaleVector(RDTime nSectionTime)
{
    return m_ScaleKey.GetKeyValue(nSectionTime,float3::GetOne());
}

std::set<RDTime> RDSection::getKeyTimeSet() const
{
    std::set<RDTime>  keyTimeSet;
    for(int i = RDSectionPos; i < RDSectionCount;i++)
    {
        const RDKeyList<float3>& list = getKeyList(static_cast<RDSectionKeyType>(i));
        list.GetTime(keyTimeSet);
    }
    return keyTimeSet;
}

RDKeyList<float3>& RDSection::getKeyList(RDSectionKeyType eType)
{
    switch(eType)
    {
    case RDSectionPos:
        return m_PosKey;
    case RDSectionAngle:
        return m_RotateKey;
    case RDSectionScale:
        return m_ScaleKey;
    default:
        throw;
    }
}

const RDKeyList<float3>& RDSection::getKeyList(RDSectionKeyType eType)const
{
    switch(eType)
    {
    case RDSectionPos:
        return m_PosKey;
    case RDSectionAngle:
        return m_RotateKey;
    case RDSectionScale:
        return m_ScaleKey;
    default:
        throw;
    }
}

void RDSection::delKey(RDTime nSectionTime, RDSectionKeyType type)
{
    auto& keylist = getKeyList(type);
    keylist.delKey(nSectionTime);
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
