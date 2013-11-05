//email : hjc0720@gmail.com
//animation software

#ifndef RDMATERIAL_H
#define RDMATERIAL_H
#include "HVector4f.h"
#include <QString>
#include "RDRenderDevice.h"
#include "mac_define.h"

enum RDMatTextureType
{
    RDNormalMatTexture,
    RDMatTextureCount,
};

class QRectF;
class RDMatTexture
{
public:
    RDMatTexture();
    RDMatTexture(const QString& strFileName);
    RDMatTexture(const uint* pBuffer,int nWidth,int nHeight);
    RDMatTexture(const RDTexture*  hTex,const QRectF& texBound);
    ~RDMatTexture();
    const QString& GetFileName()const{return m_strFile;}
protected:
    void        InitData();
protected:
    QString     m_strFile;
    bool        m_bFileTex;

    float2      m_vTexOffset;
    float2      m_vTexScale;
    float       m_fRotate;

    //临时数据
    union{
        const RDTexture*  m_hConstTex;
        RDTexture*  m_hTex;
    };
    matrix4x4  m_matTex;
    bool        m_bReleaseTex;
};

enum RDMatChangeType
{
    MT_MAT_NO_CHANGE = 0,
    MT_ADD_TEXTURE   = 1,
};

class RDMaterial
{
public:
    RDMaterial();
    RDMaterial(bool bEnableLight,unsigned int color);
    void AddTex(RDMatTextureType nTexType,const QString& fileName);
    void AddTex(RDMatTextureType nTexType,const uint* pBuffer,int nWidth,int nHeight);
    void AddTex(RDMatTextureType nTexType,const RDTexture*  hTex,const QRectF& texBount);
    bool UpdateFrame(const RDTime& time,char m_nPointLightNum,char m_nLineLightNum,char m_nSpotLightNum);

    int  SetChange(int nChangeType);
    void ClearChange(){m_nChange = MT_MAT_NO_CHANGE;};
    bool CheckChange( RDMatChangeType nType);
    RDShader* GetShader()const{return m_pShader;}
protected:
    void CreateShader();
    void GenerateShader();
protected:
    bool            m_bEnableLight;
    float4          m_vDiffuse;
    float4          m_vAmbient;
    float4          m_vSpecular;
    float           m_fShine;
    RDMatTexture*    m_MatTexture[RDMatTextureCount];

    //临时数据
    char             m_nPointLightNum;
    char             m_nLineLightNum;
    char             m_nSpotLightNum;

    RDTime          m_nNowTime;

    QString         m_strShaderName;
    QString         m_strShader;
    RDShader*       m_pShader;

    int             m_nChange;
};

#endif // RDMATERIAL_H
