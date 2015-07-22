#include "RDTexture.h"
#include <QImage>
#include <QGLWidget>
#include <mac_define.h>
#include <GL/glext.h>
#include <QDebug>

#define COLOR_TYPE GL_RGBA

void ConvertGLToImage(unsigned int* buffer,int nSize)
{
    for(int i = 0; i < nSize; i++)
    {
        unsigned char* color = (unsigned char*)(buffer + i);
        unsigned char b = color[2];
        color[2] = color[0];
        color[0] = b;
    }
}

RDTexture::RDTexture(int nWidth,int nHeight,const uint* pBuffer,RDTexture_Type nType)
    :m_nWidth(nWidth),
      m_nHeight(nHeight),
      m_nTextureType(nType)
      ,m_nRef(1)
{
    RDRenderDevice* pManager = RDRenderDevice::GetRenderManager();
    if(m_nTextureType == RDNormal2DTexture)
    {
        GLint nIndex = pManager->GetCreateTextureIndex();
        glActiveTexture(GL_TEXTURE0 + nIndex);
        glGenTextures(1,&m_nTexture);
        GLenum target = GetTextureTarget(nType);
        glBindTexture(target , m_nTexture);
        glTexImage2D(target ,0,COLOR_TYPE,m_nWidth,m_nHeight,0,COLOR_TYPE,GL_UNSIGNED_BYTE,pBuffer);
    }
    else if(m_nTextureType == RDDepthTexture)
    {
        glGenRenderbuffers(1,&m_nTexture);
        glBindRenderbuffer(GL_RENDERBUFFER,m_nTexture);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,nWidth,nHeight);
    }
    else if(m_nTextureType == RDDepthStencilTexture)
    {
        glGenRenderbuffers(1,&m_nTexture);
        glBindRenderbuffer(GL_RENDERBUFFER,m_nTexture);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_STENCIL,nWidth,nHeight);
    }
}

RDTexture::RDTexture(const QString &fileName)
    :m_strFileName(fileName)
      ,m_nTextureType(RDReadOnly2DTexture)
      ,m_nRef(1)
{
    QImage image = QGLWidget::convertToGLFormat(QImage(fileName));
    m_nWidth = image.bytesPerLine() / 4;
    m_nHeight = image.height();
    RDRenderDevice* pManager = RDRenderDevice::GetRenderManager();
    GLint nIndex = pManager->GetCreateTextureIndex();
    glActiveTexture(GL_TEXTURE0 + nIndex);
    glGenTextures(1,&m_nTexture);
    GLenum target = GetTextureTarget(m_nTextureType);
    glBindTexture(target , m_nTexture);
    const uchar* data = image.bits();
    glTexImage2D(target ,0,COLOR_TYPE,m_nWidth,m_nHeight,0,COLOR_TYPE,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(target);
    GLenum error = glGetError();
    if(error)
    {
        qDebug() << "create mip map error";
    }
}

void RDTexture::Dump(const QString &fileName)
{
    RDRenderDevice* pManager = RDRenderDevice::GetRenderManager();
    GLint nIndex = pManager->GetCreateTextureIndex();
    glActiveTexture(GL_TEXTURE0 + nIndex);
    GLenum target = GetTextureTarget(m_nTextureType);
    glBindTexture(target , m_nTexture);
    unsigned int* buffer = new unsigned int[m_nWidth * m_nHeight];
    glGetTexImage(target,0,COLOR_TYPE,GL_UNSIGNED_BYTE,buffer);
    ConvertGLToImage(buffer,m_nWidth *m_nHeight);
    QImage img((uchar*)buffer,m_nWidth,m_nHeight,QImage::Format_ARGB32);
    img.mirrored(false,true).save(fileName);
    SAFE_DELETE_ARRAY(buffer);
}

void RDTexture::SetTexture(int loc)const
{
    glActiveTexture(GL_TEXTURE0 + loc);
    Q_ASSERT(!checkError());
    GLenum target = GetTextureTarget(m_nTextureType);
    Q_ASSERT(!checkError());
    glBindTexture(target , m_nTexture);
    Q_ASSERT(!checkError());
    GLint logLen;
    glUniform1i(loc,loc);
    Q_ASSERT(!checkError());
}

void RDTexture::SetTextureSample(RDSampleType nType)
{
    switch(nType)
    {
    case RDSample_Point:
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        break;
    case RDSample_Liner:
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    }
}

bool RDTexture::SetRenderTarget(int nIndex)
{
    if(IsTarget())
        return false;
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0 + nIndex,GetTextureTarget(m_nTextureType),m_nTexture,0);
    return true;
}

bool RDTexture::SetDepth()
{
    if(!IsDepth())
        return false;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GetTextureTarget(m_nTextureType),GL_RENDERBUFFER,m_nTexture);
    return true;
}

GLenum RDTexture::GetTextureTarget(RDTexture_Type nType)const
{
    switch(nType)
    {
    case RDNormal2DTexture:
        return GL_TEXTURE_2D;
    case RDReadOnly2DTexture:
        return GL_TEXTURE_2D;
    case RDDepthTexture:
        return GL_DEPTH_ATTACHMENT;
    case RDDepthStencilTexture:
        return GL_DEPTH_STENCIL_ATTACHMENT;
    }
    return GL_TEXTURE_2D;
}

bool RDTexture::IsDepth()
{
    return m_nTextureType == RDDepthTexture || m_nTextureType == RDDepthStencilTexture;
}

bool RDTexture::IsTarget()
{
    return  m_nTextureType == RDNormal2DTexture;
}

bool RDTexture::Release()
{
    m_nRef--;
    bool ret = (m_nRef == 0);
    if(ret)
        delete this;
    return ret ;
}

RDTexture::~RDTexture()
{
    glDeleteTextures(1,&m_nTexture);
}

bool RDTexture::checkError()const
{
    GLenum rt = glGetError();
    if(rt != GL_NO_ERROR)
        qDebug() << hex << rt;
    return false;//rt != GL_NO_ERROR;
}
