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

RDImageObject::RDImageObject()
    :RDObject(RDObjectImage)
{
}
RDImageObject::RDImageObject(const QString& fileName)
    :RDObject(RDObjectImage)
{
    RDResourceManager* pResManger = RDResourceManager::GetResourceManager();
    m_Image = pResManger->AddResource(fileName,RDResource_Image)->GetMd5();
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
    if(RenderData.GetRenderChangeLevel() >= RDRender_GraphicChange)
    {
        QRectF dst(0,0,RenderData.GetBound().width() * RenderData.GetScale(),RenderData.GetBound().height() * RenderData.GetScale());
        RenderData.m_RenderBuffer.ResizeBuffer(dst.width() ,dst.height());
        RDResourceManager* pResManager = RDResourceManager::GetResourceManager();
        RDImageResource* pResource = dynamic_cast<RDImageResource*>(pResManager->GetResource(m_Image));
        const RDBuffer* pImage = pResource->GetBuffer();
        QRectF src(0,0,pImage->GetWidth(),pImage->GetHeight());
        RenderData.m_RenderBuffer.Draw(dst,*pImage,src);
        qDebug() << "image render";
    }
}
void RDImageObject::CalFrame(const RDTime& ,RDRenderData& ) 
{
}

bool RDImageObject::HitTest(const float3& vScenePt,const RDNode& pNode,const QString& RDName) const
{
    float3 vBufferPt;
	const RDRenderData* pRenderData = pNode.GetRenderData(RDName);
    RDSceneToBuffer(vBufferPt,vScenePt,pRenderData->GetPos().GetX(),pRenderData->GetPos().GetY());
    //qDebug() << "Scene  pt in image hit test" << vScenePt.GetX() << vScenePt.GetY() << vScenePt.GetZ();
    //qDebug() << "buffer  pt in image hit test" << vBufferPt.GetX() << vBufferPt.GetY() << vBufferPt.GetZ();
    if(vBufferPt.GetX() < 0 || vBufferPt.GetX() >= m_nWidth ||
            vBufferPt.GetY() < 0 || vBufferPt.GetY() >= m_nHeight)
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
    RenderData.SetBound( QRectF(bufferPos.GetX(),bufferPos.GetY(),m_nWidth,m_nHeight));
}
