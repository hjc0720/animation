#ifndef RDRENDERMANAGERGL_H
#define RDRENDERMANAGERGL_H
# include <GL/gl.h>
#include <list>
#include <map>
#include <QString>
#include <QFlags>
#include <vector>
#include <QMutex>
#include <HVector4f.h>
#include <HMatrixQ4F.h>

enum RDShaderType
{
    VertexShader,
    FragmentShader,
    GeometryShader,
};

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

class QRect;
class QGLContext;
class RDRenderState;
class RDTexture;
class QGLFormat;
class RDVertexArray;
class RDShader;
class RDShaderProgram;

//typedef RDTexture* RDTexHandle ;
typedef RDVertexArray* RDVertexBufferHandle;
#define InvalidHandle nullptr

class RDRenderDevice
{
public:
    static RDRenderDevice* CreateRenderManager(const QGLContext *renderContex);
    static RDRenderDevice* GetRenderManager();
public:
    //create function
    RDTexture* CreateTexture(const QString& fileName);
    RDTexture* CreateTexture(int nWidth, int nHeight,const uint* buffer,  RDTexture_Type nType);
    RDShader* CreateShader(const QString &fileName, RDShaderType nType);
    RDShader* CreateShader(const QString& code,const QString& shaderName,RDShaderType nType);
    RDShaderProgram* CreateShaderProgram(RDShader *pVertexShader,  RDShader*pGeometryShader,  RDShader* pPixelShader);
    RDVertexBufferHandle     CreateVertexBuffer(const std::vector<RDVertexData> &arVertexData);
    //release function
    void    ReleaseVertexBuffer(RDVertexBufferHandle hVertexBuffer);
    void    ReleaseTexture(RDTexture* hTex);
    void    ReleaseShader(RDShader* hShader);

    //modify function
    //render function
    void    SetVertexBuffer(RDVertexBufferHandle pVertex);
    void    SetShader(RDShaderProgram* pShader);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,float value);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,float3& value);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,float4& value);
    void    SetShaderParam(RDShaderProgram* pShader,const char* name,HMatrixQ4F& value);
    void    SetShaderTexture(RDShaderProgram* pShader,const char* name,const RDTexture* tex);
    void    SetShaderSample(RDTexture* tex,RDSampleType nType);
    void    Render(GLenum mode,GLint nStart,GLsizei count);

    bool    SetRenderTarget(RDTexture* target,RDTexture* depth);
    void    SetViewPort(QRect& viewPort );
    void    SetScissor(QRect& scissor );

    //render info
    int    GetTextureWidth(RDTexture* hTex);
    int    GetTextureHeight(RDTexture*  hTex);
public:
    void DumpTexture(RDTexture*  pTex);
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

//    std::list<RDTexHandle> m_vecTex;
    std::list<RDVertexBufferHandle> m_vecVertexBuffer;
    std::map<QString,RDTexture* > m_vecFileTex;
    std::map<QString,RDShader*> m_vecShader;
    std::map<QString,RDShaderProgram*> m_vecShaderProgram;
    GLuint                 m_hFrameBuffer;

    const QGLContext*     m_pDefaultContext;

    mutable QMutex m_lock;
    //info
    GLint           m_nMaxUseTexure;
};

#endif // RDRENDERMANAGER_H
