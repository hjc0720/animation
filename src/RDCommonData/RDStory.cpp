// =====================================================================================
// 
//       Filename:  RDStory.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/08/11 21:48:26
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDStory.h"
#include "RDFileDataStream.h"
#include <QDebug>

RDStory::RDStory()
    :m_Id(QUuid::createUuid())
{
    m_bPass = false; 
    m_nPlayStartFrame = 0;
    m_nEditStartFrame = 0;
	m_nStoryLength = 1000000000;
}

void RDStory::SetStartTime(const RDTime& nTime,bool bPlay)
{
    if(bPlay)
        m_nPlayStartFrame = nTime;
    else
    {
        qDebug() << "set edit story start Frame" << nTime;
        m_nEditStartFrame = nTime;
    }
}

RDFileDataStream& operator << (RDFileDataStream& buffer,const RDStory& story)
{
    buffer << story.m_bPass;
    buffer << story.m_nEditStartFrame;
    buffer << story.m_nPlayStartFrame;
    buffer << story.m_nStoryLength;
    buffer << story.m_Id;
    return buffer;
}

RDFileDataStream& operator >> (RDFileDataStream& buffer,RDStory& story)
{
    buffer >> story.m_bPass;
    buffer >> story.m_nEditStartFrame;
    buffer >> story.m_nPlayStartFrame;
    buffer >> story.m_nStoryLength;
    buffer >> story.m_Id;
    return buffer;
}
