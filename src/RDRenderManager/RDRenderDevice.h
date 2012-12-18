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

enum RDTexture_Type
{
    RDNormal2DTexture,
    RDReadOnly2DTexture,
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

class RDRenderDevice
{
public:
    static RDRenderDevice* CreateRenderManager(const QGLContext *renderContex);
    static RDRenderDevice* GetRenderManager();
public:
    RDTexHandle CreateTexture(const QString& fileName);
    RDTexHandle CreateTexture(int nWidth,int nHeight,RDTexture_Type nType);
    QGLShader* CreateShader(const QString& fileName,QGLShader::ShaderType nType);
    QGLShader* CreateShader(const QString& code,const QString& shaderName,QGLShader::ShaderType nType);
    RDVertexBufferHandle     CreateVertexBuffer(const std::vector<RDVertexData> &arVertexData);
    //render info
public:
    void DumpTexture(RDTexHandle pTex);
    //info
public:
    GLint GetMaxUseTexture()const{return m_nMaxUseTexure - 1;};
    GLint GetCreateTextureIndex()const{return m_nMaxUseTexure - 1;}
protected:
    RDRenderDevice(const QGLContext *renderContex);
    QGLShader* GetExistShader(const QString& shaderName);
protected:
    RDRenderState* m_pCurState;
    RDRenderState* m_pTempState;

    std::list<RDTexHandle> m_vecTex;
    std::list<RDVertexBufferHandle> m_vecVertexBuffer;
    std::map<QString,RDFileTexture*> m_vecFileTex;
    std::map<QString,RDShader*> m_vecShader;

    const QGLContext*     m_pDefaultContext;

    mutable QMutex m_lock;
    //info
    GLint           m_nMaxUseTexure;
};

#endif // RDRENDERMANAGER_H
