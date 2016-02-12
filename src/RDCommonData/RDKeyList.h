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
#include <set>

class RDBaseKeyList
{
public:
    RDBaseKeyList(){};
    virtual void Serialize(RDJsonDataStream &buffer, Json::Value &parent, bool bSave)=0;
};
// =====================================================================================

template <typename KeyType>
class RDKeyList : public RDBaseKeyList
{
public:
    void AddKey(const RDTime& nFrame,const KeyType& keyValue);
    void delKey(RDTime nTime);
    void moveKey(RDTime srcTime, RDTime dstTime);
    KeyType GetKeyValue(const RDTime& nSectionTime,const KeyType& vDefaultValue=KeyType());
	void GetTime(std::set<RDTime>& set)const;
    void Serialize(RDJsonDataStream &buffer, Json::Value &parent, bool bSave)override;
protected:
    bool IsKeyTime(const RDTime& nSectionTime); 
    KeyType Interpolation(const RDTime& nTime, const RDBaseKey<KeyType>& FirstKey,const RDTime& nFirstTime, const RDBaseKey<KeyType>& SecondKey,const RDTime& nSecondTime);
protected:
    std::map<RDTime,RDBaseKey<KeyType>*> m_KeyList;
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
#endif   // ----- #ifndef rdkeylist_INC  -----
