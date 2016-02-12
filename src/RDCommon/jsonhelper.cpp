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

#include "jsonhelper.h"
#include "HMatrixQ4F.h"
#include "HVector3f.h"
#include <string>

using namespace std;


//================================================================================
QDebug operator<<(QDebug dbg, const matrix4x4 &mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            dbg.nospace() << mat.Get(i,j) << "\t";
        }
        dbg.nospace() << ";\n";
    }
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const float3 & vec)
{
    for(int i = 0; i < 3; i++)
        dbg.nospace() << vec.GetData()[i] << "\t";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::string & vec)
{
    if(!vec.empty())
        dbg << vec.c_str();
    return dbg.space();
}


#include "HVector3f.h"
using namespace Json;

#include "RDMd5.h"
Json::Value JsonHelper<RDMd5>::toJson(const RDMd5& value)
{
    return Json::Value(value.GetMd5String());
}

void JsonHelper<RDMd5>::fromJson(RDMd5& v,const Json::Value& json)
{
    v = RDMd5(json.asString());
}

Value JsonHelper<float3>::toJson(const float3 &value)
{
    Value ret(arrayValue);
    ret.append(Value(value.x()));
    ret.append(Value(value.y()));
    ret.append(Value(value.z()));
    return ret;
}

void JsonHelper<float3>::fromJson(float3 &v, const Value &json)
{
    v.Set(json[0].asFloat(),json[1].asFloat(),json[2].asFloat());
}

#include <QUuid>
Value JsonHelper<QUuid>::toJson(const QUuid &value)
{
    return Value(value.toString().toStdString());
}

void JsonHelper<QUuid>::fromJson(QUuid &v, const Value &json)
{
    v = QUuid(json.asCString());
}
