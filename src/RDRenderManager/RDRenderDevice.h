#ifndef RDRENDERMANAGERGL_H
#define RDRENDERMANAGERGL_H
#include <list>
#include <map>
#include <QString>
#include <QFlags>
#include <vector>
#include <QMutex>
#include <HVector4f.h>
#include <HVector3f.h>
#include <HMatrixQ4F.h>
# include <GL/gl.h>
#include <QtGlobal>

enum RDShaderType
{
    VertexShader,
    FragmentShader,
    GeometryShader,
    ShaderTypeCount,
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
    RDVB_Pos_Color = 0,
    RDVB_Pos_Normal_Texcoord,
    RDVertexBufferTypeCount
};

enum RDSampleType
{
    RDSample_Point,
    RDSample_Liner,
};

enum RDClearType
{
    RDClearColor = 1,
    RDClearDepth = 2,
    RDClearStencil = 4,
};

Q_DECLARE_FLAGS(RDClearTypes, RDClearType)
Q_DECLARE_OPERATORS_FOR_FLAGS(RDClearTypes)


struct RDUBO;
class QRect;
class QGLContext;
class RDRenderState;
class RDTexture;
class QGLFormat;
class RDVertexArray;
class RDShader;
class RDShaderProgram;
class RDVertexBufferDecl;

//typedef RDTexture* RDTexHandle ;
typedef RDVertexArray* RDVertexBufferHandle;
#define InvalidHandle nullptr

class RDRenderDevice
{
public:
    static RDRenderDevice* CreateRenderManager(const QGLContext *renderContex);
    static RDRenderDevice* GetRenderManager();
    static void ReleaseRenderManager();
public:
    //create functionpGer
    RDTexture* CreateTexture(const QString& fileName);
    RDTexture* CreateTexture(int nWidth, int nHeight,const uint* buffer,  RDTexture_Type nType);
    RDShader* CreateShader(const QString &fileName, RDShaderType nType);
    RDShader* CreateShader(const QString& code,const QString& shaderName,RDShaderType nType);
    RDVertexBufferHandle     CreateVertexBuffer(float* pVertexData, int nVertexCount, size_t nVertexSize, RDVertexBufferType eType);
    RDUBO*         CreateUniformBufferObject(int nCount,const float* vBuffer);
    //release function
    void    ReleaseVertexBuffer(RDVertexBufferHandle hVertexBuffer);
    void    ReleaseTexture(RDTexture* hTex);
    void    ReleaseShader(RDShader* hSGeometryhader);
    void    ReleaseUniformBufferObject(RDUBO* pBuffer);
    //modify function
    void    ModifyUniformBufferObject(RDUBO* pBuffer, const float* pData);
    //void    ClearTexture(RDTexture* pTexture, );
    //render function
    void    SetVertexBuffer(RDVertexBufferHandle pVertex);
    void    SetShaderParam(int nIndex,RDUBO* pBuffer);
    void    SetShader(RDShader* pVertexShader,RDShader* pGeometryShader,RDShader* pFragmentShader);
    void    SetShaderTexture(int nIndex,const RDTexture* tex);

    void    SetShaderSample(RDTexture* tex,RDSampleType nType);
    void    Render(GLenum mode,GLint nStart,GLsizei count);
    void    RenderLine();

    bool    SetRenderTarget(RDTexture* target,RDTexture* depth);
    void    SetViewPort(QRect& viewPort );
    void    SetScissor(QRect& scissor );
    void    ClearScreen(float4 vColor,float vDepth,RDClearTypes types);

    //render info
    int    GetTextureWidth(const RDTexture*  hTex);
    int    GetTextureHeight(const RDTexture*  hTex);
public:
    void DumpTexture(RDTexture*  pTex);
    //info
public:
    GLint GetMaxUseTexture()const{return m_nMaxUseTexture - 1;}
    GLint GetCreateTextureIndex()const{return m_nMaxUseTexture - 1;}
protected:
    RDRenderDevice(const QGLContext *renderContex);
    virtual ~RDRenderDevice();
    RDShader* GetExistShader(const QString& shaderName);
    void    SetShaderToDevice();
    RDShaderProgram* CreateShaderProgram(RDShader *pVertexShader,  RDShader*pGeometryShader,  RDShader* pPixelShader);
    void    SetShader(RDShaderProgram* pShader);
    bool    checkError();
protected:
    RDRenderState* m_pCurState;
    RDRenderState* m_pTempState;

//    std::list<RDTexHandle> m_vecTex;
    //std::list<RDVertexBufferHandle> m_vecVertexBuffer;
    std::map<QString,RDTexture* > m_vecFileTex;
    std::map<QString,RDShader*> m_vecShader;
    std::map<QString,RDShaderProgram*> m_vecShaderProgram;
    GLuint                 m_hFrameBuffer;
    RDShader*               m_pShader[ShaderTypeCount];

    const QGLContext*     m_pDefaultContext;

    mutable QMutex m_lock;
    //info
    GLint           m_nMaxUseTexture;
    RDVertexBufferDecl* m_pVertexBufferDecl;
};

#endif // RDRENDERMANAGER_H
