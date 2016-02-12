#ifndef RDTEXTURE_H
#define RDTEXTURE_H
# include <GL/gl.h>
#include "RDRenderDevice.h"

class RDTexture
{
public:
    RDTexture(int nWidth,int nHeight,const uint* pBuffer,RDTexture_Type nType);
    RDTexture(const std::string& fileName);
    
    bool Release();
    void Dump(const QString& fileName);
    void SetTexture(int loc) const;
    void SetTextureSample(RDSampleType nType);
    bool SetRenderTarget(int nIndex);
    bool SetDepth();
    void AddRef(){m_nRef++;}
    const std::string& GetFileName()const {return m_strFileName;}
    int     GetWidth()const{return m_nWidth;}
    int     GetHeight()const{return m_nHeight;}
protected:
    ~RDTexture();
    GLenum GetTextureTarget(RDTexture_Type nType)const;
    bool IsDepth();
    bool IsTarget();
    bool checkError()const;
protected:
    std::string m_strFileName;
    int     m_nWidth;
    int     m_nHeight;
    RDTexture_Type  m_nTextureType;

    int     m_nRef;
    GLuint  m_nTexture;
};

#endif // RDTEXTURE_H
