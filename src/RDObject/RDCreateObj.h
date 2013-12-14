/*
 * =====================================================================================
 *
 *       Filename:  RDCreateObj.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/04/2011 09:06:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <typeinfo>

class RDObject;
class RDNode;
class QString;

typedef RDObject* (*pCreateObj)();
typedef RDNode* (*pCreateNode)();

void RegisterObj(const QString& pStr,pCreateObj pFun);
void UnRegisterObj(const QString& pStr);
void RegisterNode(const QString& pStr,pCreateNode pFun);
void UnRegisterNode(const QString& pStr);

RDObject* CreateObj(const QString& strType);
RDNode* CreateNode(const QString& strType);

template<typename T,bool bObj>
class RDObjectCreator
{
public:
    RDObjectCreator(){
        if(bObj)
            RegisterObj(QString(typeid(T).name()),[]()->RDObject*{return (RDObject*)new T;});
        else
            RegisterNode(QString(typeid(T).name()),[]()->RDNode*{return (RDNode*)new T;});
    }
    ~RDObjectCreator(){
        if(bObj)
            UnRegisterObj(QString(typeid(T).name()));
        else
            UnRegisterNode(QString(typeid(T).name()));
    }
};
