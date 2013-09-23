nvprof ./pgi_stencil 512 512 64 100
diff out.dat /home/rengan/openacc-test-suite/certification_suite/PARBOIL/parboil/datasets/stencil/default/output/512x512x64.out
