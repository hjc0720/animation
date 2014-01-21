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
class RDMd5;
class RDFileDataStream : public QDataStream
{
public:
    RDFileDataStream ( QIODevice * d );
    RDFileDataStream ( QIODevice * d ,const QString& strResourcePath);
    virtual	~RDFileDataStream (){};
    void SetResourcePath(const QString& strResourcePath){ m_strResourcePath = strResourcePath;}
    void SaveResource(const QString& filePath);
    void EndSaveResource();
    template<typename T>
    void Serialize(T& value,bool bSave) {
        if(bSave)
            *this << value;
        else
            *this >> value;
    }
protected:
    QStringList m_ResourceList;
    QString     m_strResourcePath;
};

RDFileDataStream& operator << (RDFileDataStream& ,const float3& );
RDFileDataStream& operator >> (RDFileDataStream& ,float3& );
RDFileDataStream& operator << (RDFileDataStream& ,const RDMd5& );
RDFileDataStream& operator >> (RDFileDataStream& ,RDMd5& );

template<typename T>
size_t RDSaveData(char*& pBuffer,const T& data)
{
    if(pBuffer)
    {
        memcpy(pBuffer,&data,sizeof(T));
        pBuffer += sizeof(T);
    }
    return sizeof(T);
}

template<typename T>
size_t RDSaveDataArray(char*& pBuffer,const T* data,int nCount)
{
    if(pBuffer)
    {
        memcpy(pBuffer,data,sizeof(T) * nCount);
        pBuffer += sizeof(T) * nCount;
    }
    return sizeof(T) * nCount;
}

template<typename T>
size_t RDLoadBuffer(char*& pBuffer,T& data)
{
    memcpy(&data,pBuffer,sizeof(T));
    pBuffer += sizeof(T);
    return sizeof(T);
}

template<typename T>
size_t RDLoadDataArray(char*& pBuffer,T* data,int nCount)
{
    if(pBuffer)
    {
        memcpy(&data,pBuffer,sizeof(T) * nCount);
        pBuffer += sizeof(T) * nCount;
    }
    return sizeof(T) * nCount;
}
#endif   // ----- #ifndef rddatastream_INC  -----
