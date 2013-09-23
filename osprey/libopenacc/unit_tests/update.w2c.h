/* Include builtin types and operators.*/
/* It is like under $TOOLROOT/include/$REL_VER/ */
#include <whirl2c.h>

/* Types */
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
  /* File-level vars and routines */
__inline int vprintf(const char * restrict, struct __va_list_tag *);

extern int vfprintf(struct _IO_FILE * restrict, const char * restrict, struct __va_list_tag *);

extern struct _IO_FILE * stdout;

__inline int getchar(void);

extern int _IO_getc(struct _IO_FILE *);

extern struct _IO_FILE * stdin;

__inline int fgetc_unlocked(struct _IO_FILE *);

extern int __uflow(struct _IO_FILE *);

__inline int getc_unlocked(struct _IO_FILE *);

__inline int getchar_unlocked(void);

__inline int putchar(int);

extern int _IO_putc(int, struct _IO_FILE *);

__inline int fputc_unlocked(int, struct _IO_FILE *);

extern int __overflow(struct _IO_FILE *, int);

__inline int putc_unlocked(int, struct _IO_FILE *);

__inline int putchar_unlocked(int);

__inline int feof_unlocked(struct _IO_FILE *);

__inline int ferror_unlocked(struct _IO_FILE *);

__inline double strtod(const char * restrict, char ** restrict);

extern double __strtod_internal(const char * restrict, char ** restrict, int);

__inline long long strtol(const char * restrict, char ** restrict, int);

extern long long __strtol_internal(const char * restrict, char ** restrict, int, int);

__inline unsigned long long strtoul(const char * restrict, char ** restrict, int);

extern unsigned long long __strtoul_internal(const char * restrict, char ** restrict, int, int);

__inline long long strtoq(const char * restrict, char ** restrict, int);

extern long long __strtoll_internal(const char * restrict, char ** restrict, int, int);

__inline unsigned long long strtouq(const char * restrict, char ** restrict, int);

extern unsigned long long __strtoull_internal(const char * restrict, char ** restrict, int, int);

__inline long long strtoll(const char * restrict, char ** restrict, int);

__inline unsigned long long strtoull(const char * restrict, char ** restrict, int);

__inline double atof(const char *);

__inline int atoi(const char *);

__inline long long atol(const char *);

__inline long long atoll(const char *);

__inline unsigned int gnu_dev_major(unsigned long long);

__inline unsigned int gnu_dev_minor(unsigned long long);

__inline unsigned long long gnu_dev_makedev(unsigned int, unsigned int);

extern int main(int, const char **);

extern int printf(const char *, ...);

unsigned long long g_subteam;

extern int __accr_present_create();

extern int __accr_malloc_on_device();

extern int __accr_memin_h2d();

extern int __accr_update_device_variable();

static void __accrg_main_1_1(unsigned long long, unsigned long long);

extern int __accr_set_default_gang_vector();

extern int __accr_push_kernel_param_pointer();

extern int __accr_launchkernel();

extern int __accr_update_host_variable();

extern int __accr_memout_d2h();

extern int __accr_free_on_device();

