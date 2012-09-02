// =====================================================================================
// 
//       Filename:  RDStory.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/08/11 21:48:31
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDSTORY_INC
#define  RDSTORY_INC
#include <QUuid>
#include "mac_define.h"
class RDFileDataStream;

class RDStory
{
public:
    RDStory();
    void SetStartTime(const RDTime& nTime,bool bPlay);
    const RDTime& GetStartTime(bool bPlay)const{return bPlay ? m_nPlayStartFrame : m_nEditStartFrame;};
    const RDTime& GetStoryLength()const{return m_nStoryLength;}
	void SetStoryLength(RDTime& nLength){m_nStoryLength = nLength;}
    const QUuid& GetStoryId()const{return m_Id;}
protected:
    bool    m_bPass;
    RDTime m_nEditStartFrame;
    RDTime m_nPlayStartFrame;
    RDTime m_nStoryLength;
    QUuid   m_Id;
   //friend class
    friend RDFileDataStream& operator << (RDFileDataStream& buffer,const RDStory& proj);
    friend RDFileDataStream& operator >> (RDFileDataStream& buffer,RDStory& proj);
};
//save Load operator
RDFileDataStream& operator << (RDFileDataStream& buffer,const RDStory& proj);
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDStory& proj);
#endif   // ----- #ifndef rdstory_INC  -----
