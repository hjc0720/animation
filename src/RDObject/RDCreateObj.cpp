/*
 * =====================================================================================
 *
 *       Filename:  RDCreateObj.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/04/2011 09:05:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "RDCreateObj.h"
#include "RDImageObject.h"

RDObject* CreateObj(int nType)
{
	switch(nType)
	{
	case RDObjectImage:
		return new RDImageObject();
	default:
		return NULL;
	}
	return NULL;
}
