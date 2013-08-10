// =====================================================================================
// 
//       Filename:  RDBaseKey.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05/08/11 21:03:09
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// 
// =====================================================================================
#include "HVector3f.h"
#include "HVector4f.h"
#include "RDBaseKey.h"
#include "RDFileDataStream.h"

template <typename Value>
void RDKey<Value>::Save(RDFileDataStream& buffer)
{
    buffer << (int)m_nKeyType;
    buffer << m_KeyValue;
}
template <typename Value>
void RDKey<Value>::Load(RDFileDataStream& buffer)
{
    int nType;
    buffer >> nType;
    buffer >> m_KeyValue;
    switch(nType)
    {
    case RDLineKey:
        m_nKeyType = RDLineKey;
        break;
    default:
        m_nKeyType = RDLineKey;
    }
}

// =====================================================================================
template class RDKey<float3>;

