i=1
while(($i<4))
do
cd gaussblur/map23
make CC=pgcc -f Makefile1
./run.sh >& map23_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map23_O3_$i.txt
make clean -f Makefile2

cd ../map24
make CC=pgcc -f Makefile1
./run.sh >& map24_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map24_O3_$i.txt
make clean -f Makefile2

cd ../../jacobi/map23
make CC=pgcc -f Makefile1
./run.sh >& map23_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map23_O3_$i.txt
make clean -f Makefile2

cd ../map24
make CC=pgcc -f Makefile1
./run.sh >& map24_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map24_O3_$i.txt
make clean -f Makefile2

cd ../../dgemm/map23
make CC=pgcc -f Makefile1
./run.sh >& map23_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map23_O3_$i.txt
make clean -f Makefile2

cd ../map24
make CC=pgcc -f Makefile1
./run.sh >& map24_O0_$i.txt
make clean -f Makefile1
make CC=pgcc -f Makefile2
./run.sh >& map24_O3_$i.txt
make clean -f Makefile2
cd ../../

i=$(($i+1))
done

