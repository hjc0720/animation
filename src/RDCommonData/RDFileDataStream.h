// =====================================================================================
// 
//       Filename:  RDFileDataStream.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  22/06/11 20:04:09
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDDATASTREAM_INC
#define  RDDATASTREAM_INC
#include <QDataStream>
#include <QStringList>
#include <QString>

class float3;
class RDFileDataStream : public QDataStream
{
public:
    RDFileDataStream ( QIODevice * d );
    RDFileDataStream ( QIODevice * d ,const QString& strResourcePath);
    virtual	~RDFileDataStream (){};
    void SetResourcePath(const QString& strResourcePath){ m_strResourcePath = strResourcePath;}
    void SaveResource(const QString& filePath);
    void EndSaveResource();
protected:
    QStringList m_ResourceList;
    QString     m_strResourcePath;
};

RDFileDataStream& operator << (RDFileDataStream& ,const float3& );
RDFileDataStream& operator >> (RDFileDataStream& ,float3& );

#endif   // ----- #ifndef rddatastream_INC  -----
