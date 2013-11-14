#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

typedef struct{
	void *host_addr;
	void *device_addr;
	size_t size;
	unsigned char type; /*indicate in/out from device*/
} param_t;


int main()
{
	int i;
	float *A = (float*)malloc(1024*sizeof(float));
	float *B = (float*)malloc(1024*sizeof(float));
	float *C = (float*)malloc(1024*sizeof(float));
	int key;

	map_t mymap = hashmap_new();
	param_t *param0 = (param_t*)malloc(sizeof(param_t));
	param_t *param1 = (param_t*)malloc(sizeof(param_t));
	param_t *param2 = (param_t*)malloc(sizeof(param_t));

	key = hashmap_length(mymap);
	param0->host_addr = A;
	param0->size = 1024;
	printf("param0 addr: %p, param0->host_addr: %p\n", param0, param0->host_addr);
	hashmap_put(mymap, key, param0);

	
	key = hashmap_length(mymap);
	param1->host_addr = B;
	param1->size = 2048;
	printf("param1 addr: %p, param1->host_addr: %p\n", param1, param1->host_addr);
	hashmap_put(mymap, key, param1);

	key = hashmap_length(mymap);
	param2->host_addr = C;
	param2->size = 4096;
	printf("param2 addr: %p, param2->host_addr: %p\n", param2, param2->host_addr);
	hashmap_put(mymap, key, param2);

	printf("Length of the hashmap: %d\n", hashmap_length(mymap));

	//param_t *inst = (param_t*)malloc(sizeof(param_t));
	param_t *inst;
	hashmap_get(mymap, 0, (void**)&inst);
	printf("Inst addr: %p, Address: %p, size: %d\n", inst, inst->host_addr, inst->size);

	hashmap_get(mymap, 1, (void**)&inst);
	printf("Inst addr: %p, Address: %p, size: %d\n", inst, inst->host_addr, inst->size);

	hashmap_get(mymap, 2, (void**)&inst);
	printf("Inst addr: %p, Address: %p, size: %d\n", inst, inst->host_addr, inst->size);

	for(i=0; i<4; i++)
	{
		if(hashmap_get(mymap, i, (void**)&inst) == MAP_OK)
		{
			printf("The data with key %d is in the map\n", i);
		}else
			printf("The data with key %d is not in the map\n");
	}

	/*find whether a data is present*/
	for(i=0; i<4; i++)
	{
		if(hashmap_get(mymap, i, (void**)&inst) == MAP_OK)
		{
			if(inst->host_addr == C)
				printf("Data C is present in the device\n");
		}
	}

	free(A); free(B); free(C);
	free(param0); free(param1); free(param2);

	return 1;
}

