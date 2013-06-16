// =====================================================================================
// 
//       Filename:  RDImageResource.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  21/05/11 11:45:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDImageResource.h"
#include "mac_define.h"
#include <QDebug>
#include "RDBuffer.h"
#include "RDTexture.h"

RDImageResource::RDImageResource(const QString& imagePath,const RDMd5& md5Id)
    :RDResource(md5Id)
     ,m_ImagePath(imagePath)
{
    m_pImage = new RDTexture(imagePath);
}
RDImageResource::~RDImageResource()
{
    m_pImage->Release();
    m_pImage = nullptr;
}
