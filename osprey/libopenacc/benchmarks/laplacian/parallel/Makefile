# GCC compiler
#CC = hmpp --nvcc-options -Xptxas=-v,-arch=sm_35,-fmad=false gcc
#CFLAGS = -O0 
#LINK = hmpp --nvcc-options -Xptxas=-v,-arch=sm_35,-fmad=false gcc
#SRCS = jacobi.c
#TARGET=caps_jacobi

#CC = gcc
#CFLAGS = -O0
#LINK = gcc
#SRCS = laplacian.w2c.c
#TARGET = laplacian

#ifeq ($(CC), pgcc)
CC = pgcc
CFLAGS = -O0 -acc -ta=nvidia,keepgpu,nofma -Msafeptr -Minfo=accel
LINK = pgcc -acc -ta=nvidia,keepgpu,nofma -Msafeptr -Minfo=accel
SRCS = laplacian.c
TARGET = pgi_laplacian
#endif


INCL = -I../.. 
#INCL = -I../.. -I/opt/pgi/linux86-64/13.7/include
LIBS = -L../../
#LIBS = -L../../ -L/opt/pgi/linux86-64/13.7/lib -lOpenACC


OBJS = $(patsubst %.c, %.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(LINK) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(INCL) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -f ./$(TARGET) *.o *.O *.gpu *.h

veryclean:
	@rm -f *.cu *.ptx
