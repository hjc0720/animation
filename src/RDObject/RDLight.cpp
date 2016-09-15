/*
 * =====================================================================================
 *
 *       Filename:  RDLight.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年12月21日 20时15分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang JianChao (hjc), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "RDLight.h"
#include "RDCreateObj.h"
#include "RDFileDataStream.h"

RDObjectCreator<RDLight,false> lightCreator;

RDLight::RDLight(const std::string& strName,RDLightType nType,const float3& vPos)
    :RDNode(strName, vPos,nullptr)
     ,m_bEnable(true)
     ,m_nType(nType)
     ,m_vDiffuseColor(1,1,1,1)
     ,m_vSpecColor(1,1,1,1)
{
    m_fIntensity = 1;
    if(nType == RDSpotLight)
        m_pSpotParam = new RDSpotParam;
    else
        m_pSpotParam = nullptr;
}

void RDLight::Serialize(RDJsonDataStream& buffer, Json::Value &parent)
{
    RDNode::Serialize(buffer,parent);
    RDSingleLock locker(m_lock);
    buffer.Serialize(parent,"enable",m_bEnable);
    buffer.Serialize(parent,"type",m_nType);
    if(m_nType == RDSpotLight)
    {
        if(!buffer.IsSave() && !m_pSpotParam)
            m_pSpotParam = new RDSpotParam ;
        buffer.Serialize(parent,"spot_inner",m_pSpotParam->fInner);
        buffer.Serialize(parent,"spot_out",m_pSpotParam->fOuter);
        buffer.Serialize(parent,"spot_atten",m_pSpotParam->fAtten);
    }
}

template<>
class JsonHelper<RDLightType>
{
public:
    Json::Value toJson(const RDLightType& value){ return Json::Value(value);}
    void fromJson(RDLightType& t,const Json::Value& json){t = static_cast<RDLightType>(json.asInt());}
};

size_t    RDLight::GenerateShaderParam(char* pBuffer,const std::string& name)
{
    if(!GetCurSection(name))
        return 0;
    size_t size = 0;
    switch(m_nType)
    {
    case RDPointLight:
        size += RDSaveData(pBuffer,GetDynamicPos(name));
        break;
    default:
        return 0;
    }
    for(int i = 0; i < 4; i++)
        size += RDSaveData(pBuffer,m_fIntensity);
    size += RDSaveData(pBuffer,m_vDiffuseColor);
    size += RDSaveData(pBuffer,m_vSpecColor);
    return size;
}
