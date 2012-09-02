// =====================================================================================
// 
//       Filename:  RDResource.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/05/11 20:49:49
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include <string.h>
#include "RDResource.h"

RDResource::RDResource(const RDMd5& md5Id)
{
    memcpy(&m_md5Id,&md5Id,16 * sizeof(unsigned char));
    m_nRef = 1;
}
RDResource::~RDResource()
{

}
