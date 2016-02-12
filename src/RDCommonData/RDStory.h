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
#include <string>

class RDJsonDataStream;
namespace Json {
class Value;
}

class RDStory
{
public:
    RDStory(const std::string& name);

    void SetStartTime(const RDTime& nTime){m_nPlayStartFrame = nTime;};
    const RDTime& GetStartTime()const{return m_nPlayStartFrame;};

    const RDTime& GetStoryLength()const{return m_nStoryLength;}
	void SetStoryLength(RDTime& nLength){m_nStoryLength = nLength;}

    const QUuid& GetStoryId()const{return m_Id;}
	const std::string& GetStroyName()const{return m_strName;}

    void Serialize(RDJsonDataStream &buffer, Json::Value &parent, bool bSave);
protected:
    bool    m_bPass;
    RDTime m_nPlayStartFrame;
    RDTime m_nStoryLength;
    QUuid   m_Id;
    std::string m_strName;
};
#endif   // ----- #ifndef rdstory_INC  -----
