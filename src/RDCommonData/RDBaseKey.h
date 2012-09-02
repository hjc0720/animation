// =====================================================================================
// 
//       Filename:  RDBaseKey.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05/08/11 21:03:14
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDBASEKEY_INC
#define  RDBASEKEY_INC

class RDFileDataStream;
enum RDKeyType
{
    RDLineKey,
};
template <typename Value>
class RDKey
{
public:
    RDKey(RDKeyType nType = RDLineKey):m_nKeyType(nType){};
    RDKey(const Value& value,RDKeyType nType = RDLineKey):m_KeyValue(value),m_nKeyType(nType){};
    const Value& GetValue()const{return m_KeyValue;}
    void SetValue(const Value& value){m_KeyValue = value;}
    RDKeyType GetKeyType()const{return m_nKeyType;}
    void SetKeyType(RDKeyType nType){m_nKeyType = nType;}
	void Save(RDFileDataStream& buffer);
	void Load(RDFileDataStream& buffer);
protected:
    Value m_KeyValue;
    RDKeyType m_nKeyType;
};

#endif   // ----- #ifndef rdbasekey_INC  -----
