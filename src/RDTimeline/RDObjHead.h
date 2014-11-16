// =====================================================================================
// 
//       Filename:  RDObjHead.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/12/11 19:44:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDOBJHEAD_INC
#define  RDOBJHEAD_INC
#include <QWidget>
#include <QUuid>
#include <string>

class QLabel;
class RDNode;

class RDObjHead :public QWidget
{
    Q_OBJECT
public:
		RDObjHead(RDNode& pNode,QWidget* parent);
		~RDObjHead();
        void SetBackGround(bool bDark){setBackgroundRole(bDark?QPalette::AlternateBase : QPalette::Base);};
        virtual void paintEvent(QPaintEvent * /*e*/);
        void updateVisible();
		bool isEqual(RDNode* pNode)const {return pNode == m_pNode;}
signals:
        void    collapseNode(RDNode& pNode);
protected:
		int CalSpaceCount(const RDNode& pNode);
        void mousePressEvent ( QMouseEvent * event ) override;
protected:
        std::string m_strName;
		QUuid 	m_NodeUuid;
		RDNode* m_pNode;
        int     m_nSpace;
};
#endif   // ----- #ifndef rdobjhead_INC  -----
