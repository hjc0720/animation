#include "RDRenderDevice.h"
#include "RDTexture.h"
#include <QGLContext>
#include <QDebug>

#define SINGLE_CONTEX

class RDRenderState
{
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
    if(m_vecFileTex.find(fileName) == m_vecFileTex.end())
    {
        RDTexture* pTexture = new RDTexture(fileName);
        m_vecFileTex[fileName] = pTexture;
    }
    return m_vecFileTex[fileName];
}

RDTexHandle RDRenderDevice::CreateTexture(int nWidth, int nHeight, int nType)
{
}

void RDRenderDevice::DumpTexture(RDTexHandle pTex)
{
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
