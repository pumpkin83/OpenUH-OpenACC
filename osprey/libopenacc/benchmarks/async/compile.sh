#$gcc -O3 -I/opt/cuda/5.0/include -L/opt/cuda/5.0/lib64 -o non-async non-async.c -lcuda -lcudart
#gcc -O3 -I/opt/cuda/5.0/include -L/opt/cuda/5.0/lib64 -o async async.c -lcuda -lcudart
#gcc -O3 -I/opt/cuda/5.0/include -L/opt/cuda/5.0/lib64 -o kernel_async kernel_async.c -lcuda -lcudart
#gcc -O3 -I/opt/cuda/5.0/include -L/opt/cuda/5.0/lib64 -o data_async data_async.c -lcuda -lcudart
#gcc -O3 -I/opt/cuda/5.0/include -L/opt/cuda/5.0/lib64 -o stream_data_async stream_data_async.c -lcuda -lcudart
#gcc -O3 -I/opt/cuda/5.0/include -L/opt/cuda/5.0/lib64 -o nonmapped_async nonmapped_async.c -lcuda -lcudart
#gcc -O3 -I/opt/cuda/5.0/include -L/opt/cuda/5.0/lib64 -o array_async array_async.c -lcuda -lcudart
#gcc -I../../ -L../../ -o rt_async rt_async.c -lOpenACC
gcc -I../../ -L../../ -o cpu_async cpu_async.c -lOpenACC
