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

class RDFileDataStream; 
class RDSection
{
public:
    RDSection();
    RDSection(const RDTime& nStartFrame,const RDTime& nLength);
    const RDTime& GetStartTime()const{return m_nStartTime;}
    const RDTime& GetLength()const{return m_nLength;}
    RDTime GetEndFrame()const{return m_nStartTime + m_nLength;}
    RDSectionOutType GetType()const{return m_nType;}

    void MovSection(const RDTime& nSteps){m_nStartTime += nSteps;}

    void AddPosKey(const RDTime& nStoryTime,const float3& value);
    void AddAngleKey(const RDTime& nStoryTime,const float3& value);
    void AddScaleKey(const RDTime& nStoryTime,const float3& value);

    float3 GetPosVector(const RDTime& nSectionTime);
    float3 GetAngleVector(const RDTime& nSectionTime);
    float3 GetScaleVector(const RDTime& nSectionTime);
protected:
    RDKeyList<float3> m_PosKey;
    RDKeyList<float3> m_RotateKey;
    RDKeyList<float3> m_ScaleKey;
    //RDKeyList<float3> m_CenterKey;
    RDTime   m_nStartTime; //relative to story time;
    RDTime   m_nLength;
    RDSectionOutType m_nType;
    friend RDFileDataStream& operator << (RDFileDataStream& buffer,const RDSection& proj);
    friend RDFileDataStream& operator >> (RDFileDataStream& buffer,RDSection& proj);
};
RDFileDataStream& operator << (RDFileDataStream& buffer,const RDSection& proj);
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDSection& proj);
#endif   // ----- #ifndef rdsection_INC  -----
