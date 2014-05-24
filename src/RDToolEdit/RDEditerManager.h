// =====================================================================================
// 
//       Filename:  RDEditerManager.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/27/2011 11:53:21 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RD_EDITER_MANAGER_INC
#define  RD_EDITER_MANAGER_INC
#include <QObject>
#include <vector>
#include <map>
#include "RDMd5.h"

class RDRenderData;
class RDNode;
class RDCell;
class RDBaseEdit;
class RDDocument;
class RDEditerManager :public QObject
{
    Q_OBJECT
public:
    static RDEditerManager& GetEditerManager();
    static void ReleaseEditerManager();
    void    UpdateProperty(RDNode* pData);
    void    UpdateCell(const RDMd5* pCell,const RDNode& pData);
	void 	AddNode(RDNode& pNode){emit AddNoded(pNode);}

    void SetDocument(RDDocument* pDoc){m_pCurDoc = pDoc;}
    RDDocument* GetDocument(){return m_pCurDoc;}
signals:
    void 	PropertyChanged(RDCell** pCellArray,int nCount);
	void 	AddNoded(RDNode&);
    void    SceneChange();
public slots:
    void    CellChange(const RDMd5& pCell,int nIndex);
protected:
    RDEditerManager();
    ~RDEditerManager();
    void RegisterEditer();
    void RegisterCell();
protected:
    static RDEditerManager* m_pEditerManager;

    RDNode*           m_pCurNode;
    std::map<RDMd5,RDCell*> m_CellList;
    std::map<RDMd5,RDBaseEdit*> m_EditList;
    RDDocument*  m_pCurDoc;
};
#endif   // ----- #ifndef rdeditermanager_INC  -----
