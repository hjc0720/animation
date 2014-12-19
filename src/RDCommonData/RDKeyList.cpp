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
KeyType RDKeyList<KeyType>::GetKeyValue(const RDTime& nSectionTime,const KeyType& vDefaultValue)
{
    //qDebug() << "key list count:"<< m_KeyList.size();
    if(m_KeyList.size() == 0)
        return vDefaultValue;
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
    return vDefaultValue;
}

template <typename KeyType>
inline bool RDKeyList<KeyType>::IsKeyTime(const RDTime& nSectionTime)
{
    auto it = m_KeyList.find(nSectionTime);
    return it != m_KeyList.end() && it->second != 0;
} 

template <typename KeyType>
KeyType RDKeyList<KeyType>::Interpolation(const RDTime& nTime, const RDBaseKey<KeyType>& FirstKey,const RDTime& nFirstTime, const RDBaseKey<KeyType>& SecondKey,const RDTime& nSecondTime)
{
	double dWeight = (nTime - nFirstTime) / (double)(nSecondTime - nFirstTime);
	return FirstKey.Interpolation(dWeight,SecondKey);
}

template <typename KeyType>
void RDKeyList<KeyType>::GetTime(std::set<RDTime>& times)const
{
	std::for_each(m_KeyList.begin(),m_KeyList.end(),[&](std::pair<const RDTime,RDBaseKey<KeyType>*> it){times.insert(it.first);});
}
// =====================================================================================
template class RDKeyList<float3>;
template class RDKeyList<float>;
