#ifndef RDMODEL_H
#define RDMODEL_H

#include <vector>
#include "RDRenderDevice.h"

enum RDModelType
{
    RDSegmentModel,
};

struct RDSubModel
{
    int nStart;
    int nCount;
//    RDMaterial material;
};

struct ModelVSParam
{
	matrix4x4 MVP;
	matrix4x4 ModelMat;
	matrix4x4 NormalMat;
};
struct ModelVertexPt
{
    float4 m_vPos;
    float4 m_vNormal;
    float2 m_vUV;
};

class RDSpaceParam;
class RDMaterial;
class RDModel
{
public:
    RDModel();
    ~RDModel();
    
    void UpdateRenderData();
    void DrawSubset(size_t nSubset, RDMaterial *pMat)const;
    RDShader* GetVertexShader(){return m_pVertexShader;}
    void AddSubModel(int nCount);
    void AddSubModel(int nStart,int nCount);
    size_t GetSubsetCount()const{return m_arSubModel.size();}
    float HitTest(const float3& vMouse, const RDSpaceParam& param) const;
    RDShader* GetVertexShader()const{return m_pVertexShader;}          
protected:
    void pushPt(const ModelVertexPt& pt){m_vVertex.push_back(pt);}
protected:
    std::vector<ModelVertexPt>   m_vVertex;
    std::vector<RDSubModel> m_arSubModel;

    //render data
    RDShader*           m_pVertexShader;    
    RDVertexBufferHandle m_hVertex;

    //static create function
public:
    static RDModel* CreateModel(RDModelType nType);
    static RDModel* CreateSegmentModel();
};

#endif // RDMODEL_H
