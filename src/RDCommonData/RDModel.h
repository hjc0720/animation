#ifndef RDMODEL_H
#define RDMODEL_H

#include <vector>
#include "RDRenderDevice.h"

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

class float4;
class RDModel
{
public:
    RDModel();
    RDModel(int nCount);
    ~RDModel();
    
    void UpdateRenderData();
    void DrawSubset(int nSubset);
    void AddSubModel(int nCount);
    void AddSubModel(int nStart,int nCount);
protected:
    int m_nVersion;
    int m_nCount;
    float4* m_vPos;
    float4* m_vNormal;
    RDTexcoord* m_vUV;
    std::vector<RDSubModel> m_arSubModel;

    //render data
    RDVertexBufferHandle m_hVertex;

    //static create function
public:
    static RDModel* CreateSegmentModel();
};

#endif // RDMODEL_H
