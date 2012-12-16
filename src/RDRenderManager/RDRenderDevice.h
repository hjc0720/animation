#ifndef RDRENDERMANAGERGL_H
#define RDRENDERMANAGERGL_H
# include <GL/gl.h>
#include <list>
#include <map>
#include <QString>

class QGLContext;
class RDRenderState;
class RDTexture;
class QGLFormat;
typedef RDTexture* RDTexHandle ;

class RDRenderDevice
{
public:
    static RDRenderDevice* CreateRenderManager(const QGLContext *renderContex);
    static RDRenderDevice* GetRenderManager();
public:
    RDTexHandle CreateTexture(const QString& fileName);
    RDTexHandle CreateTexture(int nWidth,int nHeight,int nType);
    void DumpTexture(RDTexHandle pTex);
    //info
public:
    GLint GetMaxUseTexture()const{return m_nMaxUseTexure - 1;};
    GLint GetCreateTextureIndex()const{return m_nMaxUseTexure - 1;}
protected:
    RDRenderDevice(const QGLContext *renderContex);
protected:
    RDRenderState* m_pCurState;
    RDRenderState* m_pTempState;

    std::list<RDTexHandle> m_vecTex;
    std::map<QString,RDTexHandle> m_vecFileTex;

    const QGLContext*     m_pDefaultContext;
    //info
    GLint           m_nMaxUseTexure;
};

#endif // RDRENDERMANAGER_H
