struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
  };
  struct timeval {
    long long tv_sec;
    long long tv_usec;
  };
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
