/****************************************************************************\
|*                                                                          *|
|*      Copyright 2010-2012 NVIDIA Corporation.  All rights reserved.       *|
|*                                                                          *|
|*   NOTICE TO USER:                                                        *|
|*                                                                          *|
|*   This source code is subject to NVIDIA ownership rights under U.S.      *|
|*   and international Copyright laws.  Users and possessors of this        *|
|*   source code are hereby granted a nonexclusive, royalty-free            *|
|*   license to use this code in individual and commercial software.        *|
|*                                                                          *|
|*   NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE    *|
|*   CODE FOR ANY PURPOSE. IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR        *|
|*   IMPLIED WARRANTY OF ANY KIND. NVIDIA DISCLAIMS ALL WARRANTIES WITH     *|
|*   REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF        *|
|*   MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR         *|
|*   PURPOSE. IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL,           *|
|*   INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES         *|
|*   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN     *|
|*   AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING    *|
|*   OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE     *|
|*   CODE.                                                                  *|
|*                                                                          *|
|*   U.S. Government End Users. This source code is a "commercial item"     *|
|*   as that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting      *|
|*   of "commercial computer  software" and "commercial computer software   *|
|*   documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995)  *|
|*   and is provided to the U.S. Government only as a commercial end item.  *|
|*   Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through       *|
|*   227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the      *|
|*   source code with only those rights set forth herein.                   *|
|*                                                                          *|
|*   Any use of this source code in individual and commercial software must *|
|*   include, in the user documentation and internal comments to the code,  *|
|*   the above Disclaimer and U.S. Government End Users Notice.             *|
|*                                                                          *|
\****************************************************************************/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>


//#define N 1000000

int main(void) {
	double pi = 0.0f;
	long N;
	long i;
	struct timeval tim;
	double start, end;
	printf("Estimate Pi OpenACC\n");
	
	N = 1000000;
	gettimeofday(&tim, NULL);
	start = tim.tv_sec*1000.0 + (tim.tv_usec/1000.0);

#pragma acc kernels param(N) 
{
	#pragma acc loop reduction(+:pi)
	for (i=0; i<N; i++)
	{
           double t= (double)((i+0.5)/N);
           pi +=4.0/(1.0+t*t);
	}
}

	gettimeofday(&tim, NULL);
	end = tim.tv_sec*1000.0 + (tim.tv_usec/1000.0);

	printf("pi=%16.15f\n",pi/N);
	printf("Time %.2f (ms)\n", end-start);
	return 0;
}
