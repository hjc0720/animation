//email : hjc0720@gmail.com
//animation software

#include "RDMaterial.h"
#include "mac_define.h"
#include <QRectF>
#include <QFile>
#include <QTextStream>

RDMatTexture::RDMatTexture()
    :m_bFileTex(true)
{
    InitData();
}

RDMatTexture::RDMatTexture(const QString& strFileName)
     :m_strFile(strFileName)
    ,m_bFileTex(true)
      ,m_bReleaseTex(true)
{
    InitData();
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    m_hTex = pDevice->CreateTexture(strFileName);
}

RDMatTexture::RDMatTexture(const uint* pBuffer,int nWidth,int nHeight)
    :m_bFileTex(false)
     ,m_bReleaseTex(true)
{
    InitData();
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    m_hTex = pDevice->CreateTexture(nWidth,nHeight,pBuffer,RDNormal2DTexture);
}

RDMatTexture::RDMatTexture(RDTexture*  hTex,const QRectF& texBound)
    :m_hTex(hTex)
     ,m_bReleaseTex(false)
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    int nWidth = pDevice->GetTextureWidth(hTex);
    int nHeight = pDevice->GetTextureHeight(hTex);

    m_vTexOffset.x = texBound.left() / nWidth;
    m_vTexOffset.y = texBound.top() / nHeight;

    m_vTexScale.x = texBound.width() / nWidth;
    m_vTexScale.y = texBound.height() / nHeight;
    
    m_fRotate = 0;
}

RDMatTexture::~RDMatTexture()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    if(m_bReleaseTex)
        pDevice->ReleaseTexture(m_hTex);
}
void        RDMatTexture::InitData()
{
    m_vTexOffset.x = 0;
    m_vTexOffset.y = 0;
    m_fRotate = 0;
    m_vTexScale.x = 1;
    m_vTexScale.y = 1;
}
////////////////////////////////////////////////////////////////////////////////
RDMaterial::RDMaterial()
{
}

RDMaterial::RDMaterial(bool bEnableLight,unsigned int color)
    :m_bEnableLight(bEnableLight)
     ,m_vDiffuse(color)
     ,m_vAmbient(0xffffffff)
     ,m_vSpecular(color)
     ,m_fShine(32)
{
}

void RDMaterial::AddTex(RDMatTextureType nTexType,const QString& fileName)
{
    if(m_MatTexture[nTexType])
    {
        if(fileName != m_MatTexture[nTexType]->GetFileName())
            SAFE_DELETE(m_MatTexture[nTexType]);
    }
    if(!m_MatTexture[nTexType])
    {
        m_MatTexture[nTexType] = new RDMatTexture(fileName);
        SetChange(MT_ADD_TEXTURE);
    }
}

void RDMaterial::AddTex(RDMatTextureType nTexType,const uint* pBuffer,int nWidth,int nHeight)
{
    if(m_MatTexture[nTexType])
        SAFE_DELETE(m_MatTexture[nTexType]);

    m_MatTexture[nTexType] = new RDMatTexture(pBuffer,nWidth,nHeight);
    SetChange(MT_ADD_TEXTURE);
}

void RDMaterial::AddTex(RDMatTextureType nTexType,RDTexture*  hTex,const QRectF& texBound)
{
    if(m_MatTexture[nTexType])
        SAFE_DELETE(m_MatTexture[nTexType]);
    m_MatTexture[nTexType] = new RDMatTexture(hTex,texBound);
    SetChange(MT_ADD_TEXTURE);
}

bool RDMaterial::UpdateFrame(const RDTime& time,char ,char ,char )
{
    if(m_nNowTime == time && m_nChange == MT_MAT_NO_CHANGE)
        return false;
    if(CheckChange(MT_ADD_TEXTURE))
        CreateShader();
    ClearChange();
    return true;
}

bool RDMaterial::CheckChange( RDMatChangeType nType)
{
    return (m_nChange & nType) != 0;
}

int  RDMaterial::SetChange(int nChangeType)
{
    m_nChange |= nChangeType;
    return m_nChange;
}

const QString texShaderName[] = 
{
    "Normal_",
};

void RDMaterial::CreateShader()
{
    QString shaderName;
    for(int i = 0; i < RDMatTextureCount; i++)
    {
        if(m_MatTexture[i])
            shaderName += texShaderName[i];
    }
    shaderName += ("ps");
    if(m_strShaderName == shaderName)
        return;

    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->ReleaseShader(m_pShader);
    m_pShader = nullptr;

    GenerateShader();
    m_pShader = pDevice->CreateShader(m_strShader,m_strShaderName,FragmentShader);
}

void RDMaterial::GenerateShader()
{
    QFile file(":/main_ps");
    
    file.open(QIODevice::ReadOnly);
    QTextStream shader(&file);
    m_strShader = shader.readAll();
}
