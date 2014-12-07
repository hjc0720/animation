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
class RDBaseKey
{
public:
	static RDBaseKey<Value>* CreateKey(const Value& value, RDKeyType type = RDLineKey);
	static RDBaseKey<Value>* CreateKey(RDKeyType type = RDLineKey);
public:
    RDBaseKey(const Value& value):m_KeyValue(value){};
    RDBaseKey() = default;
	virtual ~RDBaseKey(){};
	virtual RDKeyType GetKeyType()const{return RDLineKey;}
    const Value& GetValue()const{return m_KeyValue;}
    void SetValue(const Value& value){m_KeyValue = value;}

	virtual void Serialize(RDFileDataStream& buffer,bool bSave);

    virtual Value Interpolation(double& dWeight, const RDBaseKey<Value>& SecondKey)const = 0;
protected:
    Value m_KeyValue;
};

template <typename Value>
class RDLinearKey :public RDBaseKey<Value>
{
public:
    RDLinearKey(const Value& value):RDBaseKey<Value>(value){};
    RDLinearKey() = default;
    virtual Value Interpolation(double& dWeight, const RDBaseKey<Value>& SecondKey)const override;
};
#endif   // ----- #ifndef rdbasekey_INC  -----
