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

class RDRenderData;
class float3;
class RDNode;
class RDBaseToolEdit
{
public:
    RDBaseToolEdit(const QString& name);
    virtual bool BeginEdit(RDNode* pFiledNode);
    virtual void EndEdit();
    const QString& GetName(){return m_ToolEditName;}

// =====================================================================================
// event process
    virtual bool OnMouseMove(const float3& ,Qt::MouseButtons,const QString&){ return false;}
    virtual bool OnMousePress(const Qt::MouseButtons& ,const float3& ,const QString&) { return false;}
    virtual bool OnMouseRelease(const Qt::MouseButtons& ,const float3&,const QString& ) { return false;}
    virtual bool OnKeyPress(int ) { return false;}
protected:
    void   AddSelectItem(RDNode& selItem);
    void   CancelSelectItem();
    void   AddChild(RDNode& pParent,RDNode& pChild);
    void   MoveItemPos(const float3& vNewPos,RDNode& pNode,bool bUpdateCell = true);
    void   MoveItemAngle(const float3& vAngle,RDNode& pNode,bool bUpdateCell = true);
    void   MoveItemScale(const float3& vScale,RDNode& pNode,bool bUpdateCell = true);
protected:
    RDNode* m_pFieldNode;
private:
    //only initialize in constructor. no other way to modify
    QString       m_ToolEditName;
};
#endif   // ----- #ifndef rdbasetooledit_INC  -----
