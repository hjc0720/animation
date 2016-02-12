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
#include "rdthreadpool.h"

using namespace std;

RDJsonDataStream::RDJsonDataStream (fstream& in , const std::string& strResourcePath, bool bSave)
    :m_bSave(bSave)
    ,m_file(in)
    ,m_strResourcePath(strResourcePath)
{
    if(!bSave)
    {
        in.seekg(0,ios::end);
        size_t size = in.tellg();
        in.seekg(0,ios::beg);
        if(in.is_open()&& size > 0 )
        {
            m_task = RDThreadPool::AddTask([&]()->bool{
                                               Json::Reader reader;
                                               return reader.parse(in,m_jsonRoot,false);
                                           });
        }
    }
}

RDJsonDataStream::~RDJsonDataStream()
{
    close();
}
RDJsonDataStream::RDJsonDataStream (fstream& in,bool bSave)
    :RDJsonDataStream(in,"",bSave)
{
}

void RDJsonDataStream::SaveResource(const std::string &filePath)
{
    m_ResourceList.insert(filePath);
}

void RDJsonDataStream::EndSaveResource()
{
    for (auto str = m_ResourceList.begin(); str != m_ResourceList.end(); ++str)
    {
        qDebug() <<  str->data();
        std::string dstFilePath(m_strResourcePath + str->substr(str->rfind('/',str->rfind('/') - 1)));

        if(!QFile::exists(dstFilePath.c_str()))
        {
            qDebug() << "copy file "<< *str  << "to" << dstFilePath ;
            if(!QFile::copy(str->data(),dstFilePath.data()))
                qDebug() << dstFilePath;
        }
         //m_ResourceList[i];
    }
}

void RDJsonDataStream::close()
{
    if(m_bSave)
    {
        Json::StyledStreamWriter write;
        write.write(m_file,m_jsonRoot);
        m_file.flush();
        m_bSave = false;
    }
}

// =====================================================================================
