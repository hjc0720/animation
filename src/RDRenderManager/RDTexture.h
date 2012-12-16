#ifndef RDTEXTURE_H
#define RDTEXTURE_H
# include <GL/gl.h>
#include <QString>
#include "RDRenderDevice.h"

class RDTexture
{
public:
    RDTexture(int nWidth,int nHeight,RDTexture_Type nType);
    RDTexture(const QString& fileName);
    void Dump(const QString& fileName);
protected:
    GLenum GetTextureTarget(RDTexture_Type nType);
protected:
    GLuint m_nTexture;
    QString m_strFileName;
    int   m_nWidth;
    int   m_nHeight;
    RDTexture_Type  m_nTextureType;
};

#endif // RDTEXTURE_H
