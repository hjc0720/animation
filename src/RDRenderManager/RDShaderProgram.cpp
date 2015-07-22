/*
 * =====================================================================================
 *
 *       Filename:  RDShaderProgram.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/16/13 16:09:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "RDShaderProgram.h"



GLenum GetGLType(RDShaderType nType)
{
    switch(nType)
    {
    case VertexShader:
        return GL_VERTEX_SHADER;
    case FragmentShader:
        return GL_FRAGMENT_SHADER;
    case GeometryShader:
        return GL_GEOMETRY_SHADER;
    default:
        return GL_VERTEX_SHADER;
    }
    return GL_VERTEX_SHADER;
}

RDShader::RDShader(const QString &code, const QString &shaderName, RDShaderType nType)
    :m_nRef(1)
    ,m_shaderName(shaderName)
{
    QByteArray ba = code.toLocal8Bit();
    char* pCode = ba.data();
    const GLchar* shaderCodeArray[] = {pCode};
    m_hShader = glCreateShader(GetGLType(nType));
    glShaderSource(m_hShader,1,shaderCodeArray,nullptr);    
    glCompileShader(m_hShader);

#ifdef _DEBUG
    //qDebug() << code;
    GLint result;
    glGetShaderiv(m_hShader,GL_COMPILE_STATUS,&result);
    if(GL_FALSE == result)
    {
        qDebug() << "shader compile error!\n";
        GLint logLen;
        glGetShaderiv(m_hShader,GL_INFO_LOG_LENGTH,&logLen);
        if(logLen > 0)
        {
            char* log = new char[logLen];
            GLsizei written;
            glGetShaderInfoLog(m_hShader,logLen,&written,log);
            qDebug() << log;
            delete []log;
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////


RDShaderProgram::RDShaderProgram(const QString& shaderName,RDShader* pVertexShader,RDShader* pGeometryShader,RDShader* pFragmentShader)
    :m_ProgramName(shaderName)
{
    m_hShaderProgram = glCreateProgram();
    memset(m_pShader,0,sizeof(m_pShader));
    SetShader(pVertexShader,VertexShader);
    SetShader(pFragmentShader,FragmentShader);
    SetShader(pGeometryShader,GeometryShader);
    link();
}

void RDShaderProgram::SetShader(RDShader* pShader,RDShaderType nType)
{
    if(m_pShader[nType])
        glDetachShader(m_hShaderProgram,pShader->GetGLShader());

    m_pShader[nType] = pShader;
    if(m_pShader[nType])
        glAttachShader(m_hShaderProgram,pShader->GetGLShader());
}

void RDShaderProgram::link()
{
    glLinkProgram(m_hShaderProgram);
#ifdef _DEBUG
    GLint result;
    glGetProgramiv(m_hShaderProgram,GL_LINK_STATUS,&result);
    if(GL_FALSE == result)
    {
        qDebug()<<"shader link error!\n";
        GLint logLen;
        glGetProgramiv(m_hShaderProgram,GL_INFO_LOG_LENGTH,&logLen);
        if(logLen > 0)
        {
            char* log = new char[logLen];
            GLsizei written;
            glGetProgramInfoLog(m_hShaderProgram,logLen,&written,log);
            qDebug()<<log;
            delete []log;
        }
    }
#endif
}
