// =====================================================================================
// 
//       Filename:  RDKeyList.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05/08/11 20:13:49
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDKeyList.h"
#include "RDBaseKey.h"
#include "HVector3f.h"
#include "HVector4f.h"
#include <QDebug>

template <typename KeyType>
KeyType RDKeyList<KeyType>::GetKeyValue(const RDTime& nSectionTime)
{
    //qDebug() << "key list count:"<< m_KeyList.size();
    if(m_KeyList.size() == 0)
        return KeyType();
    else if(m_KeyList.size() == 1)
    {
        auto it = m_KeyList.begin();
        return it->second->GetValue();
    }
    else 
    {
        if(IsKeyTime(nSectionTime) )
            return m_KeyList[nSectionTime]->GetValue();
        auto first = m_KeyList.rbegin();
        for(; first != m_KeyList.rend(); first++)
        {
            if(first->first < nSectionTime)
                break;
        }
        if(first == m_KeyList.rend())
        {
            auto it = m_KeyList.begin();
            return it->second->GetValue();
        }
        auto second = m_KeyList.begin();
        for(; second != m_KeyList.end(); second++)
        {
            if(second->first > nSectionTime)
                break;
        }
        if(second == m_KeyList.end())
        {
            auto it = m_KeyList.rbegin();
            return it->second->GetValue();
        }
        return Interpolation(nSectionTime,*(first->second),first->first,*(second->second),second->first);
    }
    return KeyType();
}

template <typename KeyType>
inline bool RDKeyList<KeyType>::IsKeyTime(const RDTime& nSectionTime)
{
    auto it = m_KeyList.find(nSectionTime);
    return it != m_KeyList.end() && it->second != 0;
} 
template <typename KeyType>
KeyType RDKeyList<KeyType>::Interpolation(const RDTime& nTime, const RDKey<KeyType>& FirstKey,const RDTime& nFirstTime, const RDKey<KeyType>& SecondKey,const RDTime& nSecondTime)
{
    RDKeyType nType = FirstKey.GetKeyType();
    if(nType == RDLineKey)
    {
        float weight = (nTime - nFirstTime) / (float)(nSecondTime - nFirstTime);
        return (1 - weight) * FirstKey.GetValue() + weight * SecondKey.GetValue();
    }
    return KeyType();
}
// =====================================================================================
template class RDKeyList<float3>;
template class RDKeyList<float>;
