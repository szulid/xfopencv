/***************************************************************************
Copyright (c) 2016, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software 
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

#ifndef _XF_SOBEL_CONFIG_H_
#define _XF_SOBEL_CONFIG_H_

#include "xf_headers.h"
#include "common/xf_common.h"
#include "common/xf_utility.h"
#include "imgproc/xf_sobel.hpp"
#include "xf_config_params.h"

typedef unsigned int uint32_t;

#define __SDSOC 0
/* Optimization type */

//#define RO 0 // Resource Optimized (8-pixel implementation)
//#define NO 1 // Normal Operation (1-pixel implementation)

/*  Set Filter size  */

//#define FILTER_SIZE_3  0
//#define FILTER_SIZE_5  0
//#define FILTER_SIZE_7  1

//////////////  To set the parameters in Top and Test bench //////////////////

/* config width and height */
#define WIDTH 	1920
#define HEIGHT	1080

#if FILTER_SIZE_3
#define FILTER_WIDTH   3
#elif FILTER_SIZE_5
#define FILTER_WIDTH   5
#elif FILTER_SIZE_7
#define FILTER_WIDTH   7
#endif

#if (FILTER_WIDTH == 3 | FILTER_WIDTH == 5)
#if RO
#define IN_TYPE      ap_uint<64>
#define OUT_TYPE     ap_uint<64>
#define NPC1 3
#define NPC2 2
#define NPC3 0
#endif
#if NO
#define IN_TYPE      ap_uint<8>
#define OUT_TYPE	 ap_uint<16>
#define NPC1 0
#define NPC2 0
#define NPC3 0
#endif
#endif

#if (FILTER_WIDTH == 7)
#if RO
#define IN_TYPE      ap_uint<64>
#define OUT_TYPE	 ap_uint<64>
#define NPC1 3
#define NPC2 0
#define NPC3 1
#endif
#if NO
#define IN_TYPE      ap_uint<8>
#define OUT_TYPE	 ap_uint<32>
#define NPC1 0
#define NPC2 0
#define NPC3 0
#endif
#endif

#endif //  _XF_SOBEL_CONFIG_H_
