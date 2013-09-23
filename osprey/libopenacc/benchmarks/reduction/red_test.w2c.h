/* Include builtin types and operators.*/
/* It is like under $TOOLROOT/include/$REL_VER/ */
#include <whirl2c.h>

/* Types */
/* File-level vars and routines */
__inline unsigned int gnu_dev_major(unsigned long long);

__inline unsigned int gnu_dev_minor(unsigned long long);

__inline unsigned long long gnu_dev_makedev(unsigned int, unsigned int);

extern void test1();

extern void * malloc(unsigned long long);

extern void free(void *);

extern int printf(const char *, ...);

extern int main();

extern void acc_init(unsigned int);

extern void acc_shutdown(unsigned int);

extern int __accr_present_create();

extern int __accr_malloc_on_device();

extern int __accr_memin_h2d();

extern int __accr_set_default_gang_vector();

static void __accrg_reduction_ADD_int32_1_1(unsigned long long, unsigned long long);

extern int __syncthreads();

extern int __accr_reduction_buff_malloc();

static void __accrg_test1_1_2(unsigned long long, unsigned long long);

extern int __accr_push_kernel_param_scalar();

extern int __accr_push_kernel_param_pointer();

extern int __accr_launchkernel();

extern int __accr_final_reduction_algorithm();

extern int __accr_free_on_device();

