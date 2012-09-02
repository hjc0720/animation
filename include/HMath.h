/*
 * =====================================================================================
 *
 *       Filename:  HMath.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年01月15日 19时07分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  黄建超 (), hjc0720@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  HMATH_INC
#define  HMATH_INC

#include <cmath>
#include <sys/time.h>
#include <unistd.h>

#define FLOAT0 5e-6

inline float min(float a,float b)
{
    return a < b ? a : b;
}

inline float max(float a,float b)
{
    return a > b ? a : b;
}

inline bool floatIsEqualNear(float a,float b)
{
    return fabs(min(a,b) / max(a,b) - 1) < FLOAT0;
}

inline bool floatIsZero(float a)
{
    return fabs(a) < FLOAT0;
}
inline int floatToInt(float f)
{
    return (int)(f + 0.5);
}

//out time is ms
inline double GetTime() 
{
    static struct timezone zone = {0,0};
    static struct timeval time;
    gettimeofday(&time,&zone);
    return time.tv_sec * 1000.0 + time.tv_usec / 1000.0;
}
#endif   // ----- #ifndef hmath_h_INC  -----
