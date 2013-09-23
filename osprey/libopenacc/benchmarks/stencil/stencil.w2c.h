/* Include builtin types and operators.*/
/* It is like under $TOOLROOT/include/$REL_VER/ */
#include <whirl2c.h>

/* Types */
/* File-level vars and routines */
__inline unsigned int gnu_dev_major(unsigned long long);

__inline unsigned int gnu_dev_minor(unsigned long long);

__inline unsigned long long gnu_dev_makedev(unsigned int, unsigned int);

static int read_data(float *, int, int, int, struct _IO_FILE *);

extern unsigned long long fread(void * restrict, unsigned long long, unsigned long long, struct _IO_FILE * restrict);

extern void cpu_stencil(float, float, float *, float *, const int, const int, const int);

extern void outputData(char *, float *, int, int, int);

extern struct _IO_FILE * fopen(const char * restrict, const char * restrict);

extern int fprintf(struct _IO_FILE * restrict, const char * restrict, ...);

extern struct _IO_FILE * stderr;

extern void exit(int);

extern unsigned long long fwrite(const void * restrict, unsigned long long, unsigned long long, struct _IO_FILE * restrict);

extern int fclose(struct _IO_FILE *);

extern int main(int, char **);

extern int printf(const char *, ...);

extern int atoi(const char *);

extern void * malloc(unsigned long long);

extern void acc_init(unsigned int);

extern int gettimeofday(struct timeval * restrict, struct timezone * restrict);

extern void free(void *);

extern int __accr_get_device_addr();

extern int __accr_set_default_gang_vector();

extern int __accr_set_gang_num_y();

extern int __accr_set_gang_num_x();

extern int __accr_set_vector_num_z();

extern int __accr_set_vector_num_y();

extern int __accr_set_vector_num_x();

static void __accrg_cpu_stencil_1_1(unsigned long long, unsigned long long);

extern int __accr_push_kernel_param_scalar();

extern int __accr_push_kernel_param_pointer();

extern int __accr_launchkernel();

extern int __accr_present_create();

extern int __accr_malloc_on_device();

extern int __accr_memin_h2d();

extern int __accr_memout_d2h();

extern int __accr_free_on_device();

