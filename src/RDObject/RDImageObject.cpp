// =====================================================================================
// 
//       Filename:  RDImageObject.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/06/11 18:45:38
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDImageObject.h"
#include "RDImageResource.h"
#include "RDResourceManager.h"
#include "RDRenderData.h"
#include "mac_define.h"
#include <QRectF>
#include "RDNode.h"
#include "RDSpaceConvert.h"
#include <QDebug>
#include "RDFileDataStream.h"
#include "HVector4f.h"
#include "RDRenderDevice.h"
#include "RDModel.h"
#include "RDTexture.h"
#include "RDModelResource.h"

class RDModel;
class RDImagePrivateData: public RDRenderPrivateData
{
public:
    RDImagePrivateData(){
        m_pSegModel = nullptr;
        m_pImage = nullptr;
        m_pVertexShader = nullptr;
        m_pPixelShader = nullptr;
        m_pShaderProgram = nullptr;
    }
public:
    RDModelResource*            m_pSegModel;
    const RDTexture*    m_pImage; 
    RDShader*           m_pVertexShader;    
    RDShader*           m_pPixelShader;    
    RDShaderProgram*    m_pShaderProgram;
};

//struct RDUV
//{
    //float u;
    //float v;
//};

//float4 vPos[6];
//RDUV vUV[6];
//bool bInit = false;
//RDVertexBufferHandle    hVertex;
//RDShader* pVS = nullptr;
//RDShader* pPS = nullptr;
//RDShaderProgram* pShaderProgram = nullptr;

const char* vs_code = "#version 400 \nlayout (location = 0) in vec4 pos; \
        layout (location = 3) in vec2 tex; \
        out vec2 oTex; \
        void main(){ oTex = tex; gl_Position = pos;}";
const char* ps_code = "#version 400 \n in vec2 oTex;layout(location = 0)out vec4 FragColor;\n \
        void main(){FragColor = vec4(oTex,1,1);}";
RDImageObject::RDImageObject()
    :RDObject(RDObjectImage)
{
}

RDImageObject::RDImageObject(const QString& fileName)
    :RDObject(RDObjectImage)
{
    RDResourceManager* pResManger = RDResourceManager::GetResourceManager();
    m_Image = pResManger->AddResource(fileName,RDResource_Image)->GetMd5();
    //if(!bInit)
    //{
        //vPos[0].Set(-1,1,0.5,1);
        //vUV[0].u = 0;
        //vUV[0].v = 0;
        //vPos[1].Set(1,1,0.5,1);
        //vUV[1].u = 1;
        //vUV[1].v = 0;
        //vPos[2].Set(1,-1,0.5,1);
        //vUV[2].u = 1;
        //vUV[2].v = 1;
        //vPos[3].Set(-1,1,0.5,1);
        //vUV[3].u = 0;
        //vUV[3].v = 1;
        //vPos[4].Set(1,-1,0.5,1);
        //vUV[4].u = 1;
        //vUV[4].v = 1;
        //vPos[5].Set(-1,-1,0.5,1);
        //vUV[5].u = 0;
        //vUV[5].v = 1;
        //bInit = true;
        //RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
        //std::vector<RDVertexData> vertex;
        //vertex.resize(2);
        //vertex[0].nType = RDVB_Pos;
        //vertex[0].pVertexData = (float*)vPos;
        //vertex[0].nVertexCount = 6 * 4;

        //vertex[1].nType = RDVB_Texcoord;
        //vertex[1].pVertexData = (float*)vUV;
        //vertex[1].nVertexCount = 6 * 2 ;
        //hVertex = pDevice->CreateVertexBuffer(vertex);
        //pVS = pDevice->CreateShader(vs_code,QString("test_vs"),VertexShader);
        //pPS = pDevice->CreateShader(ps_code,QString("test_ps"),FragmentShader);

        //pShaderProgram = pDevice->CreateShaderProgram(pVS,  nullptr,  pPS);
        //bInit = true;
    //}
}

RDImageObject::RDImageObject(const RDMd5& image)
    :RDObject(RDObjectImage)
     ,m_Image(image)
{
    RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
    pResManager->AddResource(m_Image);
    m_nWidth = 0;
    m_nWidth = 0;
}


RDImageObject::~RDImageObject()
{
    RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
    pResManager->RemoveResource(&m_Image);
}


