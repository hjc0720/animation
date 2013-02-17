#ifndef RDMODEL_H
#define RDMODEL_H

#include "RDMaterial.h"
#include <vector>

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
    RDMaterial material;
};

class RDModel
{
public:
    RDModel();
    int m_nCount;
    float3* m_vPos;
    float3* m_vNormal;
    RDTexcoord* uv;
    std::vector<RDSubModel*> m_arSubModel;
};

RDModel* CreateSegmentModel();
#endif // RDMODEL_H
