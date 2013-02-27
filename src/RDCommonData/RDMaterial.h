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

class RDMatTexture
{
public:
    RDMatTexture();
    RDMatTexture(const QString& strFileName);
    RDMatTexture(const uint* pBuffer,int nWidth,int nHeight);
    ~RDMatTexture();
    const QString& GetFileName()const{return m_strFile;}
protected:
    QString     m_strFile;
    bool        m_bFileTex;
    float2      m_vTexOffset;
    float2      m_vTexScale;
    float       m_fRotate;

    //临时数据
    RDTexHandle m_hTex;
    HMatrixQ4F  m_matTex;
    bool        m_bReleaseTex;
};

class RDMaterial
{
public:
    RDMaterial();
    RDMaterial(bool bEnableLight,unsigned int color);
    void AddTex(RDMatTextureType nTexType,const QString& fileName);
    void AddTex(RDMatTextureType nTexType,const uint* pBuffer,int nWidth,int nHeight);
    void AddTex(RDMatTextureType nTexType,RDTexHandle hTex,const QRect& texBount);
protected:
    bool            m_bEnableLight;
    float4          m_vDiffuse;
    float4          m_vAmbient;
    float4          m_vSpecular;
    float           m_fShine;
    RDMatTexture*    m_MatTexture[RDMatTextureCount];

    //临时数据
    QString         m_strShaderName;
    QString         m_strShader;
};

#endif // RDMATERIAL_H
