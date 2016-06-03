/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: This file declares the function for generating a 4-byte pseudorandom number
 * * */
#ifndef __SECURITY_UTIL_H__
#define __SECURITY_UTIL_H__

#define SEC_LEVEL_HIGH 0x0001 
#define SEC_LEVEL_LOW 0x0000 

#define FILE_NOT_FOUND 0x0001
#define FILE_OVER_SIZE 0x0002

#include <openssl/rand.h> //include openssl

uint32_t randNum();

#endif

