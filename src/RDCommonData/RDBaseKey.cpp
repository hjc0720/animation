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
void RDBaseKey<Value>::Serialize(RDFileDataStream& buffer,bool bSave)
{
	buffer.Serialize(m_KeyValue,bSave);
}
// =====================================================================================
template <typename Value>
Value RDLinearKey<Value>::Interpolation(double& dWeight, const RDBaseKey<Value>& SecondKey)const
{
	return (1 - dWeight) * this->GetValue() + dWeight * SecondKey.GetValue();
}
// =====================================================================================
template <typename Value>
RDBaseKey<Value>* RDBaseKey<Value>::CreateKey(const Value& value, RDKeyType type /* = RDLineKey  */) 
{
	switch(type)
	{
	case RDLineKey:
		return new RDLinearKey<Value>(value);
	}
	return nullptr;
}

template <typename Value>
RDBaseKey<Value>* RDBaseKey<Value>::CreateKey(RDKeyType type /* = RDLineKey  */)
{
	switch(type)
	{
	case RDLineKey:
		return new RDLinearKey<Value>();
	}
	return nullptr;
}
// =====================================================================================
template class RDBaseKey<float3>;
template class RDBaseKey<float>;
