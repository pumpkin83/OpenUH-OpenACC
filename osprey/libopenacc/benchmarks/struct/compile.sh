#uhcc -I../../ -L../../ -o multi_array multi_array.w2c.c -lOpenACC
uhcc -I../../ -L../../ -fopenacc -o multi_array multi_array.c -lOpenACC -v
