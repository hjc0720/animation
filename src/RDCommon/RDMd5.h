// =====================================================================================
// 
//       Filename:  RDMd5.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/29/2011 04:41:15 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RD_MD5_INC
#define  RD_MD5_INC
#include <string>

class RDMd5
{
public:
    RDMd5();
    RDMd5(const char* pString,int nLen = -1);
    RDMd5(const std::string& value);
    RDMd5(const RDMd5& pMd5);
    
    std::string GetMd5String()const;
    bool operator < (const RDMd5& left)const;
protected:
    unsigned char md5Value[16];

    friend bool operator == (const RDMd5& src,const RDMd5& dst);
    friend bool operator != (const RDMd5& src,const RDMd5& dst);
};

typedef bool(*RDMd5PtrCompare)(const RDMd5*,const RDMd5*);

inline bool RDMd5Compare(const RDMd5* left,const RDMd5* right)
{
    return *left < *right;
}
inline bool operator == (const RDMd5& src,const RDMd5& dst)
{
    for(int i = 0; i < 16; i++)
    {
        if(src.md5Value[i] != dst.md5Value[i])
            return false;
    }
    return true;
} 
inline bool operator != (const RDMd5& src,const RDMd5& dst)
{
    for(int i = 0; i < 16; i++)
    {
        if(src.md5Value[i] != dst.md5Value[i])
            return true;
    }
    return false;
} 
#endif   // ----- #ifndef rdmd5_INC  -----
