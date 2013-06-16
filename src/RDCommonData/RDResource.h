// =====================================================================================
// 
//       Filename:  RDResource.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/05/11 20:50:05
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDRESOURCE_INC
#define  RDRESOURCE_INC
#include <QString>
#include "RDMd5.h"

class RDResource
{
public:
    RDResource(const RDMd5& md5Id);
    RDResource(){};
    virtual ~RDResource();
    void AddRef()const{m_nRef++;}
    void SubRef()const{m_nRef--;}
    int GetRef()const{return m_nRef;}
    const RDMd5& GetMd5(){return m_md5Id;}
protected:
    RDMd5 m_md5Id;
    mutable int          m_nRef;
};

#endif   // ----- #ifndef rdresource_INC  -----
