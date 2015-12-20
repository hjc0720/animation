/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef RDEXCEPTION_H
#define RDEXCEPTION_H

#include <stdlib.h>
class RDException
{
public:
    RDException();
};

void callSystem(const char* command)
{

    if(system(command) == 0)
        throw RDException();
}

#endif // RDEXCEPTION_H
