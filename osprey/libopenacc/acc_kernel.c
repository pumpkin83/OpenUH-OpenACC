/**
 * Author: Rengan Xu
 * University of Houston
 */

#include "acc_kernel.h"

int gangs[3];
int vectors[3];
/*global kernel file name*/
static char cu_filename[512];

/*global kernel module and function*/
static CUmodule cu_module;
static CUfunction cu_function;

void __accr_set_gangs(int x, int y, int z)
{
	gangs[0] = x;
	gangs[1] = y;
	gangs[2] = z;
}

void __accr_set_vectors(int x, int y, int z)
{
	vectors[0] = x;
    vectors[1] = y;
    vectors[2] = z;
}

void __accr_set_gang_num_x(int x)
{
	gangs[0] = x;
}

void __accr_set_gang_num_y(int y)
{
	gangs[1] = y;
}

void __accr_set_gang_num_z(int z)
{
	gangs[2] = z;
}

void __accr_set_vector_num_x(int x)
{
	vectors[0] = x;
}

void __accr_set_vector_num_y(int y)
{
	vectors[1] = y;
}

void __accr_set_vector_num_z(int z)
{
	vectors[2] = z;
}

void __accr_set_default_gang_vector(void)
{
	//gangs[0] = 64;
	gangs[0] = 7813;
	//gangs[1] = 16;
	//gangs[1] = 1022;
	gangs[1] = 1;
	gangs[2] = 1;

	//vectors[0] = 32;
	//vectors[1] = 16;
	vectors[0] = 128;
	vectors[1] = 1;
	vectors[2] = 1;
	
	DEBUG(("Set gangs: %d, %d, %d", gangs[0], gangs[1], gangs[2]));
	DEBUG(("Set vectors: %d, %d, %d", vectors[0], vectors[1], vectors[2]));
}

void __accr_reset_default_gang_vector(void)
{
	gangs[0] = 0;
    gangs[1] = 0;
    gangs[2] = 0;

    vectors[0] = 0;
    vectors[1] = 0;
    vectors[2] = 0;
}

/*
 *szKernelName: kernel function name
 *szKernelLib: kernel file name
 */
void __accr_launchkernel(char* szKernelName, char* szKernelLib, int async_expr)
{
	void **args;
	FILE *fp;
	int file_size;
	CUresult ret;
	int i, args_count;
	param_t *param;

	/*
	 *if the kernel to be launched is different from the previous
	 *kernel, then update the previous kernel as current kernel
	 */
	if(strcmp(cu_filename, szKernelLib) != 0)
	{
		char *ptx_source;
		strncpy(cu_filename, szKernelLib, 512);
		
		fp = fopen(szKernelLib, "rb");
		fseek(fp, 0, SEEK_END);
		file_size = ftell(fp);
		ptx_source = (char*)malloc((file_size+1)*sizeof(char));
		fseek(fp, 0, SEEK_SET);
		fread(ptx_source, sizeof(char), file_size, fp);
		fclose(fp);
		ptx_source[file_size] = '\0';
	
		//printf("Loading module %s\n", ptx_source);
		ret = cuModuleLoadData(&cu_module, ptx_source);
		CUDA_CHECK(ret);

		free(ptx_source);

		//printf("Kernel name: %s\n", szKernelName);	
		//printf("Kernel file name: %s\n", szKernelLib);	
		ret = cuModuleGetFunction(&cu_function, cu_module, szKernelName);	
		CUDA_CHECK(ret);
	}
	
	args_count = vector_length(param_list);
	args = (void**)malloc(args_count*sizeof(void*));
	
	for(i = args_count-1; i >=0; i--)
	{
		param = (param_t*)malloc(sizeof(param_t));
		vector_popback(param_list, param);
		if(param->device_addr != NULL)
		{
			args[i] = &(param->device_addr);
			DEBUG(("args[%d] device address: %p", i, param->device_addr));
		}
		else
		{
			args[i] = param->host_addr;
			DEBUG(("args[%d] host address: %p", i, param->host_addr));
		}

	}

	DEBUG(("Arguments added successfully"));
	DEBUG(("gang_x: %d, gang_y: %d, gang_z: %d", gangs[0], gangs[1], gangs[2]));
	DEBUG(("vector_x: %d, vector_y: %d, vector_z: %d", vectors[0], vectors[1], vectors[2]));

	if(gangs[0] > __acc_gpu_config->max_grid_dim[0] ||
	   gangs[1] > __acc_gpu_config->max_grid_dim[1] ||
	   gangs[2] > __acc_gpu_config->max_grid_dim[2] )
	   ERROR(("Gang number exceeds the limit"));

	if(vectors[0] > __acc_gpu_config->max_block_dim[0] ||
	   vectors[1] > __acc_gpu_config->max_block_dim[1] ||
	   vectors[2] > __acc_gpu_config->max_block_dim[2] ||
	   vectors[0]*vectors[1]*vectors[2] > __acc_gpu_config->max_threads_per_block)
	   ERROR(("Vector number exceeds the limit"));
	

	/* the asynchronous scalar expression do not accept negative value now*/
	if(async_expr < 0)
	{
		ret = cuLaunchKernel(cu_function, gangs[0], gangs[1], gangs[2], 
						     vectors[0], vectors[1], vectors[2], 
						     0, 
						     NULL, args, NULL);
		CUDA_CHECK(ret);
	}else if(async_expr == 0)
	{
		if(async_streams[MODULE_BASE] == NULL)
		{
			CUDA_CHECK( cuStreamCreate(&async_streams[MODULE_BASE], 0) );	
		}
		
		ret = cuLaunchKernel(cu_function, gangs[0], gangs[1], gangs[2], 
						     vectors[0], vectors[1], vectors[2], 
						     0, 
						     async_streams[MODULE_BASE], args, NULL);
		CUDA_CHECK(ret);
	}else
	{
		int stream_pos;
		stream_pos = async_expr % MODULE_BASE;
		
		if(async_streams[stream_pos] == NULL)
		{
			CUDA_CHECK( cuStreamCreate(&async_streams[stream_pos], 0) );	
		}
		
		ret = cuLaunchKernel(cu_function, gangs[0], gangs[1], gangs[2], 
						     vectors[0], vectors[1], vectors[2], 
						     0, 
						     async_streams[stream_pos], args, NULL);
		CUDA_CHECK(ret);
	}

	DEBUG(("The kernel is launched successfully"));
	/* free the memory of kernel parameter */
	free(args);

}