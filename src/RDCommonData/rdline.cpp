/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "rdline.h"
#include <QRectF>

RDLine::RDLine(float fWidth)
{
    m_bChange = true;
    m_hVertex = nullptr;
    setWidth(fWidth);
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    m_hVertexShader = pDevice->CreateShader(":/shader/line_vs",VertexShader);
    m_hPixelShader = pDevice->CreateShader(":/shader/line_ps",FragmentShader);
    m_hParam = nullptr;
}

RDLine::~RDLine()
{
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->ReleaseVertexBuffer(m_hVertex);
    pDevice->ReleaseUniformBufferObject(m_hParam);
    pDevice->ReleaseShader(m_hVertexShader);
    pDevice->ReleaseShader(m_hPixelShader);
}

void RDLine::drawLine(const matrix4x4& WVP)
{
    updatePt();
    if(m_vPt.size() == 0)
        return;
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();

    if(m_hParam)
        pDevice->ModifyUniformBufferObject(m_hParam,reinterpret_cast<const float*>(&WVP));
    else
        m_hParam = pDevice->CreateUniformBufferObject(sizeof(matrix4x4) / sizeof(float),reinterpret_cast<const float*>(&WVP));

    pDevice->SetShader(m_hVertexShader,nullptr,m_hPixelShader);
    pDevice->SetShaderParam(0,m_hParam);

    pDevice->SetVertexBuffer(m_hVertex);
    glLineWidth(m_fWidth);
    pDevice->Render(GL_LINES,0,m_vPt.size() );
}

void RDLine::updatePt()
{
    if(!m_bChange)
        return;
    if(m_hVertex)
    {
        RDRenderDevice::GetRenderManager()->ReleaseVertexBuffer(m_hVertex);
        m_hVertex = nullptr;
    }

    m_bChange = false;
    if(m_vPt.size() == 0)
        return;

    m_hVertex = RDRenderDevice::GetRenderManager()->CreateVertexBuffer(reinterpret_cast<float*>(&m_vPt[0]),m_vPt.size(),sizeof(LinePoint),RDVB_Pos_Color);
}

void RDLine::moveTo(const float3 &vPt)
{
    m_startPt = LinePoint{vPt,m_vColor};
}

void RDLine::lineTo(const float3 &vPt)
{
    m_vPt.push_back(m_startPt);
    m_startPt = LinePoint{vPt,m_vColor};
    m_vPt.push_back(m_startPt);
}

void RDLine::addRect(const QRectF& rect)
{
    float3 vPt;
    vPt.Set(rect.left(),rect.top(),0.1);
    m_vPt.push_back(LinePoint{vPt,m_vColor});

    vPt.Set(rect.right(),rect.top(),0.1);
    m_vPt.push_back(LinePoint{vPt,m_vColor});
    m_vPt.push_back(LinePoint{vPt,m_vColor});

    vPt.Set(rect.right(),rect.bottom(),0.1);
    m_vPt.push_back(LinePoint{vPt,m_vColor});
    m_vPt.push_back(LinePoint{vPt,m_vColor});

    vPt.Set(rect.left(),rect.bottom(),0.1);
    m_vPt.push_back(LinePoint{vPt,m_vColor});
    m_vPt.push_back(LinePoint{vPt,m_vColor});

    vPt.Set(rect.left(),rect.top(),0.1);
    m_vPt.push_back(LinePoint{vPt,m_vColor});
}

