// =====================================================================================
// 
//       Filename:  RDResourceManager.h
// 
//    Description:  a manager for resoure such as picture and movie
// 
//        Version:  1.0
//        Created:  19/05/11 21:55:21
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDRESOURCEMANAGER_INC
#define  RDRESOURCEMANAGER_INC

#include <map>
#include <QString>
#include <QMutex>
#include "RDMd5.h"

enum RDResouceType
{
    RDResource_Image,
    RDResource_Movie,
    RDResource_Buffer,
    RDResource_Model,
};

class RDResource;

class RDResourceManager
{
public:
    static RDResourceManager* GetResourceManager();
    static void ReleaseResourceManager();
    ~RDResourceManager();
    RDResource* AddResource(const QString& resPath,RDResouceType nType);
    RDResource* AddImageResource(const QString& resPath){return AddResource(resPath,RDResource_Image);}
    RDResource* AddModelResource(const QString& modelName);
    RDResource* AddResource(const RDMd5& pMd5);
    RDResource* GetResource(const RDMd5& pMd5);
    bool RemoveResource(const RDMd5& pMd5);
    void SetResPath(const QString& strPath){m_strResDir = strPath;}
    const QString& GetResourcePath()const{return m_strResDir;}
protected:
    RDResourceManager();
protected:
    static RDResourceManager* m_pManager;
    std::map<RDMd5*,RDResource*,RDMd5PtrCompare> m_resource;
    QString m_strResDir;
    mutable QMutex m_lock;
};
#endif   // ----- #ifndef rdresourcemanager_INC  -----
