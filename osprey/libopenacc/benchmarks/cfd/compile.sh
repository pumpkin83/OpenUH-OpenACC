uhcc -I../../ -L../../ -fopenacc euler3d_acc.c -lOpenACC -v
#nvcc --ptx deviceptr.w2c.cu
#uhcc -I../../ -L../../ -o deviceptr deviceptr.w2c.c -lOpenACC
