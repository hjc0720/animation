#include "RDRenderDevice.h"
#include "RDTexture.h"
#include <QGLContext>
#include <QDebug>
#include "mac_define.h"
#include <QFileInfo>
#include <QMutexLocker>
#include "RDShaderProgram.h"
#include <algorithm>

#define SINGLE_CONTEX

class RDRenderState
{
};

//struct RDFileTexture
//{
//    RDFileTexture(){nRef = 0; pTexture = 0;}
//    ~RDFileTexture(){ SAFE_DELETE(pTexture);}
//    int nRef;
//    RDTexture* pTexture;
//};

enum RDVertexType
{
    RDVB_Pos = 0,
    RDVB_Color,
    RDVB_Normal,
    RDVB_Texcoord,
    RDVB_Tang,
    RDVB_Bi
};

struct RDVectexDecl
{
    RDVertexType type;
    short size;
    size_t offset;
};


short GetPtSize(RDVertexType nType)
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

class RDVertexBufferDecl
{
public:
    void setType(std::vector<RDVertexType> vTypes)
    {
        size_t offset = 0;
        for(RDVertexType eType:vTypes)
        {
            m_vDecl.push_back(RDVectexDecl{eType,GetPtSize(eType),offset});
            offset += GetPtSize(eType)*sizeof(float);
        }
    }
public:
    std::vector<RDVectexDecl> m_vDecl;
};

//struct RDVertexBuffer
//{
//    GLuint hVertexBuffer;
//    RDVertexBufferType nType;
//};

struct RDUBO
{
    GLuint hUbo;
    size_t nBufferSize;
};

struct RDVertexArray
{
    GLuint hVertexArray;
    //std::vector<RDVertexBuffer> arVertexBuffer;
    GLuint hVertexBuffer;
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

void RDRenderDevice::ReleaseRenderManager()
{
    SAFE_DELETE(g_pRenderManager);
}
////////////////////////////////////////////////////////////////////////////////

RDRenderDevice::~RDRenderDevice()
{
    std::for_each(m_vecShaderProgram.begin(),m_vecShaderProgram.end(),[](std::pair<QString,RDShaderProgram*> pair)
            {SAFE_DELETE(pair.second);});
}

RDTexture*  RDRenderDevice::CreateTexture(const QString &fileName)
{
    QMutexLocker locker(&m_lock);
    auto it = m_vecFileTex.find(fileName);
    if(it != m_vecFileTex.end())
    {
        it->second->AddRef();
        return it->second;
    }

    RDTexture* pFileTex = new RDTexture(fileName);
    m_vecFileTex[fileName] = pFileTex;
    Q_ASSERT(!checkError());
    return pFileTex;
}

RDTexture*  RDRenderDevice::CreateTexture(int nWidth, int nHeight,const uint* buffer,  RDTexture_Type nType)
{
    QMutexLocker locker(&m_lock);
    RDTexture* pTexture = new RDTexture(nWidth,nHeight,buffer,nType);
//    m_vecTex.push_back(pTexture);
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
    default:
        return "";
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
        Q_ASSERT(!checkError());
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
        Q_ASSERT(!checkError());
    }    
    return pShader;
}


//QString GetAttributeName(RDVertexType nType)
//{
//    switch(nType)
//    {
//    case  RDVB_Pos:
//        return QString("pos");
//    case RDVB_Color:
//        return QString("color");
//    case RDVB_Normal:
//        return QString("normal");
//    case RDVB_Texcoord:
//        return QString("texcoord");
//    case RDVB_Tang:
//        return QString("tang");
//    case RDVB_Bi:
//        return QString("bi");
//    }
//    return QString("");
//}

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
       return it->second;
    
    RDShaderProgram* pShaderProgram = new RDShaderProgram(programName,pVertexShader,pGeometryShader,pPixelShader);
    m_vecShaderProgram[programName] = pShaderProgram;
    Q_ASSERT(!checkError());
    return pShaderProgram;
}

