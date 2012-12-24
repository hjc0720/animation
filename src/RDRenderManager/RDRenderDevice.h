#ifndef RDRENDERMANAGERGL_H
#define RDRENDERMANAGERGL_H
# include <GL/gl.h>
#include <list>
#include <map>
#include <QString>
#include <QGLShader>
#include <QFlags>
#include <vector>
#include <QMutex>
#include <HVector3f.h>
#include <HVector4f.h>
#include <HMatrixQ4F.h>

enum RDTexture_Type
{
    RDNormal2DTexture,
    RDReadOnly2DTexture,
    RDDepthTexture,
    RDDepthStencilTexture,
};

enum RDVertexBufferType
{
    RDVB_Pos = 0,
    RDVB_Color,
    RDVB_Normal,
    RDVB_Texcoord,
    RDVB_Tang,
    RDVB_Bi
};

enum RDSampleType
{
    RDSample_Point,
    RDSample_Liner,
};

struct RDVertexData
{
    RDVertexBufferType nType;
    float* pVertexData;
    int nVertexCount;
};

class QGLContext;
class RDRenderState;
class RDTexture;
class QGLFormat;
class RDVertexArray;
typedef RDTexture* RDTexHandle ;
typedef RDVertexArray* RDVertexBufferHandle;

struct RDFileTexture;
struct RDShader;
struct RDShaderProgram;

class RDRenderDevice
{
public:
    static RDRenderDevice* CreateRenderManager(const QGLContext *renderContex);
    static RDRenderDevice* GetRenderManager();
public:
    //create function
    RDTexHandle CreateTexture(const QString& fileName);
    RDTexHandle CreateTexture(int nWidth,int nHeight,RDTexture_Type nType);
    RDShader* CreateShader(const QString& fileName,QGLShader::ShaderType nType);
    RDShader* CreateShader(const QString& code,const QString& shaderName,QGLShader::ShaderType nType);
    RDShaderProgram* CreateShaderProgram(RDShader *pVertexShader,  RDShader*pGeometryShader,  RDShader* pPixelShader, std::vector<RDVertexBufferType> VertexShaderType);
    RDVertexBufferHandle     CreateVertexBuffer(const std::vector<RDVertexData> &arVertexData);
    //release function

    //modify function
    //render function
    void    SetVertexBuffer(RDVertexBufferHandle pVertex);
    void    SetShader(RDShaderProgram* pShader);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,float value);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,float3& value);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,float4& value);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,HMatrixQ4F& value);
    void    SetShaderTexture(RDShaderProgram* pShader,const char* name,RDTexHandle tex);
    void    SetShaderSample(RDTexHandle tex,RDSampleType nType);

    bool    SetRenderTarget(RDTexHandle target,RDTexHandle depth);
    void    SetViewPort(QRect& viewPort );
    void    SetScissor(QRect& scissor );

    //render info
public:
    void DumpTexture(RDTexHandle pTex);
    //info
public:
    GLint GetMaxUseTexture()const{return m_nMaxUseTexure - 1;}
    GLint GetCreateTextureIndex()const{return m_nMaxUseTexure - 1;}
protected:
    RDRenderDevice(const QGLContext *renderContex);
    RDShader* GetExistShader(const QString& shaderName);
protected:
    RDRenderState* m_pCurState;
    RDRenderState* m_pTempState;

    std::list<RDTexHandle> m_vecTex;
    std::list<RDVertexBufferHandle> m_vecVertexBuffer;
    std::map<QString,RDFileTexture*> m_vecFileTex;
    std::map<QString,RDShader*> m_vecShader;
    std::map<QString,RDShaderProgram*> m_vecShaderProgram;
    GLuint                 m_hFrameBuffer;

    const QGLContext*     m_pDefaultContext;

    mutable QMutex m_lock;
    //info
    GLint           m_nMaxUseTexure;
};

#endif // RDRENDERMANAGER_H