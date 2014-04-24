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
#include "RDFileDataStream.h"
#include <QDir>
#include "HMatrixQ4F.h"

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
            if(!QFile::copy(*str,dstFilePath))
                qDebug() << dstFilePath;
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

#include "RDMd5.h"
RDFileDataStream& operator << (RDFileDataStream& buffer,const RDMd5& md5)
{
	buffer.writeRawData( (char*)&md5,sizeof(RDMd5));
    return buffer;
}

RDFileDataStream& operator >> (RDFileDataStream& buffer,RDMd5& md5)
{
	buffer.readRawData( (char*)&md5,sizeof(RDMd5));
    return buffer;
}

QDebug operator<<(QDebug dbg, const matrix4x4 &mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            dbg.nospace() << mat.Get(i,j) << "\t";
        }
        dbg.nospace() << ";\n";
    }
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const float3 & vec)
{
    for(int i = 0; i < 3; i++)
        dbg.nospace() << vec.GetData()[i] << "\t";
    return dbg.space();
}
