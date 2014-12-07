// =====================================================================================
// 
//       Filename:  RDKeyList.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05/08/11 20:13:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDKEYLIST_INC
#define  RDKEYLIST_INC
#include "mac_define.h"
#include <map>
#include "RDBaseKey.h"
#include <cstdint>
#include "RDFileDataStream.h"
#include <QDebug>

// =====================================================================================
template <typename KeyType> class RDKeyList ;
template <typename KeyType>
RDFileDataStream& operator << (RDFileDataStream& buffer,const RDKeyList<KeyType>& KeyList);
template <typename KeyType>
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDKeyList<KeyType>& KeyList);

// =====================================================================================
class RDBaseKeyList
{
public:
    RDBaseKeyList(){};
};
// =====================================================================================

template <typename KeyType>
class RDKeyList : public RDBaseKeyList
{
public:
    void AddKey(const RDTime& nFrame,const KeyType& keyValue);
    KeyType GetKeyValue(const RDTime& nSectionTime,const KeyType& vDefaultValue=KeyType());
protected:
    bool IsKeyTime(const RDTime& nSectionTime); 
    KeyType Interpolation(const RDTime& nTime, const RDBaseKey<KeyType>& FirstKey,const RDTime& nFirstTime, const RDBaseKey<KeyType>& SecondKey,const RDTime& nSecondTime);
protected:
    std::map<RDTime,RDBaseKey<KeyType>*> m_KeyList;
friend RDFileDataStream& operator << <KeyType>(RDFileDataStream& buffer,const RDKeyList<KeyType>& proj);
friend RDFileDataStream& operator >> <KeyType>(RDFileDataStream& buffer,RDKeyList<KeyType>& proj);
};

// =====================================================================================
template <typename KeyType>
inline void RDKeyList<KeyType>::AddKey(const RDTime& nSectionTime,const KeyType& keyValue)
{
    //qDebug() << "add Key time" << nSectionTime;
    if(m_KeyList[nSectionTime] == 0)
    {
        qDebug() << "add new Key";
        RDBaseKey<KeyType>* pNewKey = RDBaseKey<KeyType>::CreateKey(keyValue);
        m_KeyList[nSectionTime] = pNewKey;
    }
    else
    {
        m_KeyList[nSectionTime]->SetValue(keyValue);
        //qDebug() << "modify Key";
    }
}

// =====================================================================================
template <typename KeyType>
RDFileDataStream& operator << (RDFileDataStream& buffer,const RDKeyList<KeyType>& KeyList)
{
    quint64 nCount = (quint64)KeyList.m_KeyList.size();
    buffer << nCount;
    for(auto it = KeyList.m_KeyList.begin(); it != KeyList.m_KeyList.end(); it++)
    {
        auto pKey = it->second;
        buffer << it->first;
		buffer << (int)(pKey->GetKeyType());
        pKey->Serialize(buffer,true);
    }
    return buffer;
}

template <typename KeyType>
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDKeyList<KeyType>& KeyList)
{
    quint64 nCount = 0;
    buffer >> nCount;
    for(quint64 i = 0 ; i < nCount; i++)
    {
        RDTime nFrame;
        buffer >> nFrame;
		int nType;
		buffer >> nType;
        RDBaseKey<KeyType>* pKey = RDBaseKey<KeyType>::CreateKey((RDKeyType)nType);
		pKey->Serialize(buffer,false);
        KeyList.m_KeyList[nFrame] = pKey;
    }
    return buffer;
}
#endif   // ----- #ifndef rdkeylist_INC  -----