RDVertexBufferHandle RDRenderDevice::CreateVertexBuffer(float* pVertexData,int nVertexCount,size_t nVertexSize,RDVertexBufferType eType)
{
    QMutexLocker locker(&m_lock);
    //size_t nBufferCount = arVertexData.size();

    RDVertexArray* pVertexArray = new RDVertexArray;
    glGenVertexArrays(1,&pVertexArray->hVertexArray);
    glBindVertexArray(pVertexArray->hVertexArray);

    glGenBuffers(1,&pVertexArray->hVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,pVertexArray->hVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,nVertexCount * nVertexSize,pVertexData,GL_STATIC_DRAW);
    std::vector<RDVectexDecl>& vDecl = m_pVertexBufferDecl[eType].m_vDecl;
    for(size_t i = 0; i < vDecl.size(); i++)
    {        
        glEnableVertexAttribArray(vDecl[i].type);
        glVertexAttribPointer(vDecl[i].type,vDecl[i].size,GL_FLOAT,GL_FALSE,nVertexSize,(GLvoid*)vDecl[i].offset);
    }
    Q_ASSERT(!checkError());
    return pVertexArray;
}

void RDRenderDevice::SetVertexBuffer(RDVertexBufferHandle pVertex)
{
    QMutexLocker locker(&m_lock);
    glBindVertexArray(pVertex->hVertexArray);
    Q_ASSERT(!checkError());
}

void RDRenderDevice::SetShader(RDShaderProgram *pShader)
{
    QMutexLocker locker(&m_lock);
    pShader->use();
    Q_ASSERT(!checkError());
}

bool RDRenderDevice::checkError()
{
    GLenum rt = glGetError();
    if(rt != GL_NO_ERROR)
        qDebug() << hex << rt;
    return rt != GL_NO_ERROR;
}

void RDRenderDevice::SetShaderSample(RDTexture*  tex, RDSampleType nType)
{
    QMutexLocker locker(&m_lock);
    tex->SetTextureSample(nType);
}

void RDRenderDevice::Render(GLenum mode, GLint nStart, GLsizei count)
{
    QMutexLocker locker(&m_lock);
    //SetShaderToDevice();
    glDrawArrays(mode,nStart,count);
    Q_ASSERT(!checkError());
}

bool RDRenderDevice::SetRenderTarget(RDTexture*  target, RDTexture*  depth)
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

void RDRenderDevice::ClearScreen(float4 vColor, float vDepth, RDClearTypes types)
{
    GLbitfield field = 0;
    if(types.testFlag(RDClearColor))
    {
        field |= GL_COLOR_BUFFER_BIT;
        glClearColor(vColor.x(),vColor.y(),vColor.z(),vColor.w());
    }
    if(types.testFlag(RDClearDepth))
    {
        field |= GL_DEPTH_BUFFER_BIT;
        glClearDepthf(vDepth);
    }
    if(types.testFlag(RDClearStencil))
    {
        field |= GL_STENCIL_BUFFER_BIT;
        glClearStencil(0);
    }
    glClear(field);
}

void RDRenderDevice::DumpTexture(RDTexture*  pTex)
{
    QMutexLocker locker(&m_lock);
    RDTexture* pTexture = (RDTexture*)pTex;
    pTexture->Dump("/home/hjc/test.png");
}

RDRenderDevice::RDRenderDevice(const QGLContext* renderContex)
    :m_pDefaultContext(renderContex)
    ,m_lock(QMutex::Recursive)
{
    memset(m_pShader,0,sizeof(RDShader*) * ShaderTypeCount);
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    qDebug() << "OpenGL version "<<major<<"."<<minor;

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&m_nMaxUseTexture);
//    m_nMaxUseTexture = 32;

    m_pCurState = new RDRenderState;
    m_pTempState = new RDRenderState;
    //glGenFramebuffers(1,&m_hFrameBuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER,m_hFrameBuffer);
    glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LINE_SMOOTH);

    m_pVertexBufferDecl = new RDVertexBufferDecl[RDVertexBufferTypeCount];
    m_pVertexBufferDecl[RDVB_Pos_Color].setType(std::vector<RDVertexType>{RDVB_Pos,RDVB_Color});
    m_pVertexBufferDecl[RDVB_Pos_Normal_Texcoord].setType(std::vector<RDVertexType>{RDVB_Pos,RDVB_Normal,RDVB_Texcoord});

}

