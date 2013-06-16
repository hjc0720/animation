/*
 * =====================================================================================
 *
 *       Filename:  RDModelResource.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年06月07日 22时26分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "RDModelResource.h"
#include <QString>
#include "HMath.h"
#include "RDModel.h"

RDMd5 strModel[] = 
{
    "segment",   
};

RDModelType nModel[] = 
{
    RDSegmentModel,
};

RDModelResource::RDModelResource(const RDMd5& md5Id)    //for internal model
    :RDResource(md5Id)
     ,m_nType(RDInternalModel)
     ,m_pModel(nullptr)
{
    for(size_t i = 0; i < GetArrayLen(strModel); i++)
    {
        if(strModel[i] == md5Id)
        {
            m_pModel = RDModel::CreateModel(nModel[i]);
        }
    }
}
