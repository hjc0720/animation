// =====================================================================================
// 
//       Filename:  RDSection.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  31/07/11 22:12:29
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDSECTION_INC
#define  RDSECTION_INC
#include "mac_define.h"
#include "RDKeyList.h"
#include "HVector4f.h"
#include "HVector3f.h"

enum RDSectionOutType
{
    RDSectionFinish,
    RDSectionKeep,
    RDSecionCycle,
};

enum RDSectionKeyType
{
	RDSectionPos,
	RDSectionAngle,
	RDSectionScale,
	RDSectionCount,
};

class RDJsonDataStream; 
class RDSection
{
public:
    RDSection();
    RDSection(const RDTime& nStartFrame,const RDTime& nLength);
    const RDTime& GetStartTime()const{return m_nStartTime;}
    const RDTime& GetLength()const{return m_nLength;}
    RDTime GetEndTime()const{return m_nStartTime + m_nLength;}
    RDSectionOutType GetType()const{return m_nType;}

    void MovSection(const RDTime& nSteps){m_nStartTime += nSteps;m_nStartTime = std::max<RDTime>(m_nStartTime,0);}

    void AddPosKey(const RDTime& nStoryTime,const float3& value);
    void AddAngleKey(const RDTime& nStoryTime,const float3& value);
    void AddScaleKey(const RDTime& nStoryTime,const float3& value);

    void delPosKey(const RDTime& nSectionTime);
    void delAngleKey(const RDTime& nSectionTime);
    void delScaleKey(const RDTime& nSectionTime);

    void delKey(RDTime nSectionTime);
    void moveKey(RDTime nSrcTime,RDTime nDstTime);

    float3 GetPosVector(RDTime nSectionTime);
    float3 GetAngleVector(RDTime nSectionTime);
    float3 GetScaleVector(RDTime nSectionTime);

    std::set<RDTime> getKeyTimeSet()const;

    void Serialize(RDJsonDataStream& buffer, Json::Value &parent, bool bSave);
protected:
    void delKey(RDTime nSectionTime,RDSectionKeyType type);
    RDKeyList<float3>& getKeyList(RDSectionKeyType eType) ;
    const RDKeyList<float3>& getKeyList(RDSectionKeyType eType) const;
protected:
    RDKeyList<float3> m_PosKey;
    RDKeyList<float3> m_RotateKey;
    RDKeyList<float3> m_ScaleKey;
    //RDKeyList<float3> m_CenterKey;
    RDTime   m_nStartTime; //relative to story time;
    RDTime   m_nLength;
    RDSectionOutType m_nType;
};
#endif   // ----- #ifndef rdsection_INC  -----
