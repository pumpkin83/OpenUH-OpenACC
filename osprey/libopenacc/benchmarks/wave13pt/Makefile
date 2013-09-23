# GCC compiler
#CC = hmpp --nvcc-options -Xptxas=-v,-arch=sm_35,-fmad=false gcc
#CFLAGS = -O0 
#LINK = hmpp --nvcc-options -Xptxas=-v,-arch=sm_35,-fmad=false gcc
#SRCS = jacobi.c
#TARGET=caps_jacobi

CC = gcc
CFLAGS = -O0
LINK = gcc
SRCS = wave13pt.w2c.c
TARGET = wave13pt

ifeq ($(CC), pgcc)
CC = pgcc
CFLAGS = -O0 -acc -ta=nvidia,cc35,keepgpu,nofma -Msafeptr -Minfo=accel
LINK = pgcc -acc -ta=nvidia,cc35,keepgpu,nofma -Msafeptr -Minfo=accel
SRCS = wave13pt.c
TARGET = pgi_wave13pt
endif


INCL = -I../.. 
LIBS = -L../../ -lOpenACC


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
