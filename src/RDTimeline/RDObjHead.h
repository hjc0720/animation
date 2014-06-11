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

class QLabel;
class RDNode;

class RDObjHead :public QWidget
{
    Q_OBJECT
public:
		RDObjHead(RDNode& pNode,QWidget* parent);
        virtual void paintEvent(QPaintEvent * /*e*/);
protected:
		int CalSpaceCount(const RDNode& pNode);
protected:
		QLabel* m_pObjName;
		QUuid 	m_NodeUuid;
		RDNode* m_pNode;
};
#endif   // ----- #ifndef rdobjhead_INC  -----
