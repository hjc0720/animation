//email : hjc0720@gmail.com
//animation software

#include "RDMaterial.h"
#include "mac_define.h"

RDMatTexture::RDMatTexture()
    :m_bFileTex(true)
{
    m_vTexScale.x = 1;
    m_vTexScale.y = 1;
}

RDMatTexture::RDMatTexture(const QString& strFileName)
     :m_strFile(strFileName)
    ,m_bFileTex(true)
      ,m_bReleaseTex(true)
{
    m_vTexScale.x = 1;
    m_vTexScale.y = 1;
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    m_hTex = pDevice->CreateTexture(strFileName);
}

RDMatTexture::RDMatTexture(const uint* pBuffer,int nWidth,int nHeight)
    :m_bFileTex(false)
     ,m_bReleaseTex(true)
{
    m_vTexScale.x = 1;
    m_vTexScale.y = 1;
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    m_hTex = pDevice->CreateTexture(nWidth,nHeight,pBuffer,RDNormal2DTexture);
}
RDMatTexture::~RDMatTexture()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->ReleaseTexture(m_hTex);
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
        m_MatTexture[nTexType] = new RDMatTexture(fileName);
}

void RDMaterial::AddTex(RDMatTextureType nTexType,const uint* pBuffer,int nWidth,int nHeight)
{
    if(m_MatTexture[nTexType])
        SAFE_DELETE(m_MatTexture[nTexType]);

    m_MatTexture[nTexType] = new RDMatTexture(pBuffer,nWidth,nHeight);
}

void RDMaterial::AddTex(RDMatTextureType nTexType,RDTexHandle hTex,const QRect& texBound)
{

}
