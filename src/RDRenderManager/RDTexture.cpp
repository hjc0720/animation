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

RDTexture::RDTexture(int nWidth,int nHeight,RDTexture_Type nType)
    :m_nWidth(nWidth),
      m_nHeight(nHeight),
      m_nTextureType(nType)
{
    RDRenderDevice* pManager = RDRenderDevice::GetRenderManager();
    GLint nIndex = pManager->GetCreateTextureIndex();
    glActiveTexture(GL_TEXTURE0 + nIndex);
    glGenTextures(1,&m_nTexture);
    GLenum target = GetTextureTarget(nType);
    glBindTexture(target , m_nTexture);
    glTexImage2D(target ,0,COLOR_TYPE,m_nWidth,m_nHeight,0,COLOR_TYPE,GL_UNSIGNED_BYTE,nullptr);
}

RDTexture::RDTexture(const QString &fileName)
    :m_strFileName(fileName)
      ,m_nTextureType(RDReadOnly2DTexture)
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

GLenum RDTexture::GetTextureTarget(RDTexture_Type nType)
{
    switch(nType)
    {
    case RDNormal2DTexture:
        return GL_TEXTURE_2D;
    case RDReadOnly2DTexture:
        return GL_TEXTURE_2D;
    }
    return GL_TEXTURE_2D;
}
