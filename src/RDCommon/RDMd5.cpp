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

QString RDMd5::GetMd5String()const
{
    QString md5Str;
    for(int j = 0;j < 16;j++)
    {
        md5Str += QString("%1").arg(md5Value[j],0,16);
    }
    return md5Str;
}
RDMd5::RDMd5(const RDMd5& pMd5)
{
    memcpy(md5Value,pMd5.md5Value, 16 * sizeof(char));
}
