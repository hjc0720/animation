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

RDLight::RDLight(const QString& strName,RDLightType nType,const float3& vPos)
    :RDNode(strName, vPos,nullptr)
     ,m_bEnable(true)
     ,m_nType(nType)
     ,m_vDiffuseColor(1,1,1,1)
     ,m_vSpecColor(1,1,1,1)
{
    if(nType == RDSpotLight)
        m_pSpotParam = new RDSpotParam;
    else
        m_pSpotParam = nullptr;
}

void RDLight::Serialize(RDFileDataStream& buffer,bool bSave)
{
    QMutexLocker locker(&m_lock);
    int nVersion = 0;
    buffer.Serialize(nVersion,bSave);
    buffer.Serialize(m_bEnable,bSave);
    int nLightType = m_nType;
    buffer.Serialize(nLightType,bSave);
    if(!bSave)
        m_nType = static_cast<RDLightType>(nLightType);
    if(m_nType == RDSpotLight)
    {
        if(!bSave && !m_pSpotParam)
            m_pSpotParam = new RDSpotParam ;
        buffer.Serialize(m_pSpotParam->fInner,bSave);
        buffer.Serialize(m_pSpotParam->fOuter,bSave);
        buffer.Serialize(m_pSpotParam->fAtten,bSave);
    }
}

size_t    RDLight::GenerateShaderParam(char* pBuffer,const QString& name)
{
    size_t size = 0;
    float fNoUse = 0;
    switch(m_nType)
    {
    case RDPointLight:
        size += RDSaveData(pBuffer,GetDynamicPos(name));
        size += RDSaveData(pBuffer,fNoUse);
    default:
        return 0;
    }
    for(int i = 0; i < 4; i++)
        size += RDSaveData(pBuffer,m_fIntensity);
    size += RDSaveData(pBuffer,m_vDiffuseColor);
    size += RDSaveData(pBuffer,m_vSpecColor);
    return size;
}
