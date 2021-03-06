// =====================================================================================
// 
//       Filename:  RDResourceManager.cpp
// 
//    Description:  a manager for resoure such as picture and movie
// 
//        Version:  1.0
//        Created:  19/05/11 21:55:17
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDResourceManager.h"
#include <openssl/md5.h>
#include <QFile>
#include <QDataStream>
#include "RDImageResource.h"
#include "mac_define.h"
#include <QMutexLocker>
#include <QDir>
#include <QDebug>
#include "RDModelResource.h"
#include "rdexception.h"
using namespace std;

const char* g_fileDir[] = {
    "Image/",
    "Movie/",
    "Model/"
};


RDResourceManager* RDResourceManager::m_pManager = NULL;


RDResourceManager* RDResourceManager::GetResourceManager()
{
    if(!m_pManager)
    {
        m_pManager = new RDResourceManager();
    }
    return m_pManager;
}

void RDResourceManager::ReleaseResourceManager()
{
    std::string dstFilePath(m_pManager->m_strResDir);
    SAFE_DELETE(m_pManager);
    std::string commond("rm -rf ");
    commond += dstFilePath;
    for(size_t i = 0; i < sizeof(g_fileDir) / sizeof(char*);i++)
    {
        std::string cmdStr(commond + g_fileDir[i]);
        cmdStr += "*";
        callSystem(cmdStr.data());
    }
}


RDResourceManager::RDResourceManager()
    :m_resource(RDMd5Compare)
     ,m_lock(QMutex::Recursive)
{
    //m_strResDir = resDirPath;
}

RDResourceManager::~RDResourceManager()
{
    map<RDMd5*,RDResource*>::iterator it;
    for ( it = m_resource.begin() ; it != m_resource.end(); it++ )
    {
        RDMd5* pMd5 = it->first;
        RDResource* pResoucre = it->second;
        SAFE_DELETE(pMd5);
        SAFE_DELETE(pResoucre);
    }
}

RDResource* RDResourceManager::AddResource(const std::string& resPath,RDResouceType nType)
{
    QFile srcFile(resPath.data());
    if(!srcFile.open(QIODevice::ReadOnly))
        return 0;
    char* fileData = new char[srcFile.size()];
    QDataStream data(&srcFile);
    data.readRawData(fileData,srcFile.size());
    RDMd5* pMd5 = new RDMd5(fileData,srcFile.size());
    SAFE_DELETE_ARRAY(fileData);

    QMutexLocker locker(&m_lock);

    RDResource* pResource = GetResource(*pMd5);
    if(pResource)
    {
        SAFE_DELETE(pMd5);
        pResource->AddRef();
        return pResource;
    }

    std::string dstFilePath(m_strResDir);
    dstFilePath += g_fileDir[nType];
    dstFilePath += pMd5->GetMd5String();
    dstFilePath += resPath.substr(resPath.rfind('.'));
    if(!QFile::exists(dstFilePath.data()))
    {
        if(!srcFile.copy(dstFilePath.data()))
        {
            qDebug() << dstFilePath.data();
            SAFE_DELETE(pMd5);
            return 0;
        }
    }

    RDResource* pNewResource = 0;
    switch(nType)
    {
    case RDResource_Image:
        pNewResource = new RDImageResource(dstFilePath,*pMd5);
        break;
    default:
        return 0;
    }
    m_resource[pMd5] = pNewResource; 
    return pNewResource;
}

RDResource* RDResourceManager::GetResource(const RDMd5& pMd5)
{
    QMutexLocker locker(&m_lock);
    map<RDMd5*,RDResource*>::iterator it;
    RDMd5* pTemp = const_cast<RDMd5*>(&pMd5);
    it = m_resource.find(pTemp );
    if(it != m_resource.end())
    {
        //it->second->AddRef();
        return it->second;
    }
    return NULL;
}

RDResource* RDResourceManager::AddResource(const RDMd5& pMd5)
{
    QMutexLocker locker(&m_lock);
    for(size_t i = 0; i < sizeof(g_fileDir) / sizeof(char*);i++)
    {
        std::string dstFilePath(m_strResDir);
        dstFilePath += g_fileDir[i];
        QDir dir(dstFilePath.data(),QString());
        QStringList names = dir.entryList();
        QStringList findName = names.filter(pMd5.GetMd5String().data());

        qDebug() << dstFilePath.data() << names << findName;
        if(!findName.isEmpty())
        {
            qDebug() << dstFilePath.data() << names << findName;
            RDResouceType nType = static_cast<RDResouceType>(i);
            dstFilePath += findName.at(0).toStdString();
            qDebug() << "success add resource " << pMd5.GetMd5String().data();
            return AddResource(dstFilePath,nType);
        }
    }
    return NULL;
}

bool RDResourceManager::RemoveResource(const RDMd5& pMd5)
{
    QMutexLocker locker(&m_lock);
    RDMd5 data = pMd5;
    map<RDMd5*,RDResource*>::iterator it;
    it = m_resource.find(&data);
    RDMd5* pGetMd5 = const_cast<RDMd5*>(it->first);
    RDResource* pResource = it->second;
    pResource->SubRef();
    if(pResource->GetRef() == 0)
    {
        m_resource.erase(it);
        SAFE_DELETE(pResource);
        SAFE_DELETE(pGetMd5);
    }
    return true;
}

RDResource* RDResourceManager::AddModelResource(const QString& modelName)
{
    QByteArray utf8_str = modelName.toUtf8();
    RDMd5* pMd5 = new RDMd5(utf8_str.data(),utf8_str.size());
    QMutexLocker locker(&m_lock);

    RDResource* pResource = GetResource(*pMd5);
    if(pResource)
    {
        SAFE_DELETE(pMd5);
        pResource->AddRef();
        return pResource;
    }
    RDModelResource* pNewResource = new RDModelResource(*pMd5);
    m_resource[pMd5] = pNewResource; 
    return pNewResource;
}
