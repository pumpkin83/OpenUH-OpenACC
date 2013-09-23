# GCC compiler
CC = gcc
#CFLAGS = -g -DACC_WARN_LOG -DACC_DEBUG_LOG 
CFLAGS = -g -DACC_WARN_LOG
LINK = gcc
INCL = -I.. -I/opt/cuda/5.0/include
DYNAMIC_LIBS = -lOpenACC -lcuda -lcudart -lpthread
LIBS = -L.. -L/opt/cuda/5.0/lib64 $(DYNAMIC_LIBS)

#TARGET = rt_vector_add
#TARGET = vectoradd.w2c

ifndef TARGET
TARGET = acc_mm
SRCS = acc_mm.c
else
SRCS = $(TARGET).c
endif

SRCDIR = .
#SRCS = vectoradd.w2c.c
#SRCS = acc_mm.c
#SRCS = rt_vector_add.c
OBJS = $(patsubst %.c, %.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(LINK) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) -g $(INCL) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -f ./$(TARGET) ./*.o