RDShader * RDRenderDevice::GetExistShader(const QString &shaderName)
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

void    RDRenderDevice::ReleaseVertexBuffer(RDVertexBufferHandle hVertexBuffer)
{
    if(hVertexBuffer == nullptr)
        return;
    QMutexLocker locker(&m_lock);    
    glDeleteBuffers(1,&hVertexBuffer->hVertexBuffer);
    glDeleteVertexArrays(1,&hVertexBuffer->hVertexArray);
}

void    RDRenderDevice::ReleaseTexture(RDTexture*  hTex)
{
    QString strFile = hTex->GetFileName();
    bool bRelease = hTex->Release();
    if(bRelease)
        m_vecFileTex.erase(strFile);
}

int    RDRenderDevice::GetTextureWidth(const RDTexture*  hTex)
{
    return hTex->GetWidth();
}

int    RDRenderDevice::GetTextureHeight(const RDTexture*  hTex)
{
    return hTex->GetHeight();
}

void    RDRenderDevice::ReleaseShader(RDShader* hShader)
{
    if(!hShader)
        return;
     QString strShaderName = hShader->ShaderName();   
     bool bRelease = hShader->Release();
     if(bRelease)
         m_vecShader.erase(strShaderName);
}

RDUBO* RDRenderDevice::CreateUniformBufferObject(int nCount,const float* pData)
{
    QMutexLocker locker(&m_lock);
    RDUBO* pBuffer = new RDUBO;
    glGenBuffers(1,&pBuffer->hUbo);
    glBindBuffer( GL_UNIFORM_BUFFER,pBuffer->hUbo);
    pBuffer->nBufferSize = nCount * sizeof(float);
    glBufferData(GL_UNIFORM_BUFFER,pBuffer->nBufferSize,pData,GL_DYNAMIC_DRAW);
    return pBuffer;
}

void    RDRenderDevice::ReleaseUniformBufferObject(RDUBO* pBuffer)
{
    glDeleteBuffers(1,&pBuffer->hUbo);
    SAFE_DELETE(pBuffer);
}

void    RDRenderDevice::SetShaderParam(int nIndex,RDUBO* pBuffer)
{
    glBindBufferBase( GL_UNIFORM_BUFFER, nIndex, pBuffer->hUbo);
    Q_ASSERT(!checkError());
}

void RDRenderDevice::SetShader(RDShader *pVertexShader, RDShader *pGeometryShader, RDShader *pFragmentShader)
{
    QMutexLocker locker(&m_lock);
    m_pShader[VertexShader] = pVertexShader;
    m_pShader[GeometryShader] = pGeometryShader;
    m_pShader[FragmentShader] = pFragmentShader;
    SetShaderToDevice();
}

void    RDRenderDevice::ModifyUniformBufferObject(RDUBO* pBuffer, const float* pData)
{
    glBindBuffer( GL_UNIFORM_BUFFER,pBuffer->hUbo);
    glBufferSubData(GL_UNIFORM_BUFFER,0,pBuffer->nBufferSize,pData);
}

//void    RDRenderDevice::SetShader(RDShader* pShader,RDShaderType nType)
//{
//    QMutexLocker locker(&m_lock);
//    m_pShader[nType] = pShader;
//}

void    RDRenderDevice::SetShaderToDevice()
{
    QMutexLocker locker(&m_lock);
    RDShaderProgram* pParam = CreateShaderProgram(m_pShader[VertexShader],m_pShader[GeometryShader],m_pShader[FragmentShader]);
    SetShader(pParam);
}

void    RDRenderDevice::SetShaderTexture(int nIndex,const RDTexture* tex)
{
    QMutexLocker locker(&m_lock);
    tex->SetTexture(nIndex);
    Q_ASSERT(!checkError());
}
