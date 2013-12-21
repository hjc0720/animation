//email : hjc0720@gmail.com
//animation software

#include "RDMaterial.h"
#include "mac_define.h"
#include <QRectF>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class RDMatShaderCode
{
public:
    RDMatShaderCode(const QString& filePath){
        if(!LoadCode(filePath))  m_pPath = new QString(filePath);
        else                    m_pPath = nullptr;
    }
    ~RDMatShaderCode(){ SAFE_DELETE(m_pPath);}
    const QString& GetStr(){
        if(m_pPath){
            LoadCode(*m_pPath);
            SAFE_DELETE(m_pPath);
        }
        return m_strCode;
    }
protected:
    bool LoadCode(const QString& filePath){
        QFile shader(filePath);
        if(shader.open(QIODevice::ReadOnly)){
            QTextStream stream(&shader);
            m_strCode = stream.readAll();
            return true;
        }
        return false;
    }
protected:
    QString* m_pPath;
    QString m_strCode;
};

RDMatShaderCode MainMatCode(":/shader/main_ps");
RDMatShaderCode TexMatCode[] = {QString(":/shader/diffuse_ps")};
RDMatShaderCode ParamMatCode[] = {QString(":/shader/buffer_ps")};

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

RDMatTexture::RDMatTexture(const RDTexture*  hTex,const QRectF& texBound)
     :m_bReleaseTex(false)
{
    m_hTex = const_cast<RDTexture*>(hTex);
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    int nWidth = pDevice->GetTextureWidth(hTex);
    int nHeight = pDevice->GetTextureHeight(hTex);
    m_vTexCenter.x = 0;
    m_vTexCenter.y = 0;

    m_vTexOffset.x = texBound.left() / nWidth;
    m_vTexOffset.y = texBound.top() / nHeight;

    m_vTexScale.x = texBound.width() / nWidth;
    m_vTexScale.y = texBound.height() / nHeight;
    
    m_fRotate = 0;
    m_fAlpha = 1;

    m_pTexParam = nullptr;
}

RDMatTexture::~RDMatTexture()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    if(m_bReleaseTex)
        pDevice->ReleaseTexture(m_hTex);
    pDevice->ReleaseUniformBufferObject(m_pTexParam);
}

void        RDMatTexture::InitData()
{
    m_vTexOffset.x = 0;
    m_vTexOffset.y = 0;
    m_vTexCenter.x = 0;
    m_vTexCenter.y = 0;
    m_fRotate = 0;
    m_vTexScale.x = 1;
    m_vTexScale.y = 1;
    m_hTex = nullptr;
    m_pTexParam = nullptr;
    m_fAlpha = 1;
}

void            RDMatTexture::SetParamToDevice(int nIndex,RDRenderDevice* pDevice )
{
    pDevice->SetShaderTexture(nIndex,m_hTex);
    pDevice->SetShaderParam(2 + nIndex,m_pTexParam);
}

void            RDMatTexture::UpdateFrame(RDRenderDevice* pDevice ,const RDTime& )
{
    m_matTex = matrix4x4(m_vTexCenter,HMatrixQ4F_POS) * matrix4x4(m_vTexScale.x,m_vTexScale.y,1,HMatrixQ4F_Scale) 
        * matrix4x4(0,0,m_fRotate,HMatrixQ4F_Rotate) * matrix4x4(m_vTexOffset,HMatrixQ4F_POS); 
    float vec[5 * 4];
    memcpy(vec,m_matTex.data(),4*4*sizeof(float));
    vec[4*4] = m_fAlpha;
    
    if(!m_pTexParam)
        m_pTexParam = pDevice->CreateUniformBufferObject(5*4,vec);
    else
        pDevice->ModifyUniformBufferObject(m_pTexParam,vec);
}
////////////////////////////////////////////////////////////////////////////////
RDMaterial::RDMaterial()
{
    memset(m_MatTexture,0,sizeof(RDMatTexture*) * RDMatTextureCount);
    m_pShader = nullptr;
    m_nNowTime = 0;
    m_nChange = MT_ADD_TEXTURE;
}

RDMaterial::RDMaterial(bool bEnableLight,unsigned int color)
    :m_bEnableLight(bEnableLight)
     ,m_vDiffuse(color)
     ,m_vAmbient(0xffffffff)
     ,m_vSpecular(color)
     ,m_fShine(32)
{
    memset(m_MatTexture,0,sizeof(RDMatTexture*) * RDMatTextureCount);
    m_nNowTime = 0;
    m_nChange = MT_ADD_TEXTURE;
    m_pShader = nullptr;
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

void RDMaterial::AddTex(RDMatTextureType nTexType,const RDTexture*  hTex,const QRectF& texBound)
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
    if(CheckChange(MT_ADD_TEXTURE))
    {
        RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
        for(int i = 0; i < RDMatTextureCount;i++)
        {
            if(m_MatTexture[i])
                m_MatTexture[i]->UpdateFrame(pDevice,time);
        }
    }
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

    m_strShaderName = shaderName;
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->ReleaseShader(m_pShader);
    m_pShader = nullptr;

    GenerateShader();
    m_pShader = pDevice->CreateShader(m_strShader,m_strShaderName,FragmentShader);
}

void RDMaterial::GenerateShader()
{
    qDebug() << "main shader:\n"<< MainMatCode.GetStr();
    m_strShader = MainMatCode.GetStr();
    int nParamIndex = m_strShader.indexOf("//end param");
    for(int i = 0;i < RDMatTextureCount; i++)
    {
        if(m_MatTexture[i])
            m_strShader.insert(nParamIndex,ParamMatCode[i].GetStr());
        qDebug() << "tex shader:\n"<< ParamMatCode[i].GetStr();
    }
    int nCodeIndex = m_strShader.indexOf("//end code");
    for(int i = 0; i < RDMatTextureCount;i++)
    {
        if(m_MatTexture[i])
            m_strShader.insert(nCodeIndex,TexMatCode[i].GetStr());
    }
    qDebug() << "material shader:\n"<< m_strShader;
}

void RDMaterial::SetParamToDevice()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->SetShader(m_pShader,FragmentShader);

    for(int i = 0; i < RDMatTextureCount; i++)
    {
        if(m_MatTexture[i])
        {
            m_MatTexture[i]->SetParamToDevice(i,pDevice);
        }
    }
}
