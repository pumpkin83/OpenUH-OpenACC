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

extern float get_sum(float *, int);

extern int main(int, char **);

extern void srand(unsigned int);

extern void * valloc(unsigned long long);

extern void * malloc(unsigned long long);

extern int rand(void);

extern void acc_init(unsigned int);

extern int gettimeofday(struct timeval * restrict, struct timezone * restrict);

extern int printf(const char *, ...);

extern void acc_shutdown(unsigned int);

extern int __accr_present_create();

extern int __accr_malloc_on_device();

extern int __accr_update_device_variable();

extern int __accr_set_default_gang_vector();

extern int __accr_set_gang_num_y();

extern int __accr_set_gang_num_x();

extern int __accr_set_vector_num_y();

extern int __accr_set_vector_num_x();

static void __accrg_main_1_1(unsigned long long, unsigned long long);

extern int __accr_push_kernel_param_scalar();

extern int __accr_push_kernel_param_pointer();

extern int __accr_launchkernel();

extern int __accr_update_host_variable();

extern int __accr_wait_some_or_all_stream();

extern int __accr_free_on_device();

