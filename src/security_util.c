/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: This file defines the function for generating a 4-byte pseudorandom number
 * */
#include <stdio.h>
#include <stdint.h>

#include "error_handling.h"
#include "security_util.h"

uint32_t randNum()
{
    unsigned char seqNumBuf[sizeof(uint32_t)];
    if(RAND_bytes(seqNumBuf, sizeof(seqNumBuf)) != 1) {
    	error_handling("Failed to generate random bytes");
    }
    return *((uint32_t *)seqNumBuf);
}

