i=1
while(($i<4))
do
cd stencil/map31
make CC=pgcc -f Makefile1
./run.sh >& map31_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map31_O3_$i.txt
make clean -f Makefile2

cd ../map32
make CC=pgcc -f Makefile1
./run.sh >& map32_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map32_O3_$i.txt
make clean -f Makefile2

cd ../map33
make CC=pgcc -f Makefile1
./run.sh >& map33_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map33_O3_$i.txt
make clean -f Makefile2

cd ../../laplacian/map31
make CC=pgcc -f Makefile1
./run.sh >& map31_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map31_O3_$i.txt
make clean -f Makefile2

cd ../map32
make CC=pgcc -f Makefile1
./run.sh >& map32_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map32_O3_$i.txt
make clean -f Makefile2

cd ../map33
make CC=pgcc -f Makefile1
./run.sh >& map33_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map33_O3_$i.txt
make clean -f Makefile2

cd ../../wave13pt/map31
make CC=pgcc -f Makefile1
./run.sh >& map31_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map31_O3_$i.txt
make clean -f Makefile2

cd ../map32
make CC=pgcc -f Makefile1
./run.sh >& map32_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map32_O3_$i.txt
make clean -f Makefile2

cd ../map33
make CC=pgcc -f Makefile1
./run.sh >& map33_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map33_O3_$i.txt
make clean -f Makefile2
cd ../../
i=$(($i+1))
done

