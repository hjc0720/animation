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

void RDLight::Serialize(RDJsonDataStream& buffer, Json::Value &parent, bool bSave)
{
    RDNode::Serialize(buffer,parent,bSave);
    RDSingleLock locker(m_lock);
    buffer.Serialize(parent,"enable",bSave,m_bEnable);
    buffer.Serialize(parent,"type",bSave,m_nType);
    if(m_nType == RDSpotLight)
    {
        if(!bSave && !m_pSpotParam)
            m_pSpotParam = new RDSpotParam ;
        buffer.Serialize(parent,"spot_inner",bSave,m_pSpotParam->fInner);
        buffer.Serialize(parent,"spot_out",bSave,m_pSpotParam->fOuter);
        buffer.Serialize(parent,"spot_atten",bSave,m_pSpotParam->fAtten);
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
