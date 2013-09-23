	 #include <stdlib.h>
     #include <stdio.h>
     #include <cuda.h>
     
     const int blockSize=16;
     const int gridSize=1024;
     
     const int totalSize=blockSize*gridSize;
     
     typedef unsigned char byte;
     
     __global__ void Calculate_Pi_On_GPU(byte isInsideCircle[])
     {
            const unsigned idx = blockIdx.x*blockDim.x + threadIdx.x;
            const unsigned idy = blockIdx.y*blockDim.y + threadIdx.y;
     
            const unsigned id=idx*totalSize+idy;
     
            double x=double(idx)/totalSize;
            double y=double(idy)/totalSize;
     
            x-=0.5;
            y-=0.5;
     
            isInsideCircle[id]=0;
            if (x*x + y*y<= 0.5*0.5)
                    isInsideCircle[id]=1;
     }
     
     int main()
     {
            int totalPoints=totalSize*totalSize;
     
            cudaError_t cudaError;
            byte* isInsideCircleOnDevice;
     
            cudaError=cudaMalloc((void**)&isInsideCircleOnDevice, totalPoints*sizeof(byte));
            if (cudaError!=cudaSuccess){
                    printf("Out of nvidia device memory\n");
                    return 0;
                    }
     
            /* Call the GPU kernel */
            dim3 block(blockSize,blockSize);
            dim3 grid(gridSize,gridSize);
            Calculate_Pi_On_GPU<<<grid, block>>>(isInsideCircleOnDevice);
     
            byte* isInsideCircle;
            isInsideCircle=(byte*) malloc(totalPoints*sizeof(byte));
            if (isInsideCircle==NULL) {
                    printf("Out of memory on host\n");
                    return 0;
                    }
     
            cudaMemcpy( /* Desination:*/     isInsideCircle,
                               /* Source:    */     isInsideCircleOnDevice,
                               /* Size in bytes: */ totalPoints*sizeof(byte),
                               /* Direction   */    cudaMemcpyDeviceToHost
                              );
     
            int insidePointCount=0;
            for (int i=0;i<totalPoints;i++)
                    insidePointCount+=isInsideCircle[i];
     
            double ratio=double(totalPoints)/insidePointCount;
            double pi=1/(ratio*0.5*0.5);
     
            printf("Total points : %d\n", totalPoints);
            printf("Inside points: %d\n", insidePointCount);
            printf("Ratio        : %f\n", ratio);
            printf("PI:            %f\n", pi);
     
     }
