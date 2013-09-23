#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

typedef struct _param{
	void *host_addr;
	void *device_addr;
	size_t size;
	unsigned char type; /*indicate in/out from device*/
} param_t;

int main()
{
	float *A = (float*)malloc(1024*sizeof(float));
	float *B = (float*)malloc(1024*sizeof(float));
	float *C = (float*)malloc(1024*sizeof(float));
	vector param_list = vector_create(sizeof(param_t));
	param_t *param = (param_t*)malloc(sizeof(param_t));
	param->host_addr = A;
	param->device_addr = NULL;
	param->size = 1024;
	vector_pushback(param_list, param);

	param->host_addr = B;
	param->device_addr = NULL;
	param->size = 2048;
	vector_pushback(param_list, param);

	param->host_addr = C;
	param->device_addr = NULL;
	param->size = 4096;
	vector_pushback(param_list, param);

	printf("Number of elements: %d\n", vector_length(param_list) );
	param_t *inst = (param_t*)malloc(sizeof(param_t));
	vector_popback(param_list, inst);
	printf("inst->size = %d\n", inst->size);
	vector_popback(param_list, inst);
	printf("inst->size = %d\n", inst->size);
	vector_popback(param_list, inst);
	printf("inst->size = %d\n", inst->size);

	free(A);
	free(B);
	free(C);
	free(param);
	vector_destroy(param_list);
}
