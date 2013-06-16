/*
 * =====================================================================================
 *
 *       Filename:  RDModelResource.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年06月07日 22时27分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  RDMODELRESOURCE_INC
#define  RDMODELRESOURCE_INC

#include "RDResource.h"
#include <QString>

class RDModel;

enum RDModelResourceType
{
    RDInternalModel,
    RDImportModel,
};

class RDModelResource: public RDResource
{
public:
    RDModelResource(const RDMd5& md5Id);    //for internal model
    //RDModelResource(const QString& imagePath,const RDMd5& md5Id){};    //for import model
    ~RDModelResource(){};
    const RDModel* GetModel()const{return m_pModel;};
protected:
    QString             m_strPath;
    RDModelResourceType m_nType;
    RDModel*            m_pModel;
};
#endif   /* ----- #ifndef RDModelResource_INC  ----- */
