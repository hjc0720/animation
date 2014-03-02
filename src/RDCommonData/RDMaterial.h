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
class RDUBO;
class RDMatTexture
{
public:
    RDMatTexture();
    RDMatTexture(const QString& strFileName);
    RDMatTexture(const uint* pBuffer,int nWidth,int nHeight);
    RDMatTexture(const RDTexture*  hTex,const QRectF& texBound);
    ~RDMatTexture();
    const QString&  GetFileName()const{return m_strFile;}
    void            SetParamToDevice(int nIndex,RDRenderDevice* pDevice );
    void            UpdateFrame(RDRenderDevice* pDevice ,const RDTime& time);
protected:
    void        InitData();
protected:
    QString     m_strFile;
    bool        m_bFileTex;

    float2      m_vTexCenter;
    float2      m_vTexOffset;
    float2      m_vTexScale;
    float       m_fRotate;
    float       m_fAlpha;

    //临时数据
    RDTexture*  m_hTex;
    matrix4x4   m_matTex;
    bool        m_bReleaseTex;
    RDUBO*      m_pTexParam;
};

enum RDMatChangeType
{
    RD_MAT_NO_CHANGE    = 0,
    RD_ADD_TEXTURE      = 1,
    RD_Mat_Param        =   2,
};

#define MAT_NEW_CHANGE RD_Mat_Param

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
    void ClearChange(){m_nChange = RD_MAT_NO_CHANGE;};
    bool CheckChange( RDMatChangeType nType);
    void SetParamToDevice();
protected:
    void    GenerateMatParam();
    size_t  GenerateMatParamBuffer(char* pBuffer);
    void    CreateShader();
    void    GenerateShader();
protected:
    bool            m_bEnableLight;
    float4          m_vDiffuse;
    float4          m_vAmbient;
    float4          m_vSpecular;
    float           m_fShine;
    RDMatTexture*   m_MatTexture[RDMatTextureCount];

    //临时数据
    char             m_nPointLightNum;
    char             m_nLineLightNum;
    char             m_nSpotLightNum;

    RDTime          m_nNowTime;

    QString         m_strShaderName;
    QString         m_strShader;
    RDShader*       m_pShader;
    RDUBO*           m_pMatParam;

    int             m_nChange;
};

#endif // RDMATERIAL_H
