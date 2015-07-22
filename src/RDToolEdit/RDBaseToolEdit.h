// =====================================================================================
// 
//       Filename:  RDBaseToolEdit.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  28/06/11 21:09:46
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDBASETOOLEDIT_INC
#define  RDBASETOOLEDIT_INC
#include <QString>
#include <QRectF>

class RDRenderData;
class float3;
class RDNode;
class RDLine;

class RDBaseToolEdit
{
public:
    RDBaseToolEdit(const QString& name);
    virtual ~RDBaseToolEdit();
    virtual bool BeginEdit(RDNode* pFiledNode);
    virtual void EndEdit();
    const QString& GetName(){return m_ToolEditName;}

// =====================================================================================
// event process
    virtual bool OnMouseMove(const float3& ,Qt::MouseButtons,const QString&){ return false;}
    virtual bool OnMousePress(const Qt::MouseButtons& ,const float3& ,const QString&) { return false;}
    virtual bool OnMouseRelease(const Qt::MouseButtons& ,const float3& ,const std::string& ) { return false;}
    virtual bool OnKeyPress(int ) { return false;}
    virtual QRectF GetDirtyRect(const std::string& ){return QRectF();}
    virtual void resetDirty(const std::string&){}
    virtual void OnDrawInDepth(const std::string& ){}
    virtual void OnDrawNoDepth(const std::string& ){}
protected:
    void   AddSelectItem(RDNode& selItem);
    void   CancelSelectItem();
    void   AddChild(RDNode& pParent,RDNode& pChild);
    void   MoveItemPos(const float3& vNewPos,RDNode& pNode,bool bUpdateCell = true);
    void   MoveItemAngle(const float3& vAngle,RDNode& pNode,bool bUpdateCell = true);
    void   MoveItemScale(const float3& vScale,RDNode& pNode,bool bUpdateCell = true);
protected:
    RDNode* m_pFieldNode;
    RDLine* m_pLine;
private:
    //only initialize in constructor. no other way to modify
    QString       m_ToolEditName;
};
#endif   // ----- #ifndef rdbasetooledit_INC  -----
