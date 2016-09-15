// =====================================================================================
// 
//       Filename:  RDFileDataStream.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  22/06/11 20:04:09
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDDATASTREAM_INC
#define  RDDATASTREAM_INC
#include <QStringList>
#include <QString>
#include "rdthreadpool.h"
#include <iostream>
#include <type_traits>
#include <map>
#include <set>
#include "jsonhelper.h"


class RDJsonDataStream
{
public:
    RDJsonDataStream (std::iostream &d, bool bSave );
    RDJsonDataStream (std::iostream &d , const std::string &strResourcePath, bool bSave);
    virtual	~RDJsonDataStream ();
    void SetResourcePath(const std::string& strResourcePath){ m_strResourcePath = strResourcePath;}
    void SaveResource(const std::string& filePath);
    void EndSaveResource();
    bool IsSave()const{return m_bSave;}
    Json::Value& getRoot(){
        if(m_task)
        {
            m_task->getValue();
            m_task=nullptr;
        }
        return m_jsonRoot;
    }
    template<typename T>
    void Save(Json::Value& parent,T& value,const std::string& key) {
        parent[key]=JsonHelper<T>().toJson(value);
    }
    template<typename T>
    void Load(const Json::Value& parent,T& value,const std::string& key,const T& defaultValue=T())
    {
        const Json::Value& json = parent.get(key,Json::Value());
        if(json.isNull())
            value = defaultValue;
        else
            JsonHelper<T>().fromJson(value,json);
    }

    template<typename T>
    void Serialize(Json::Value& parent,const std::string& key,T& value,const T& defaultValue=T())
    {
        if(m_bSave)
            Save(parent,value,key);
        else
            Load(parent,value,key,defaultValue);
    }

    template<typename InIt,typename OutIt,typename Func>
    void Serialize(Json::Value& parent,const std::string& key,InIt begin,InIt end,OutIt out,Func fun)
    {
        if(m_bSave)
        {
            Json::Value ret(Json::arrayValue);
            for(auto it = begin; it != end; it++)
            {
                Json::Value child;
                fun(*this,child,*it);
                ret.append(child);
            }
            parent[key]=ret;
        }
        else
        {
            Json::Value& value = parent[key];
            if(!value.isArray())
                return;
            for(auto it = value.begin(); it != value.end(); it++)
            {
                typedef typename InIt::value_type T;
                T tmp = T();
                fun(*this,*it,tmp);
                *out = tmp;
                out++;
            }
        }
    }

    template<typename K,typename V,typename Func>
    void Serialize(Json::Value& parent,const std::string& key,std::map<K,V>& v,Func fun)
    {
        if(m_bSave)
        {
            Json::Value ret(Json::arrayValue);
            for(auto it = v.begin(); it != v.end(); it++)
            {
                Json::Value child;
                K key(it->first);
                fun(*this,child,key,it->second);
                ret.append(child);
            }
            parent[key]=ret;
        }
        else
        {
            Json::Value& value = parent[key];
            if(!value.isArray())
                return;
            for(auto it = value.begin(); it != value.end(); it++)
            {
                K key;
                V value;
                fun(*this,*it,key,value);
                v.insert({key,value});
            }
        }
    }

    void close();
protected:
    bool                m_bSave;
    std::iostream&             m_file;
    std::set<std::string> m_ResourceList;
    std::string     m_strResourcePath;
    Json::Value m_jsonRoot;
    std::shared_ptr<ThreadTask<bool>> m_task;
};

template<typename T>
size_t RDSaveData(char*& pBuffer,const T& data)
{
    if(pBuffer)
    {
        memcpy(pBuffer,&data,sizeof(T));
        pBuffer += sizeof(T);
    }
    return sizeof(T);
}

template<typename T>
size_t RDSaveDataArray(char*& pBuffer,const T* data,int nCount)
{
    if(pBuffer)
    {
        memcpy(pBuffer,data,sizeof(T) * nCount);
        pBuffer += sizeof(T) * nCount;
    }
    return sizeof(T) * nCount;
}

template<typename T>
size_t RDLoadBuffer(char*& pBuffer,T& data)
{
    memcpy(&data,pBuffer,sizeof(T));
    pBuffer += sizeof(T);
    return sizeof(T);
}

template<typename T>
size_t RDLoadDataArray(char*& pBuffer,T* data,int nCount)
{
    if(pBuffer)
    {
        memcpy(&data,pBuffer,sizeof(T) * nCount);
        pBuffer += sizeof(T) * nCount;
    }
    return sizeof(T) * nCount;
}

#endif   // ----- #ifndef rddatastream_INC  -----
