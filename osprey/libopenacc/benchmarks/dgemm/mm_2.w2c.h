/* Include builtin types and operators.*/
/* It is like under $TOOLROOT/include/$REL_VER/ */
#include <whirl2c.h>

/* Types */
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
  };
  struct timeval {
    long long tv_sec;
    long long tv_usec;
  };
  /* File-level vars and routines */
__inline unsigned int gnu_dev_major(unsigned long long);

__inline unsigned int gnu_dev_minor(unsigned long long);

__inline unsigned long long gnu_dev_makedev(unsigned int, unsigned int);

extern void zero(double *, int);

extern void initA(double *, int);

extern void initB(double *, int);

extern void initC(double *, int);

extern void iter_matmul(double *, double *, double *, int);

extern void verify(double *, int);

extern int printf(const char *, ...);

extern int main(int, char **);

struct _IO_marker {
    struct _IO_marker * _next;
    struct _IO_FILE * _sbuf;
    int _pos;
  };
  struct _IO_FILE {
  int _flags;
  char * _IO_read_ptr;
  char * _IO_read_end;
  char * _IO_read_base;
  char * _IO_write_base;
  char * _IO_write_ptr;
  char * _IO_write_end;
  char * _IO_buf_base;
  char * _IO_buf_end;
  char * _IO_save_base;
  char * _IO_backup_base;
  char * _IO_save_end;
  struct _IO_marker * _markers;
  struct _IO_FILE * _chain;
  int _fileno;
  int _flags2;
  long long _old_offset;
  unsigned short _cur_column;
  char _vtable_offset;
  char _shortbuf[1LL];
  void * _lock;
  long long _offset;
  void * __pad1;
  void * __pad2;
  void * __pad3;
  void * __pad4;
  unsigned long long __pad5;
  int _mode;
  char _unused2[20LL];
};
extern int fprintf(struct _IO_FILE * restrict, const char * restrict, ...);

extern struct _IO_FILE * stderr;

extern void exit(int);

extern int atoi(const char *);

extern void * malloc(unsigned long long);

extern void acc_init(unsigned int);

extern int gettimeofday(struct timeval * restrict, struct timezone * restrict);

extern void free(void *);

extern int __accr_present_create();

extern int __accr_malloc_on_device();

extern int __accr_memin_h2d();

extern int __accr_set_default_gang_vector();

extern int __accr_set_gang_num_y();

extern int __accr_set_gang_num_x();

extern int __accr_set_vector_num_y();

static void __accrg_iter_matmul_1_1(unsigned long long, unsigned long long);

extern int __accr_push_kernel_param_scalar();

extern int __accr_push_kernel_param_pointer();

extern int __accr_launchkernel();

extern int __accr_memout_d2h();

extern int __accr_free_on_device();

