// =====================================================================================
// 
//       Filename:  RDMd5.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/29/2011 04:41:11 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  :
// 
// =====================================================================================
#include "RDMd5.h"
//#include <openssl/md5.h>
#include <stdio.h>
#include <QCryptographicHash>


RDMd5::RDMd5()
{
    memset(md5Value,0,16 * sizeof(char));
}
RDMd5::RDMd5(const char* pString,int nLen)
{
    if(nLen < 0)
        nLen = strlen(pString);
//    MD5((unsigned char*)pString,nLen,md5Value);
    QByteArray tmp = QCryptographicHash::hash(QByteArray(pString,nLen),QCryptographicHash::Md5);
    memcpy(md5Value,tmp.constData(), 16 * sizeof(char));
}

char charToHex(char v)
{
    v = toupper(v);
    if(v >='A')
        return 10+v-'A';
    else
        return v - '0';
}

RDMd5::RDMd5(const std::string &value)
{
    auto strIt = value.begin();
    for(int j = 0;j < 16;j++)
    {
        md5Value[j] = (charToHex(*strIt) << 4);
        strIt++;
        md5Value[j] |= charToHex(*strIt);
        strIt++;
    }
}

bool RDMd5::operator < (const RDMd5& right)const
{
    for(int i = 0; i < 16; i++)
    {
        if(md5Value[i] == right.md5Value[i])
            continue;
        return md5Value[i] < right.md5Value[i];
    }
    return false;
}

std::string RDMd5::GetMd5String()const
{
    static char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    std::string md5Str;
    char value[3]={0};
    for(int j = 0;j < 16;j++)
    {
        value[1] = hex[md5Value[j]%16];
        value[0] = hex[md5Value[j] >> 4];
        md5Str += value;
    }
    return md5Str;
}
RDMd5::RDMd5(const RDMd5& pMd5)
{
    memcpy(md5Value,pMd5.md5Value, 16 * sizeof(char));
}
