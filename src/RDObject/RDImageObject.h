// =====================================================================================
// 
//       Filename:  RDImageObject.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/06/11 18:45:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RD_IMAGE_OBJECT_INC
#define  RD_IMAGE_OBJECT_INC
#include "RDObject.h"
#include <QString>
#include "RDResource.h"
#include "mac_define.h"

class RDImageObject : public RDObject
{
public:
    RDImageObject();
    RDImageObject(const RDMd5& image);
    RDImageObject(const std::string &fileName);
    ~RDImageObject();
    virtual void CreateRenderData(RDRenderData& );
    virtual void ReleaseRenderData(RDRenderData& ){}

    const RDMd5& GetObjMd5()const ;

    void SetFile(const std::string &fileName, bool bInit = false);
    const std::string &GetFile()const;
    void SetWidth(int nWidth){m_nWidth = nWidth;}
    void SetHeight( int nHeight){m_nHeight = nHeight;}
    int GetWidth()const {return m_nWidth;}
    int GetHeight()const {return m_nHeight;}
    int GetOriginWidth()const;
    int GetOriginHeight()const;
    virtual void Render(unsigned long nTime,RDRenderData& RenderData) ;
    virtual void CalFrame(const RDTime& nTime,RDRenderData& RenderData) ;
    virtual void UpdateBound(const RDTime& nTime,RDRenderData& RenderData) ;

    virtual float HitTest(const float3& vScenePt,const RDNode& pNode,const std::string& RDName) const override;

    virtual void Serialize(RDJsonDataStream& buffer, Json::Value& parent, bool bSave)override;
protected:
    RDMd5 m_Image;
    int m_nWidth;
    int m_nHeight;
};

#endif   // ----- #ifndef rdimageobject_INC  -----
