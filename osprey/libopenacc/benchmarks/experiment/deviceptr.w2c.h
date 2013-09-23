/* Include builtin types and operators.*/
/* It is like under $TOOLROOT/include/$REL_VER/ */
#include <whirl2c.h>

/* Types */
/* File-level vars and routines */
__inline unsigned int gnu_dev_major(unsigned long long);

__inline unsigned int gnu_dev_minor(unsigned long long);

__inline unsigned long long gnu_dev_makedev(unsigned int, unsigned int);

extern int main();

extern void * acc_malloc(unsigned int);

extern void * malloc(unsigned long long);

extern void acc_init(unsigned int);

extern void acc_free(void *);

extern int printf(const char *, ...);

extern int __accr_present_create();

extern int __accr_malloc_on_device();

extern int __accr_set_default_gang_vector();

extern int __accr_set_gang_num_x();

extern int __accr_set_vector_num_x();

static void __accrg_main_1_1(unsigned long long, unsigned long long);

extern int __accr_push_kernel_param_scalar();

extern int __accr_push_kernel_param_pointer();

extern int __accr_launchkernel();

extern int __accr_memout_d2h();

extern int __accr_free_on_device();
