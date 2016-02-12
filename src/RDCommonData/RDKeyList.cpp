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
void RDKeyList<KeyType>::delKey(RDTime nTime)
{
    m_KeyList.erase(nTime);
}

template <typename KeyType>
void RDKeyList<KeyType>::moveKey(RDTime srcTime, RDTime dstTime)
{
    auto it = m_KeyList.find(srcTime);
    if(it != m_KeyList.end())
    {
        RDBaseKey<KeyType>* value = it->second;
        m_KeyList.erase(it);
        m_KeyList.insert({dstTime,value});
    }
}

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
		auto low = m_KeyList.lower_bound(nSectionTime);
		if(low == m_KeyList.end())
			return m_KeyList.rbegin()->second->GetValue();

		if(low == m_KeyList.begin() || low->first == nSectionTime)
			return low->second->GetValue();

		auto before = low--;
		return Interpolation(nSectionTime,*(before->second),before->first,*(low->second),low->first);
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
    for(auto& key : m_KeyList)
        times.insert(key.first);
}

template <typename KeyType>
void RDKeyList<KeyType>::Serialize(RDJsonDataStream &buffer, Json::Value &parent, bool bSave)
{
    buffer.Serialize(parent,"keylist",bSave,m_KeyList,[&buffer](Json::Value& child,RDTime& id,RDBaseKey<KeyType>*& key,bool bSave){
        buffer.Serialize(child,"key",bSave,id);
        RDKeyType keyType;
        if(bSave)
            keyType = key->GetKeyType();
        buffer.Serialize(child["value"],"type",bSave,keyType);

        if(!bSave)
            key = RDBaseKey<KeyType>::CreateKey(keyType);
        key->Serialize(buffer,child["value"],bSave);
    });
}

template<>
class JsonHelper<RDKeyType>
{
public:
    Json::Value toJson(const RDKeyType& value) { return Json::Value(value); }
    void fromJson(RDKeyType& v,const Json::Value& json){ v = static_cast<RDKeyType>(json.asInt()); }
};
// =====================================================================================
template class RDKeyList<float3>;
template class RDKeyList<float>;
