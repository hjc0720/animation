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
#include <type_traits>
#include "rdexception.h"

class RDJsonDataStream;
class float3;
namespace Json
{
class Value;
}
enum RDKeyType
{
    RDLineKey,
};

enum RDKeyValueType
{
    RDFloat3,
    RDInt,
    RDFloat
};

template<typename>
struct is_float3 : public std::false_type { };

template<>
struct is_float3<float3> : public std::true_type { };

class RDBaseKey{
public:
    virtual RDKeyValueType ValueType()const=0;
    virtual void Serialize(RDJsonDataStream &buffer, Json::Value &parent)=0;
};

template <typename Value>
class RDKey :public RDBaseKey
{
public:
    static RDKey<Value>* CreateKey(const Value& value, RDKeyType type = RDLineKey);
    static RDKey<Value>* CreateKey(RDKeyType type = RDLineKey);
public:
    RDKey(const Value& value):m_KeyValue(value){}
    RDKey() = default;
    virtual ~RDKey(){}
    virtual RDKeyType GetKeyType()const{return RDLineKey;}
    const Value& GetValue()const{return m_KeyValue;}
    void SetValue(const Value& value){m_KeyValue = value;}
    void Serialize(RDJsonDataStream &buffer, Json::Value &parent)override;
    virtual RDKeyValueType ValueType()const override
    {
        if(std::is_integral<Value>::value)
            return RDInt;
        else if(std::is_floating_point<Value>::value)
            return RDFloat;
        else if(is_float3<Value>::value)
            return RDFloat3;
        else
            throw RDException();
    }

    virtual Value Interpolation(double& dWeight, const RDKey<Value>& SecondKey)const = 0;
protected:
    Value m_KeyValue;
};

template <typename Value>
class RDLinearKey :public RDKey<Value>
{
public:
    RDLinearKey(const Value& value):RDKey<Value>(value){};
    RDLinearKey() = default;
    virtual Value Interpolation(double& dWeight, const RDKey<Value>& SecondKey)const override;
};

#endif   // ----- #ifndef rdbasekey_INC  -----
