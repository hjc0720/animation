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
#include <QImage>

RDImageResource::RDImageResource(const QString& imagePath,const RDMd5& md5Id)
    :RDResource(md5Id)
     ,m_ImagePath(imagePath)
{
    m_pImage = new RDBuffer(QImage(imagePath).convertToFormat(QImage::Format_ARGB32));
}
RDImageResource::~RDImageResource()
{
    SAFE_DELETE(m_pImage);
}
