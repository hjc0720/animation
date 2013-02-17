#include "RDRenderDevice.h"
#include "RDTexture.h"
#include <QGLContext>
#include <QDebug>
#include "mac_define.h"
#include <QFileInfo>
#include <QMutexLocker>
#include "RDShaderProgram.h"

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

const char* GetShaderExt(RDShaderType nType)
{
    switch(nType)
    {
    case VertexShader:
        return "_VS";
    case FragmentShader:
        return "_FS";
    case GeometryShader:
        return "_GS";
    }
    return "";
}

RDShader *RDRenderDevice::CreateShader(const QString &fileName, RDShaderType nType)
{
    QMutexLocker locker(&m_lock);
    QFileInfo info(fileName);
    QString strShaderName = info.baseName() + GetShaderExt(nType);
    RDShader* pShader = GetExistShader(strShaderName);

    if(!pShader)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QTextStream shader(&file);
        pShader = new RDShader(shader.readAll(),strShaderName,nType);
        m_vecShader[strShaderName] = pShader;
    }
    return pShader;
}

RDShader *RDRenderDevice::CreateShader(const QString& code,const QString& shaderName,RDShaderType nType)
{
    QMutexLocker locker(&m_lock);
    QString strShaderName = shaderName + GetShaderExt(nType);
    RDShader* pShader = GetExistShader(strShaderName);

    if(!pShader)
    {
        pShader = new RDShader(code,strShaderName,nType);
        m_vecShader[strShaderName] = pShader;
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
        return 4;
    case RDVB_Texcoord:
        return 2;
    case RDVB_Color:
        return 4;
    }
    return 4;
}

QString GetAttributeName(RDVertexBufferType nType)
{
    switch(nType)
    {
    case  RDVB_Pos:
        return QString("pos");
    case RDVB_Color:
        return QString("color");
    case RDVB_Normal:
        return QString("normal");
    case RDVB_Texcoord:
        return QString("texcoord");
    case RDVB_Tang:
        return QString("tang");
    case RDVB_Bi:
        return QString("bi");
    }
    return QString("");
}

RDShaderProgram *RDRenderDevice::CreateShaderProgram(RDShader *pVertexShader,  RDShader*pGeometryShader,  RDShader* pPixelShader)
{
    QMutexLocker locker(&m_lock);
    QString programName;
    if(pVertexShader)
        programName += pVertexShader->ShaderName() ;
    if(pGeometryShader)
        programName += pGeometryShader->ShaderName();
    if(pPixelShader)
        programName += pPixelShader->ShaderName();

    auto it = m_vecShaderProgram.find(programName) ;
    if(it != m_vecShaderProgram.end())
    {
        it->second->AddRef();
       return it->second;
    }
    
    RDShaderProgram* pShaderProgram = new RDShaderProgram(programName,pVertexShader,pGeometryShader,pPixelShader);
    m_vecShaderProgram[programName] = pShaderProgram;
    return pShaderProgram;
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

void RDRenderDevice::SetVertexBuffer(RDVertexBufferHandle pVertex)
{
    QMutexLocker locker(&m_lock);
    glBindVertexArray(pVertex->hVertexArray);
}

void RDRenderDevice::SetShader(RDShaderProgram *pShader)
{
    QMutexLocker locker(&m_lock);
    pShader->use();
}

void RDRenderDevice::SetShaderParam(RDShaderProgram *pShader, const char *name, float value)
{
    QMutexLocker locker(&m_lock);
    GLint hLocation = pShader->GetUniformLocation(name);
    glUniform1f(hLocation,value);
}

void RDRenderDevice::SetShaderParam(RDShaderProgram *pShader, const char *name, float3 &value)
{
    QMutexLocker locker(&m_lock);
    GLint hLocation = pShader->GetUniformLocation(name);
    glUniform3f(hLocation,value.x(),value.y(),value.z());
}

void RDRenderDevice::SetShaderParam(RDShaderProgram *pShader, const char *name, float4 &value)
{
    QMutexLocker locker(&m_lock);
    GLint hLocation = pShader->GetUniformLocation(name);
    glUniform4f(hLocation,value.x(),value.y(),value.z(),value.w());
}

void RDRenderDevice::SetShaderParam(RDShaderProgram *pShader, const char *name, HMatrixQ4F &value)
{
    QMutexLocker locker(&m_lock);
    GLint hLocation = pShader->GetUniformLocation(name);
    glUniformMatrix4fv(hLocation,1,false,value.data());
}

void RDRenderDevice::SetShaderTexture(RDShaderProgram *pShader, const char *name, RDTexHandle tex)
{
    QMutexLocker locker(&m_lock);
    GLint hLocation = pShader->GetUniformLocation(name);
    tex->SetTexture(hLocation);
}

void RDRenderDevice::SetShaderSample(RDTexHandle tex, RDSampleType nType)
{
    QMutexLocker locker(&m_lock);
    tex->SetTextureSample(nType);
}

void RDRenderDevice::Render(GLenum mode, GLint nStart, GLsizei count)
{
    QMutexLocker locker(&m_lock);
    glDrawArrays(mode,nStart,count);
}

bool RDRenderDevice::SetRenderTarget(RDTexHandle target, RDTexHandle depth)
{
    glBindFramebuffer(GL_FRAMEBUFFER,m_hFrameBuffer);
    bool ret = target->SetRenderTarget(0);
    ret &= depth->SetDepth();
    return ret;
}

void RDRenderDevice::SetViewPort(QRect &viewPort)
{
    QMutexLocker locker(&m_lock);
    glViewport(viewPort.left(),viewPort.right(),viewPort.width(),viewPort.height());
}

void RDRenderDevice::SetScissor(QRect &scissor)
{
    QMutexLocker locker(&m_lock);
    glScissor(scissor.left(),scissor.right(),scissor.width(),scissor.height());
}

void RDRenderDevice::DumpTexture(RDTexHandle pTex)
{
    QMutexLocker locker(&m_lock);
    RDTexture* pTexture = (RDTexture*)pTex;
    pTexture->Dump("/home/hjc/test.png");
}

RDRenderDevice::RDRenderDevice(const QGLContext* renderContex)
    :m_pDefaultContext(renderContex)
    ,m_lock(QMutex::Recursive)
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    qDebug() << "OpenGL version "<<major<<"."<<minor;

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&m_nMaxUseTexure);

    m_pCurState = new RDRenderState;
    m_pTempState = new RDRenderState;
    //glGenFramebuffers(1,&m_hFrameBuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER,m_hFrameBuffer);
}

RDShader *RDRenderDevice::GetExistShader(const QString &shaderName)
{
    QMutexLocker locker(&m_lock);
    auto it = m_vecShader.find(shaderName);
    if(it != m_vecShader.end())
    {
        it->second->AddRef();
        return it->second;
    }
    return nullptr;
}
