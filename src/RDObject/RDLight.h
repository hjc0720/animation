/*
 * =====================================================================================
 *
 *       Filename:  RDLight.h
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

#include "RDNode.h"
#include <atomic>

enum RDLightType
{
    RDPointLight,
    RDLineLight,
    RDSpotLight,
    RDLightTypeCount,
};

struct RDSpotParam
{
    float fInner;
    float fOuter;
    float fAtten;
};

class RDLight:public RDNode
{
public:
    RDLight()
        :m_bEnable(true),m_nType(RDPointLight),m_fIntensity(1),m_pSpotParam(nullptr)
    {};
    RDLight(const std::string& strName,RDLightType nType,const float3& vPos);

    void SetLigthIntensity(float fIntensity){m_fIntensity = fIntensity;}
    float GetLigthIntensity(){return m_fIntensity; }

    virtual void Serialize(RDFileDataStream& buffer,bool bSave);
    size_t    GenerateShaderParam(char* pBuffer,const std::string& name);
    RDLightType GetLightType()const{return m_nType;}
protected:
    ~RDLight(){}
protected:
    bool        m_bEnable;
    RDLightType m_nType;        //灯光类型
    float       m_fIntensity;   //光强
    RDSpotParam* m_pSpotParam;  //聚光灯参数
    float4          m_vDiffuseColor;
    float4          m_vSpecColor;
//动态数据
};
