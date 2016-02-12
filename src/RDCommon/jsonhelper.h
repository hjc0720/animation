/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QDebug>
#include <jsoncpp/json/json.h>
#include <string>

class float3;
template<typename T>
class JsonHelper
{
public:
    Json::Value toJson(const T& value)
    {
        return Json::Value(value);
    }

    void fromJson(T& t,const Json::Value& json)
    {
        t =  static_cast<T>(json.asInt64());
    }
};

template<>
class JsonHelper<bool>
{
public:
    Json::Value toJson(const bool& value) { return Json::Value(value); }
    void fromJson(bool& v,const Json::Value& json){ v = json.asBool(); }
};

template<>
class JsonHelper<std::string>
{
public:
    Json::Value toJson(const std::string& value) { return Json::Value(value); }
    void fromJson(std::string& v,const Json::Value& json){ v = json.asString(); }
};

template<>
class JsonHelper<float3>
{
public:
    Json::Value toJson(const float3& value);
    void fromJson(float3& v,const Json::Value& json);
};

class RDMd5;
template<>
class JsonHelper<RDMd5>
{
public:
    Json::Value toJson(const RDMd5& value);
    void fromJson(RDMd5&,const Json::Value& json);
};

class QUuid;
template<>
class JsonHelper<QUuid>
{
public:
    Json::Value toJson(const QUuid& value);
    void fromJson(QUuid&,const Json::Value& json);
};

class matrix4x4;
QDebug operator<<(QDebug dbg, const matrix4x4 &mat);
QDebug operator<<(QDebug dbg, const float3 & vec);
QDebug operator<<(QDebug dbg, const std::string & vec);

#endif // JSONHELPER_H
