#include "RDRenderDevice.h"
#include "RDTexture.h"
#include <QGLContext>
#include <QDebug>
#include "mac_define.h"
#include <QFileInfo>
#include <QMutexLocker>

#define SINGLE_CONTEX

class RDRenderState
{
};

struct RDFileTexture
{
    RDFileTexture(){nRef = 0; pTexture = 0;}
    ~RDFileTexture(){ SAFE_DELETE(pTexture);}
    int nRef;
    RDTexture* pTexture;
};

struct RDShader
{
    RDShader(){nRef = 0; pShader = 0;}
    ~RDShader(){ SAFE_DELETE(pShader);}
    int nRef;
    QGLShader* pShader;
};

struct RDVertexBuffer
{
    GLuint hVertexBuffer;
    RDVertexBufferType nType;
};

struct RDVertexArray
{
    GLuint hVertexArray;
    std::vector<RDVertexBuffer> arVertexBuffer;
};

////////////////////////////////////////////////////////////////////////////////
RDRenderDevice* g_pRenderManager = nullptr;
////////////////////////////////////////////////////////////////////////////////
RDRenderDevice * RDRenderDevice::CreateRenderManager(const QGLContext* renderContex)
{
    if(!g_pRenderManager)
    {
        g_pRenderManager = new RDRenderDevice(renderContex);
    }
    return g_pRenderManager;
}

RDRenderDevice* RDRenderDevice::GetRenderManager()
{
    return g_pRenderManager;
}

RDTexHandle RDRenderDevice::CreateTexture(const QString &fileName)
{
    QMutexLocker locker(&m_lock);
    auto it = m_vecFileTex.find(fileName);
    if(it != m_vecFileTex.end())
    {
        it->second->nRef++;
        return it->second->pTexture;
    }

    RDFileTexture* pFileTex = new RDFileTexture;
    pFileTex->pTexture = new RDTexture(fileName);
    pFileTex->nRef = 1;
    m_vecFileTex[fileName] = pFileTex;
    return pFileTex->pTexture;
}

RDTexHandle RDRenderDevice::CreateTexture(int nWidth, int nHeight,  RDTexture_Type nType)
{
    QMutexLocker locker(&m_lock);
    RDTexture* pTexture = new RDTexture(nWidth,nHeight,nType);
    m_vecTex.push_back(pTexture);
    return pTexture;
}

const char* GetShaderExt(QGLShader::ShaderType nType)
{
    switch(nType)
    {
    case QGLShader::Vertex:
        return "_VS";
    case QGLShader::Fragment:
        return "_FS";
    case QGLShader::Geometry:
        return "_GS";
    }
    return "";
}

QGLShader *RDRenderDevice::CreateShader(const QString &fileName, QGLShader::ShaderType nType)
{
    QMutexLocker locker(&m_lock);
    QFileInfo info(fileName);
    QString strShaderName = info.baseName() + GetShaderExt(nType);
    QGLShader* pShader = GetExistShader(strShaderName);

    if(!pShader)
    {
        RDShader* pNewShader = new RDShader();
        pNewShader->pShader = new QGLShader(nType,m_pDefaultContext);
        pNewShader->pShader->compileSourceFile(fileName);
        pNewShader->nRef = 1;
        m_vecShader[strShaderName] = pNewShader;
        pShader = pNewShader->pShader;
    }
    return pShader;
}

QGLShader *RDRenderDevice::CreateShader(const QString &code, const QString &shaderName, QGLShader::ShaderType nType)
{
    QMutexLocker locker(&m_lock);
    QString strShaderName = shaderName + GetShaderExt(nType);
    QGLShader* pShader = GetExistShader(strShaderName);

    if(!pShader)
    {
        RDShader* pNewShader = new RDShader();
        pNewShader->pShader = new QGLShader(nType,m_pDefaultContext);
        pNewShader->pShader->compileSourceCode(code);
        pNewShader->nRef = 1;
        m_vecShader[strShaderName] = pNewShader;
        pShader = pNewShader->pShader;
    }
    return pShader;
}

int GetPtSize(RDVertexBufferType nType)
{
    switch(nType)
    {
    case RDVB_Pos:
    case RDVB_Normal:
    case RDVB_Bi:
    case RDVB_Tang:
        return 3;
    case RDVB_Texcoord:
        return 2;
    case RDVB_Color:
        return 4;
    }
    return 4;
}
RDVertexBufferHandle RDRenderDevice::CreateVertexBuffer(const std::vector<RDVertexData> &arVertexData)
{
    QMutexLocker locker(&m_lock);
    size_t nBufferCount = arVertexData.size();

    RDVertexArray* pVertexArray = new RDVertexArray;
    glGenVertexArrays(1,&pVertexArray->hVertexArray);
    glBindVertexArray(pVertexArray->hVertexArray);

    GLuint* pBuffer = new GLuint[nBufferCount];
    glGenBuffers(nBufferCount,pBuffer);
    for(size_t i = 0; i < nBufferCount; i++)
    {
        RDVertexBuffer vertexBuffer;
        vertexBuffer.hVertexBuffer = pBuffer[i];
        vertexBuffer.nType = arVertexData[i].nType;
        glEnableVertexAttribArray(arVertexData[i].nType);
        glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer.hVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,arVertexData[i].nVertexCount * sizeof(float),arVertexData[i].pVertexData,GL_STATIC_DRAW);
        glVertexAttribPointer(arVertexData[i].nType,GetPtSize(arVertexData[i].nType),GL_FLOAT,GL_FALSE,0,nullptr);
        pVertexArray->arVertexBuffer.push_back(vertexBuffer);
    }
    m_vecVertexBuffer.push_back(pVertexArray);
    SAFE_DELETE_ARRAY(pBuffer);
    return pVertexArray;
}

void RDRenderDevice::DumpTexture(RDTexHandle pTex)
{
    QMutexLocker locker(&m_lock);
    RDTexture* pTexture = (RDTexture*)pTex;
    pTexture->Dump("/home/hjc/test.png");
}

RDRenderDevice::RDRenderDevice(const QGLContext* renderContex)
    :m_pDefaultContext(renderContex)
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    qDebug() << "OpenGL version "<<major<<"."<<minor;

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&m_nMaxUseTexure);
    m_pCurState = new RDRenderState;
    m_pTempState = new RDRenderState;
}

QGLShader *RDRenderDevice::GetExistShader(const QString &shaderName)
{
    QMutexLocker locker(&m_lock);
    auto it = m_vecShader.find(shaderName);
    if(it != m_vecShader.end())
    {
        it->second->nRef++;
        return it->second->pShader;
    }
    return nullptr;
}
