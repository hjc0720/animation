// =====================================================================================
// 
//       Filename:  RDImageResource.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  21/05/11 11:45:51
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDIMAGERESOURCE_INC
#define  RDIMAGERESOURCE_INC

#include "RDResource.h"
#include <QString>

class RDTexture;

class RDImageResource : public RDResource
{
public:
    RDImageResource(const std::string& imagePath,const RDMd5& md5Id);
    ~RDImageResource();
    const RDTexture* GetBuffer()const{return m_pImage;}
    const std::string& GetPath()const{return m_ImagePath;}
protected:
    std::string m_ImagePath;
    RDTexture* m_pImage;

};
#endif   // ----- #ifndef rdimageresource_INC  -----
