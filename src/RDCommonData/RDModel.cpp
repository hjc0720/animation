#include "RDModel.h"
#include <map>
#include <QString>

RDModel::RDModel()
{
}


////////////////////////////////////////////////////////////////////////////////
//function
std::map<QString,RDModel*> g_mapModel;
RDModel* CreateSegmentModel()
{
    return nullptr;
}
