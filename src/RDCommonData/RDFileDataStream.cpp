// =====================================================================================
// 
//       Filename:  RDFileDataStream.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  22/06/11 20:04:00
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include <QFile>
#include <QDebug>
#include "RDFileDataStream.h"

RDFileDataStream::RDFileDataStream ( QIODevice * d ,const QString& strResourcePath)
    :QDataStream(d)
     ,m_strResourcePath(strResourcePath)
{
}
RDFileDataStream::RDFileDataStream ( QIODevice * d )
    :QDataStream(d)
{

}

void RDFileDataStream::SaveResource(const QString& filePath)
{
    m_ResourceList.append(filePath);
}

void RDFileDataStream::EndSaveResource()
{
    m_ResourceList.removeDuplicates();
    for (QStringList::const_iterator str = m_ResourceList.constBegin(); str != m_ResourceList.constEnd(); ++str)
    {
		qDebug() <<  *str;
        QString dstFilePath(m_strResourcePath);
        dstFilePath += "/";
        dstFilePath += (*str).section('/',-2,-1);
 
        if(!QFile::exists(dstFilePath))
        {
            qDebug() << "copy file "<< *str  << "to" << dstFilePath ;
            QFile srcFile(*str);
            if(!srcFile.open(QIODevice::ReadOnly))
                continue;
            if(!srcFile.copy(dstFilePath))
                qDebug() << dstFilePath;
            srcFile.close();
        }
         //m_ResourceList[i];
    }
}

// =====================================================================================
#include "HVector3f.h"
RDFileDataStream& operator << (RDFileDataStream& buffer,const float3& vec)
{
    buffer << vec.x();
    buffer << vec.y();
    buffer << vec.z();
    return buffer;
}
RDFileDataStream& operator >> (RDFileDataStream& buffer,float3& vec)
{
    float value;
    buffer >> value;
    vec.SetX(value );

    buffer >> value;
    vec.SetY(value );

    buffer >> value;
    vec.SetZ(value );
    return buffer;
}
