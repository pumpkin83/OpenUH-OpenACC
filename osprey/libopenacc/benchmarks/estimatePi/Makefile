# GCC compiler
#CC = hmpp --nvcc-options -Xptxas=-v,-arch=sm_35,-fmad=false gcc
#CFLAGS = -O0 
#LINK = hmpp --nvcc-options -Xptxas=-v,-arch=sm_35,-fmad=false gcc
#SRCS = jacobi.c
#TARGET=caps_jacobi

CC = gcc
CFLAGS = -O0
LINK = gcc
SRCS = pi.w2c.c
TARGET = pi

ifeq ($(CC), pgcc)
CC = pgcc
CFLAGS = -O0 -acc -ta=nvidia,cc35,keepgpu,nofma -Msafeptr -Minfo=accel
LINK = pgcc -acc -ta=nvidia,cc35,keepgpu,nofma -Msafeptr -Minfo=accel
SRCS = pi.c
TARGET = pgi_pi
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
	@rm -f pgi_pi *.cu *.ptx *.dat
