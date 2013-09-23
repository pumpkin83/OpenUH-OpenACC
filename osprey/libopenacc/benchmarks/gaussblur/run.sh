nvprof ./gaussblur 1024 1024 20000
#nvprof --print-gpu-trace ./pgi_laplacian 128 128 128 5
#nvprof --print-gpu-trace --print-api-trace ./pgi_laplacian 128 128 128 5
#nvprof --print-api-trace ./pgi_laplacian 128 128 128 5
