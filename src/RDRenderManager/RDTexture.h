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
    void SetTexture(int loc);
    void SetTextureSample(RDSampleType nType);
    bool SetRenderTarget(int nIndex);
    bool SetDepth();
protected:
    GLenum GetTextureTarget(RDTexture_Type nType);
    bool IsDepth();
    bool IsTarget();
protected:
    GLuint m_nTexture;
    QString m_strFileName;
    int   m_nWidth;
    int   m_nHeight;
    RDTexture_Type  m_nTextureType;
};

#endif // RDTEXTURE_H
