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
#include <string>

class RDObject;
class RDNode;
class QString;

typedef RDObject* (*pCreateObj)();
typedef RDNode* (*pCreateNode)();

void RegisterObj(const std::string& pStr,pCreateObj pFun);
void UnRegisterObj(const std::string& pStr);
void RegisterNode(const std::string& pStr,pCreateNode pFun);
void UnRegisterNode(const std::string& pStr);

RDObject* CreateObj(const std::string &strType);
RDNode* CreateNode(const std::string& strType);

template<typename T,bool bObj>
class RDObjectCreator
{
public:
    RDObjectCreator(){
        if(bObj)
            RegisterObj(typeid(T).name(),[]()->RDObject*{return (RDObject*)new T;});
        else
            RegisterNode(typeid(T).name(),[]()->RDNode*{return (RDNode*)new T;});
    }
    ~RDObjectCreator(){
        if(bObj)
            UnRegisterObj(typeid(T).name());
        else
            UnRegisterNode(typeid(T).name());
    }
};
