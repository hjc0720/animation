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
    
    void UpdateRenderData();
protected:
    int m_nVersion;
    int m_nCount;
    float4* m_vPos;
    float4* m_vNormal;
    RDTexcoord* m_vUV;
    std::vector<RDSubModel*> m_arSubModel;

    //render data
    RDVertexBufferHandle m_hVertex;

    //friend function
    friend RDModel* CreateSegmentModel();
};

RDModel* CreateSegmentModel();
#endif // RDMODEL_H
