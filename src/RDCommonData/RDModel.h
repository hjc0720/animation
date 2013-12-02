#ifndef RDMODEL_H
#define RDMODEL_H

#include <vector>
#include "RDRenderDevice.h"

enum RDModelType
{
    RDSegmentModel,
};

class float3;
struct RDTexcoord
{
    float u;
    float v;
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
	matrix4x4 NormalMat;
	matrix4x4 ModelMat;
};

class RDSpaceParam;
class float4;
class RDModel
{
public:
    RDModel();
    RDModel(int nCount);
    ~RDModel();
    
    void UpdateRenderData();
    void DrawSubset(size_t nSubset)const;
    void AddSubModel(int nCount);
    void AddSubModel(int nStart,int nCount);
    size_t GetSubsetCount()const{return m_arSubModel.size();}
    bool HitTest(float3 &vHitPt, const float3& vMouse, const RDSpaceParam& param) const;
    RDShader* GetVertexShader()const{return m_pVertexShader;}          
protected:
    int m_nCount;
    float4* m_vPos;
    float4* m_vNormal;
    RDTexcoord* m_vUV;
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
