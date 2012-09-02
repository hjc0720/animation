// =====================================================================================
// 
//       Filename:  mac_define.h
// 
//    Description:  some easy moc define
// 
//        Version:  1.0
//        Created:  26/03/11 14:14:33
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
//
#ifndef  MAC_DEFINE_INC
#define  MAC_DEFINE_INC

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p) delete p;p=0;}
#endif


#ifndef  SAFE_DELETE_ARRAY
#define  SAFE_DELETE_ARRAY(p) {if(p) delete[]p;p=0;}
#endif   // ----- #ifndef safe_delete_array_INC  -----

typedef unsigned long long RDTime;
typedef unsigned int uint;
#endif   // ----- #ifndef mac_define_INC  -----