void RDImageObject::Render(unsigned long ,RDRenderData& RenderData) 
{
    RDImagePrivateData* pPrivateData = dynamic_cast<RDImagePrivateData*>( RenderData.GetPrivateData());
    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pDevice->SetShader(pPrivateData->m_pShaderProgram);
    const RDModel* pModel = pPrivateData->m_pSegModel->GetModel();
    for(size_t i = 0; i <  pModel->GetSubsetCount(); i++)
    {
        pDevice->SetShaderTexture(pPrivateData->m_pShaderProgram,"DiffuseTex",pPrivateData->m_pImage);
        pModel->DrawSubset(i);
    }
    qDebug() << "image render";
    return;

    //if(RenderData.GetRenderChangeLevel() >= RDRender_GraphicChange)
    //{
        //QRectF dst(0,0,RenderData.GetBound().width() * RenderData.GetScale(),RenderData.GetBound().height() * RenderData.GetScale());
        //RenderData.m_RenderBuffer.ResizeBuffer(dst.width() ,dst.height());
        //RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
        //RDImageResource* pResource = dynamic_cast<RDImageResource*>(pResManager->GetResource(m_Image));
        //const RDTexture* pImage = pResource->GetBuffer();
        ////QRectF src(0,0,pImage->GetWidth(),pImage->GetHeight());
        ////RenderData.m_RenderBuffer.Draw(dst,*pImage,src);
        //qDebug() << "image render";
    //}
}
void RDImageObject::CalFrame(const RDTime& ,RDRenderData& ) 
{
}

bool RDImageObject::HitTest(const float3& vScenePt,const RDNode& pNode,const QString& RDName) const
{
    float3 vBufferPt;
	const RDRenderData* pRenderData = pNode.GetRenderData(RDName);
    RDSceneToBuffer(vBufferPt,vScenePt,pRenderData->GetPos().x(),pRenderData->GetPos().y());
    //qDebug() << "Scene  pt in image hit test" << vScenePt.GetX() << vScenePt.GetY() << vScenePt.GetZ();
    //qDebug() << "buffer  pt in image hit test" << vBufferPt.GetX() << vBufferPt.GetY() << vBufferPt.GetZ();
    if(vBufferPt.x() < 0 || vBufferPt.x() >= m_nWidth ||
            vBufferPt.y() < 0 || vBufferPt.y() >= m_nHeight)
        return false;
    return true;
}

void RDImageObject::Save(RDFileDataStream& buffer)
{
	RDObject::Save(buffer);
	buffer.writeRawData( (char*)&m_Image,sizeof(RDMd5));
	RDImageResource* pResource = dynamic_cast<RDImageResource*>(RDResourceManager::GetResourceManager()->GetResource(m_Image));
	buffer.SaveResource(pResource->GetPath());
	buffer << m_nWidth;
	buffer << m_nHeight;
}

void RDImageObject::Load(RDFileDataStream& buffer)
{
	qDebug() << "begine load image";
	RDObject::Load(buffer);
	buffer.readRawData( (char*)&m_Image,sizeof(RDMd5));
	buffer >> m_nWidth;
	buffer >> m_nHeight;
    RDResourceManager::GetResourceManager()->AddResource(m_Image);
	qDebug() << "end load image";
}
const RDMd5& RDImageObject::GetObjMd5()const 
{
    static RDMd5 ImageMd5("RDImageObject", -1);
    return ImageMd5;
}
void RDImageObject::UpdateBound(const RDTime& ,RDRenderData& RenderData) 
{
    float3 bufferPos;
    RDSceneToBuffer(bufferPos,RenderData.GetPos() ,-(float)RenderData.GetSceneWidth()/ 2,RenderData.GetSceneHeight()/ 2);
    RenderData.SetBound( QRectF(bufferPos.x(),bufferPos.y(),m_nWidth,m_nHeight));
}

void RDImageObject::CreateRenderData(RDRenderData& pRD)
{
    RDImagePrivateData* pPrivateData = dynamic_cast<RDImagePrivateData*>( pRD.GetPrivateData());
    if(!pPrivateData)
    {
        pPrivateData = new RDImagePrivateData;
        pRD.SetPrivateData(pPrivateData);
    }
    RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
    pPrivateData->m_pSegModel = dynamic_cast<RDModelResource*>(pResManager->AddModelResource("segment"));
    RDImageResource* pResource = dynamic_cast<RDImageResource*>(pResManager->GetResource(m_Image));
    pPrivateData->m_pImage = pResource->GetBuffer();

    RDRenderDevice* pDevice = RDRenderDevice::GetRenderManager();
    pPrivateData->m_pVertexShader = pDevice->CreateShader(":/shader/model_vs",VertexShader);    
    pPrivateData->m_pPixelShader = pDevice->CreateShader(":/shader/main_ps",FragmentShader);    
    pPrivateData->m_pShaderProgram = pDevice->CreateShaderProgram(pPrivateData->m_pVertexShader,nullptr,pPrivateData->m_pPixelShader);
}
