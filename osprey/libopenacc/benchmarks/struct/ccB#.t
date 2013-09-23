=======================================================================
WN_Lower: "Fast exponents lowering"
flags are:
<unrecognized> 
 LOC 0 0 source files:	1	"/home/rengan/openacc/uh_libopenacc/benchmarks/struct/multi_array.c"
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
       I4ADD
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
 REGION 1 (kind=16) {line: 1/30}
 REGION EXITS
  BLOCK {line: 0/0}
  END_BLOCK
 REGION PRAGMAS
  BLOCK {line: 1/30}
  PRAGMA 4 197 <null-st> 0 (0x0) # ACC_KERNELS_BEGIN {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 212 <1,52,y> # ACC_CLAUSE_COPYOUT {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 211 <1,51,x> # ACC_CLAUSE_COPYIN {line: 1/30}
  END_BLOCK
 REGION BODY
  BLOCK {line: 1/30}
  REGION 2 (kind=16) {line: 1/32}
  REGION EXITS
   BLOCK {line: 0/0}
   END_BLOCK
  REGION PRAGMAS
   BLOCK {line: 1/32}
   PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/32}
    I4INTCONST 8 (0x8)
   XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/32}
   END_BLOCK
  REGION BODY
   BLOCK {line: 1/32}
   DO_LOOP {line: 1/32}
    IDNAME 0 <2,3,k>
   INIT
     I4INTCONST 0 (0x0)
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   COMP
     I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
     I4INTCONST 7 (0x7)
    I4I4LE
   INCR
      I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   BODY
    BLOCK {line: 1/32}
    REGION 3 (kind=16) {line: 1/36}
    REGION EXITS
     BLOCK {line: 0/0}
     END_BLOCK
    REGION PRAGMAS
     BLOCK {line: 1/36}
     PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/36}
      I4INTCONST 2 (0x2)
     XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/36}
      I4INTCONST 16 (0x10)
     XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/36}
     END_BLOCK
    REGION BODY
     BLOCK {line: 1/36}
     DO_LOOP {line: 1/36}
      IDNAME 0 <2,2,j>
     INIT
       I4INTCONST 0 (0x0)
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     COMP
       I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
       I4INTCONST 15 (0xf)
      I4I4LE
     INCR
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4INTCONST 1 (0x1)
       I4ADD
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     BODY
      BLOCK {line: 1/36}
      REGION 4 (kind=16) {line: 1/40}
      REGION EXITS
       BLOCK {line: 0/0}
       END_BLOCK
      REGION PRAGMAS
       BLOCK {line: 1/40}
       PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/40}
        I4INTCONST 128 (0x80)
       XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/40}
       END_BLOCK
      REGION BODY
       BLOCK {line: 1/40}
       DO_LOOP {line: 1/40}
        IDNAME 0 <2,1,i>
       INIT
         I4INTCONST 0 (0x0)
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       COMP
         I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         I4INTCONST 127 (0x7f)
        I4I4LE
       INCR
          I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
          I4INTCONST 1 (0x1)
         I4ADD
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       BODY
        BLOCK {line: 1/40}
             U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
             I4INTCONST 8 (0x8)
             I4INTCONST 16 (0x10)
             I4INTCONST 128 (0x80)
             I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
             I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
             I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
            U8ARRAY 3 4
           I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
           I4INTCONST 3 (0x3)
          I4MPY
          I4INTCONST 2 (0x2)
         I4ADD
          U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
        END_BLOCK
       END_BLOCK
      END_REGION 4
      END_BLOCK
     END_BLOCK
    END_REGION 3
    END_BLOCK
   END_BLOCK
  END_REGION 2
  END_BLOCK
 END_REGION 1
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4RETURN_VAL {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
 REGION 1 (kind=16) {line: 1/30}
 REGION EXITS
  BLOCK {line: 0/0}
  END_BLOCK
 REGION PRAGMAS
  BLOCK {line: 1/30}
  PRAGMA 4 197 <null-st> 0 (0x0) # ACC_KERNELS_BEGIN {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 212 <1,52,y> # ACC_CLAUSE_COPYOUT {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 211 <1,51,x> # ACC_CLAUSE_COPYIN {line: 1/30}
  END_BLOCK
 REGION BODY
  BLOCK {line: 1/30}
  REGION 2 (kind=16) {line: 1/32}
  REGION EXITS
   BLOCK {line: 0/0}
   END_BLOCK
  REGION PRAGMAS
   BLOCK {line: 1/32}
   PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/32}
    I4INTCONST 8 (0x8)
   XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/32}
   END_BLOCK
  REGION BODY
   BLOCK {line: 1/32}
   DO_LOOP {line: 1/32}
    IDNAME 0 <2,3,k>
   INIT
     I4INTCONST 0 (0x0)
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   COMP
     I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
     I4INTCONST 7 (0x7)
    I4I4LE
   INCR
      I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   BODY
    BLOCK {line: 1/32}
    REGION 3 (kind=16) {line: 1/36}
    REGION EXITS
     BLOCK {line: 0/0}
     END_BLOCK
    REGION PRAGMAS
     BLOCK {line: 1/36}
     PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/36}
      I4INTCONST 2 (0x2)
     XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/36}
      I4INTCONST 16 (0x10)
     XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/36}
     END_BLOCK
    REGION BODY
     BLOCK {line: 1/36}
     DO_LOOP {line: 1/36}
      IDNAME 0 <2,2,j>
     INIT
       I4INTCONST 0 (0x0)
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     COMP
       I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
       I4INTCONST 15 (0xf)
      I4I4LE
     INCR
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4INTCONST 1 (0x1)
       I4ADD
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     BODY
      BLOCK {line: 1/36}
      REGION 4 (kind=16) {line: 1/40}
      REGION EXITS
       BLOCK {line: 0/0}
       END_BLOCK
      REGION PRAGMAS
       BLOCK {line: 1/40}
       PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/40}
        I4INTCONST 128 (0x80)
       XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/40}
       END_BLOCK
      REGION BODY
       BLOCK {line: 1/40}
       DO_LOOP {line: 1/40}
        IDNAME 0 <2,1,i>
       INIT
         I4INTCONST 0 (0x0)
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       COMP
         I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         I4INTCONST 127 (0x7f)
        I4I4LE
       INCR
          I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
          I4INTCONST 1 (0x1)
         I4ADD
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       BODY
        BLOCK {line: 1/40}
             U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
             I4INTCONST 8 (0x8)
             I4INTCONST 16 (0x10)
             I4INTCONST 128 (0x80)
             I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
             I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
             I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
            U8ARRAY 3 4
           I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
           I4INTCONST 3 (0x3)
          I4MPY
          I4INTCONST 2 (0x2)
         I4ADD
          U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
        END_BLOCK
       END_BLOCK
      END_REGION 4
      END_BLOCK
     END_BLOCK
    END_REGION 3
    END_BLOCK
   END_BLOCK
  END_REGION 2
  END_BLOCK
 END_REGION 1
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4RETURN_VAL {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "RETURN_VAL & MLDID/MSTID lowering"
flags are:
LOWER_RETURN_VAL 
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
 REGION 1 1 (kind=16) {line: 1/30}
 REGION EXITS
  BLOCK {line: 0/0}
  REGION_EXIT L8962 {line: 1/32}
  END_BLOCK
 REGION PRAGMAS
  BLOCK {line: 1/30}
  PRAGMA 4 197 <null-st> 0 (0x0) # ACC_KERNELS_BEGIN {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 212 <1,52,y> # ACC_CLAUSE_COPYOUT {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 211 <1,51,x> # ACC_CLAUSE_COPYIN {line: 1/30}
  END_BLOCK
 REGION BODY
  BLOCK {line: 1/30}
  REGION 2 2 (kind=16) {line: 1/32}
  REGION EXITS
   BLOCK {line: 0/0}
   REGION_EXIT L9218 {line: 1/32}
   END_BLOCK
  REGION PRAGMAS
   BLOCK {line: 1/32}
   PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/32}
    I4INTCONST 8 (0x8)
   XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/32}
   END_BLOCK
  REGION BODY
   BLOCK {line: 1/32}
   DO_LOOP {line: 1/32}
    IDNAME 0 <2,3,k>
   INIT
     I4INTCONST 0 (0x0)
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   COMP
     I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
     I4INTCONST 7 (0x7)
    I4I4LE
   INCR
      I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   BODY
    BLOCK {line: 1/32}
    REGION 3 3 (kind=16) {line: 1/36}
    REGION EXITS
     BLOCK {line: 0/0}
     REGION_EXIT L9474 {line: 1/36}
     END_BLOCK
    REGION PRAGMAS
     BLOCK {line: 1/36}
     PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/36}
      I4INTCONST 2 (0x2)
     XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/36}
      I4INTCONST 16 (0x10)
     XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/36}
     END_BLOCK
    REGION BODY
     BLOCK {line: 1/36}
     DO_LOOP {line: 1/36}
      IDNAME 0 <2,2,j>
     INIT
       I4INTCONST 0 (0x0)
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     COMP
       I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
       I4INTCONST 15 (0xf)
      I4I4LE
     INCR
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4INTCONST 1 (0x1)
       I4ADD
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     BODY
      BLOCK {line: 1/36}
      REGION 4 4 (kind=16) {line: 1/40}
      REGION EXITS
       BLOCK {line: 0/0}
       REGION_EXIT L9730 {line: 1/40}
       END_BLOCK
      REGION PRAGMAS
       BLOCK {line: 1/40}
       PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/40}
        I4INTCONST 128 (0x80)
       XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/40}
       END_BLOCK
      REGION BODY
       BLOCK {line: 1/40}
       DO_LOOP {line: 1/40}
        IDNAME 0 <2,1,i>
       INIT
         I4INTCONST 0 (0x0)
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       COMP
         I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         I4INTCONST 127 (0x7f)
        I4I4LE
       INCR
          I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
          I4INTCONST 1 (0x1)
         I4ADD
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       BODY
        BLOCK {line: 1/40}
             U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
             I4INTCONST 8 (0x8)
             I4INTCONST 16 (0x10)
             I4INTCONST 128 (0x80)
             I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
             I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
             I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
            U8ARRAY 3 4
           I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
           I4INTCONST 3 (0x3)
          I4MPY
          I4INTCONST 2 (0x2)
         I4ADD
          U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
        END_BLOCK
       REGION_EXIT L9730 {line: 1/40}
       END_BLOCK
      END_REGION 4
     LABEL L9730 0 {line: 1/36}
      END_BLOCK
     REGION_EXIT L9474 {line: 1/36}
     END_BLOCK
    END_REGION 3
   LABEL L9474 0 {line: 1/32}
    END_BLOCK
   REGION_EXIT L9218 {line: 1/32}
   END_BLOCK
  END_REGION 2
 LABEL L9218 0 {line: 1/32}
  REGION_EXIT L8962 {line: 1/32}
  END_BLOCK
 END_REGION 1
LABEL L8962 0 {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4RETURN_VAL {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
 REGION 1 1 (kind=16) {line: 1/30}
 REGION EXITS
  BLOCK {line: 0/0}
  REGION_EXIT L8962 {line: 1/32}
  END_BLOCK
 REGION PRAGMAS
  BLOCK {line: 1/30}
  PRAGMA 4 197 <null-st> 0 (0x0) # ACC_KERNELS_BEGIN {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 212 <1,52,y> # ACC_CLAUSE_COPYOUT {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 211 <1,51,x> # ACC_CLAUSE_COPYIN {line: 1/30}
  END_BLOCK
 REGION BODY
  BLOCK {line: 1/30}
  REGION 2 2 (kind=16) {line: 1/32}
  REGION EXITS
   BLOCK {line: 0/0}
   REGION_EXIT L9218 {line: 1/32}
   END_BLOCK
  REGION PRAGMAS
   BLOCK {line: 1/32}
   PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/32}
    I4INTCONST 8 (0x8)
   XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/32}
   END_BLOCK
  REGION BODY
   BLOCK {line: 1/32}
   DO_LOOP {line: 1/32}
    IDNAME 0 <2,3,k>
   INIT
     I4INTCONST 0 (0x0)
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   COMP
     I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
     I4INTCONST 7 (0x7)
    I4I4LE
   INCR
      I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   BODY
    BLOCK {line: 1/32}
    REGION 3 3 (kind=16) {line: 1/36}
    REGION EXITS
     BLOCK {line: 0/0}
     REGION_EXIT L9474 {line: 1/36}
     END_BLOCK
    REGION PRAGMAS
     BLOCK {line: 1/36}
     PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/36}
      I4INTCONST 2 (0x2)
     XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/36}
      I4INTCONST 16 (0x10)
     XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/36}
     END_BLOCK
    REGION BODY
     BLOCK {line: 1/36}
     DO_LOOP {line: 1/36}
      IDNAME 0 <2,2,j>
     INIT
       I4INTCONST 0 (0x0)
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     COMP
       I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
       I4INTCONST 15 (0xf)
      I4I4LE
     INCR
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4INTCONST 1 (0x1)
       I4ADD
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     BODY
      BLOCK {line: 1/36}
      REGION 4 4 (kind=16) {line: 1/40}
      REGION EXITS
       BLOCK {line: 0/0}
       REGION_EXIT L9730 {line: 1/40}
       END_BLOCK
      REGION PRAGMAS
       BLOCK {line: 1/40}
       PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/40}
        I4INTCONST 128 (0x80)
       XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/40}
       END_BLOCK
      REGION BODY
       BLOCK {line: 1/40}
       DO_LOOP {line: 1/40}
        IDNAME 0 <2,1,i>
       INIT
         I4INTCONST 0 (0x0)
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       COMP
         I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         I4INTCONST 127 (0x7f)
        I4I4LE
       INCR
          I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
          I4INTCONST 1 (0x1)
         I4ADD
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       BODY
        BLOCK {line: 1/40}
             U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
             I4INTCONST 8 (0x8)
             I4INTCONST 16 (0x10)
             I4INTCONST 128 (0x80)
             I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
             I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
             I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
            U8ARRAY 3 4
           I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
           I4INTCONST 3 (0x3)
          I4MPY
          I4INTCONST 2 (0x2)
         I4ADD
          U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
        END_BLOCK
       REGION_EXIT L9730 {line: 1/40}
       END_BLOCK
      END_REGION 4
     LABEL L9730 0 {line: 1/36}
      END_BLOCK
     REGION_EXIT L9474 {line: 1/36}
     END_BLOCK
    END_REGION 3
   LABEL L9474 0 {line: 1/32}
    END_BLOCK
   REGION_EXIT L9218 {line: 1/32}
   END_BLOCK
  END_REGION 2
 LABEL L9218 0 {line: 1/32}
  REGION_EXIT L8962 {line: 1/32}
  END_BLOCK
 END_REGION 1
LABEL L8962 0 {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68}
 RETURN {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "W2C Lowering"
flags are:
LOWER_MP <unrecognized> 
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
 REGION 1 1 (kind=16) {line: 1/30}
 REGION EXITS
  BLOCK {line: 0/0}
  REGION_EXIT L8962 {line: 1/32}
  END_BLOCK
 REGION PRAGMAS
  BLOCK {line: 1/30}
  PRAGMA 4 197 <null-st> 0 (0x0) # ACC_KERNELS_BEGIN {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 212 <1,52,y> # ACC_CLAUSE_COPYOUT {line: 1/30}
    I4INTCONST 0 (0x0)
    I4INTCONST 0 (0x0)
   I4I4LT
  XPRAGMA 4 211 <1,51,x> # ACC_CLAUSE_COPYIN {line: 1/30}
  END_BLOCK
 REGION BODY
  BLOCK {line: 1/30}
  REGION 2 2 (kind=16) {line: 1/32}
  REGION EXITS
   BLOCK {line: 0/0}
   REGION_EXIT L9218 {line: 1/32}
   END_BLOCK
  REGION PRAGMAS
   BLOCK {line: 1/32}
   PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/32}
    I4INTCONST 8 (0x8)
   XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/32}
   END_BLOCK
  REGION BODY
   BLOCK {line: 1/32}
   DO_LOOP {line: 1/32}
    IDNAME 0 <2,3,k>
   INIT
     I4INTCONST 0 (0x0)
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   COMP
     I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
     I4INTCONST 7 (0x7)
    I4I4LE
   INCR
      I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/32}
   BODY
    BLOCK {line: 1/32}
    REGION 3 3 (kind=16) {line: 1/36}
    REGION EXITS
     BLOCK {line: 0/0}
     REGION_EXIT L9474 {line: 1/36}
     END_BLOCK
    REGION PRAGMAS
     BLOCK {line: 1/36}
     PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/36}
      I4INTCONST 2 (0x2)
     XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/36}
      I4INTCONST 16 (0x10)
     XPRAGMA 4 230 <null-st> # ACC_CLAUSE_GANG {line: 1/36}
     END_BLOCK
    REGION BODY
     BLOCK {line: 1/36}
     DO_LOOP {line: 1/36}
      IDNAME 0 <2,2,j>
     INIT
       I4INTCONST 0 (0x0)
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     COMP
       I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
       I4INTCONST 15 (0xf)
      I4I4LE
     INCR
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4INTCONST 1 (0x1)
       I4ADD
      I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/36}
     BODY
      BLOCK {line: 1/36}
      REGION 4 4 (kind=16) {line: 1/40}
      REGION EXITS
       BLOCK {line: 0/0}
       REGION_EXIT L9730 {line: 1/40}
       END_BLOCK
      REGION PRAGMAS
       BLOCK {line: 1/40}
       PRAGMA 4 203 <null-st> 0 (0x0) # ACC_LOOP_BEGIN {line: 1/40}
        I4INTCONST 128 (0x80)
       XPRAGMA 4 232 <null-st> # ACC_CLAUSE_VECTOR {line: 1/40}
       END_BLOCK
      REGION BODY
       BLOCK {line: 1/40}
       DO_LOOP {line: 1/40}
        IDNAME 0 <2,1,i>
       INIT
         I4INTCONST 0 (0x0)
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       COMP
         I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         I4INTCONST 127 (0x7f)
        I4I4LE
       INCR
          I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
          I4INTCONST 1 (0x1)
         I4ADD
        I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/40}
       BODY
        BLOCK {line: 1/40}
             U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
             I4INTCONST 8 (0x8)
             I4INTCONST 16 (0x10)
             I4INTCONST 128 (0x80)
             I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
             I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
             I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
            U8ARRAY 3 4
           I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
           I4INTCONST 3 (0x3)
          I4MPY
          I4INTCONST 2 (0x2)
         I4ADD
          U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
        END_BLOCK
       REGION_EXIT L9730 {line: 1/40}
       END_BLOCK
      END_REGION 4
     LABEL L9730 0 {line: 1/36}
      END_BLOCK
     REGION_EXIT L9474 {line: 1/36}
     END_BLOCK
    END_REGION 3
   LABEL L9474 0 {line: 1/32}
    END_BLOCK
   REGION_EXIT L9218 {line: 1/32}
   END_BLOCK
  END_REGION 2
 LABEL L9218 0 {line: 1/32}
  REGION_EXIT L8962 {line: 1/32}
  END_BLOCK
 END_REGION 1
LABEL L8962 0 {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68}
 RETURN {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0}
 IF {line: 0/0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
   U8PARM 2 T<57,anon_ptr.,8> #  by_value 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    U4INTCONST 0 (0x0)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    I4INTCONST -2 (0xfffffffffffffffe)
   I4PARM 2 T<4,.predef_I4,4> #  by_value 
  VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0}
  END_BLOCK
 END_IF
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0}
 IF {line: 0/0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0}
    U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0}
  END_BLOCK
 END_IF
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0}
   I4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0}
   I4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0}
   I4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0}
   I4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
LABEL L8962 0 {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68}
 RETURN {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "Before ACC Lowering"
flags are:
<unrecognized> 
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0}
 IF {line: 0/0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
   U8PARM 2 T<57,anon_ptr.,8> #  by_value 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    U4INTCONST 0 (0x0)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    I4INTCONST -2 (0xfffffffffffffffe)
   I4PARM 2 T<4,.predef_I4,4> #  by_value 
  VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0}
  END_BLOCK
 END_IF
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0}
 IF {line: 0/0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0}
    U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0}
  END_BLOCK
 END_IF
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0}
   I4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0}
   I4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0}
   I4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0}
   I4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
LABEL L8962 0 {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68}
 RETURN {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 WHILE_DO {line: 1/17}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17}
 LABEL L514 0 {line: 1/17}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  WHILE_DO {line: 1/19}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19}
  LABEL L1026 0 {line: 1/19}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   WHILE_DO {line: 1/21}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21}
   LABEL L1538 0 {line: 1/21}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21}
   LABEL L1282 0 {line: 1/21}
    END_BLOCK
   GOTO L1794 {line: 1/21}
  LABEL L1794 0 {line: 1/21}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19}
  LABEL L770 0 {line: 1/19}
   END_BLOCK
  GOTO L2050 {line: 1/19}
 LABEL L2050 0 {line: 1/19}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17}
 LABEL L258 0 {line: 1/17}
  END_BLOCK
 GOTO L2306 {line: 1/17}
LABEL L2306 0 {line: 1/17}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0}
 IF {line: 0/0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
   U8PARM 2 T<57,anon_ptr.,8> #  by_value 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    U4INTCONST 0 (0x0)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    I4INTCONST -2 (0xfffffffffffffffe)
   I4PARM 2 T<4,.predef_I4,4> #  by_value 
  VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0}
  END_BLOCK
 END_IF
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0}
 IF {line: 0/0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0}
    U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0}
  END_BLOCK
 END_IF
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0}
   I4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0}
   I4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0}
   I4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0}
   I4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
LABEL L8962 0 {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 WHILE_DO {line: 1/49}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49}
 LABEL L2818 0 {line: 1/49}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  WHILE_DO {line: 1/51}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51}
  LABEL L3330 0 {line: 1/51}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   WHILE_DO {line: 1/53}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53}
   LABEL L3842 0 {line: 1/53}
    IF {line: 1/55}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53}
   LABEL L3586 0 {line: 1/53}
    END_BLOCK
   GOTO L4098 {line: 1/53}
  LABEL L4098 0 {line: 1/53}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51}
  LABEL L3074 0 {line: 1/51}
   END_BLOCK
  GOTO L4354 {line: 1/51}
 LABEL L4354 0 {line: 1/51}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49}
 LABEL L2562 0 {line: 1/49}
  END_BLOCK
 GOTO L4610 {line: 1/49}
LABEL L4610 0 {line: 1/49}
 IF {line: 1/61}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68}
 RETURN {line: 1/68}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "Pre_Opt"
flags are:
LOWER_COMPLEX LOWER_ARRAY LOWER_ENTRY_EXIT LOWER_IO_STATEMENT LOWER_INLINE_INTRINSIC <unrecognized> LOWER_PREFETCH_MAPS LOWER_ALIAS_MAPS LOWER_DEPGRAPH_MAPS LOWER_PARITY_MAPS LOWER_BASE_INDEX LOWER_FREQUENCY_MAPS LOWER_UPLEVEL LOWER_SHORTCIRCUIT LOWER_BIT_FIELD_ID LOWER_TO_MEMLIB <unrecognized> 
FUNC_ENTRY <1,50,main> {line: 1/13} {freq: 0, ln: 13, col: 0}
BODY
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 1/13} {freq: 0, ln: 13, col: 0}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13} {freq: 0, ln: 13, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17} {freq: 0, ln: 17, col: 0}
 WHILE_DO {line: 1/17} {freq: 0, ln: 17, col: 0}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17} {freq: 0, ln: 17, col: 0}
 LABEL L514 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19} {freq: 0, ln: 19, col: 0}
  WHILE_DO {line: 1/19} {freq: 0, ln: 19, col: 0}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19} {freq: 0, ln: 19, col: 0}
  LABEL L1026 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21} {freq: 0, ln: 21, col: 0}
   WHILE_DO {line: 1/21} {freq: 0, ln: 21, col: 0}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21} {freq: 0, ln: 21, col: 0}
   LABEL L1538 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
      I4INTCONST 8 (0x8)
      I4INTCONST 16 (0x10)
      I4INTCONST 128 (0x80)
      I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
      I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
      I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     U8ARRAY 3 4
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22} {freq: 0, ln: 22, col: 0}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21} {freq: 0, ln: 21, col: 0}
   LABEL L1282 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
    END_BLOCK
   GOTO L1794 {line: 1/21} {freq: 0, ln: 21, col: 0}
  LABEL L1794 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19} {freq: 0, ln: 19, col: 0}
  LABEL L770 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
   END_BLOCK
  GOTO L2050 {line: 1/19} {freq: 0, ln: 19, col: 0}
 LABEL L2050 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17} {freq: 0, ln: 17, col: 0}
 LABEL L258 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
  END_BLOCK
 GOTO L2306 {line: 1/17} {freq: 0, ln: 17, col: 0}
LABEL L2306 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IF {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
   U8PARM 2 T<57,anon_ptr.,8> #  by_value 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    U4INTCONST 0 (0x0)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    I4INTCONST -2 (0xfffffffffffffffe)
   I4PARM 2 T<4,.predef_I4,4> #  by_value 
  VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_IF
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IF {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_IF
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
LABEL L8962 0 {line: 1/48} {freq: 0, ln: 48, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48} {freq: 0, ln: 48, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49} {freq: 0, ln: 49, col: 0}
 WHILE_DO {line: 1/49} {freq: 0, ln: 49, col: 0}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49} {freq: 0, ln: 49, col: 0}
 LABEL L2818 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51} {freq: 0, ln: 51, col: 0}
  WHILE_DO {line: 1/51} {freq: 0, ln: 51, col: 0}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51} {freq: 0, ln: 51, col: 0}
  LABEL L3330 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53} {freq: 0, ln: 53, col: 0}
   WHILE_DO {line: 1/53} {freq: 0, ln: 53, col: 0}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53} {freq: 0, ln: 53, col: 0}
   LABEL L3842 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
    IF {line: 1/55} {freq: 0, ln: 55, col: 0}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
        I4INTCONST 8 (0x8)
        I4INTCONST 16 (0x10)
        I4INTCONST 128 (0x80)
        I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
        I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
       U8ARRAY 3 4
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
          I4INTCONST 8 (0x8)
          I4INTCONST 16 (0x10)
          I4INTCONST 128 (0x80)
          I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
         U8ARRAY 3 4
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55} {freq: 0, ln: 55, col: 0}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55} {freq: 0, ln: 55, col: 0}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55} {freq: 0, ln: 55, col: 0}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53} {freq: 0, ln: 53, col: 0}
   LABEL L3586 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
    END_BLOCK
   GOTO L4098 {line: 1/53} {freq: 0, ln: 53, col: 0}
  LABEL L4098 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51} {freq: 0, ln: 51, col: 0}
  LABEL L3074 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
   END_BLOCK
  GOTO L4354 {line: 1/51} {freq: 0, ln: 51, col: 0}
 LABEL L4354 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49} {freq: 0, ln: 49, col: 0}
 LABEL L2562 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
  END_BLOCK
 GOTO L4610 {line: 1/49} {freq: 0, ln: 49, col: 0}
LABEL L4610 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
 IF {line: 1/61} {freq: 0, ln: 61, col: 0}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61} {freq: 0, ln: 61, col: 0}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61} {freq: 0, ln: 61, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61} {freq: 0, ln: 61, col: 0}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61} {freq: 0, ln: 61, col: 0}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68} {freq: 0, ln: 68, col: 0}
 RETURN {line: 1/68} {freq: 0, ln: 68, col: 0}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,50,main> {line: 1/13} {freq: 0, ln: 13, col: 0}
BODY
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 1/13} {freq: 0, ln: 13, col: 0}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13} {freq: 0, ln: 13, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17} {freq: 0, ln: 17, col: 0}
 WHILE_DO {line: 1/17} {freq: 0, ln: 17, col: 0}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17} {freq: 0, ln: 17, col: 0}
 LABEL L514 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19} {freq: 0, ln: 19, col: 0}
  WHILE_DO {line: 1/19} {freq: 0, ln: 19, col: 0}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19} {freq: 0, ln: 19, col: 0}
  LABEL L1026 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21} {freq: 0, ln: 21, col: 0}
   WHILE_DO {line: 1/21} {freq: 0, ln: 21, col: 0}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21} {freq: 0, ln: 21, col: 0}
   LABEL L1538 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
         I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8INTCONST 128 (0x80)
         U8MPY
        U8ADD
         I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
         U8INTCONST 2048 (0x800)
        U8MPY
       U8ADD
       U8INTCONST 4 (0x4)
      U8MPY
     U8ADD
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22} {freq: 0, ln: 22, col: 0}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21} {freq: 0, ln: 21, col: 0}
   LABEL L1282 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
    END_BLOCK
   GOTO L1794 {line: 1/21} {freq: 0, ln: 21, col: 0}
  LABEL L1794 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19} {freq: 0, ln: 19, col: 0}
  LABEL L770 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
   END_BLOCK
  GOTO L2050 {line: 1/19} {freq: 0, ln: 19, col: 0}
 LABEL L2050 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17} {freq: 0, ln: 17, col: 0}
 LABEL L258 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
  END_BLOCK
 GOTO L2306 {line: 1/17} {freq: 0, ln: 17, col: 0}
LABEL L2306 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IF {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
   U8PARM 2 T<57,anon_ptr.,8> #  by_value 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    U4INTCONST 0 (0x0)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    I4INTCONST -2 (0xfffffffffffffffe)
   I4PARM 2 T<4,.predef_I4,4> #  by_value 
  VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_IF
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IF {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_IF
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
LABEL L8962 0 {line: 1/48} {freq: 0, ln: 48, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48} {freq: 0, ln: 48, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49} {freq: 0, ln: 49, col: 0}
 WHILE_DO {line: 1/49} {freq: 0, ln: 49, col: 0}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49} {freq: 0, ln: 49, col: 0}
 LABEL L2818 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51} {freq: 0, ln: 51, col: 0}
  WHILE_DO {line: 1/51} {freq: 0, ln: 51, col: 0}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51} {freq: 0, ln: 51, col: 0}
  LABEL L3330 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53} {freq: 0, ln: 53, col: 0}
   WHILE_DO {line: 1/53} {freq: 0, ln: 53, col: 0}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53} {freq: 0, ln: 53, col: 0}
   LABEL L3842 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
    IF {line: 1/55} {freq: 0, ln: 55, col: 0}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
           I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
            I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
            I8INTCONST 128 (0x80)
           U8MPY
          U8ADD
           I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
           U8INTCONST 2048 (0x800)
          U8MPY
         U8ADD
         U8INTCONST 4 (0x4)
        U8MPY
       U8ADD
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
             I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
              I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
              I8INTCONST 128 (0x80)
             U8MPY
            U8ADD
             I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
             U8INTCONST 2048 (0x800)
            U8MPY
           U8ADD
           U8INTCONST 4 (0x4)
          U8MPY
         U8ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55} {freq: 0, ln: 55, col: 0}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55} {freq: 0, ln: 55, col: 0}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55} {freq: 0, ln: 55, col: 0}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53} {freq: 0, ln: 53, col: 0}
   LABEL L3586 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
    END_BLOCK
   GOTO L4098 {line: 1/53} {freq: 0, ln: 53, col: 0}
  LABEL L4098 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51} {freq: 0, ln: 51, col: 0}
  LABEL L3074 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
   END_BLOCK
  GOTO L4354 {line: 1/51} {freq: 0, ln: 51, col: 0}
 LABEL L4354 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49} {freq: 0, ln: 49, col: 0}
 LABEL L2562 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
  END_BLOCK
 GOTO L4610 {line: 1/49} {freq: 0, ln: 49, col: 0}
LABEL L4610 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
 IF {line: 1/61} {freq: 0, ln: 61, col: 0}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61} {freq: 0, ln: 61, col: 0}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61} {freq: 0, ln: 61, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61} {freq: 0, ln: 61, col: 0}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61} {freq: 0, ln: 61, col: 0}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68} {freq: 0, ln: 68, col: 0}
 RETURN {line: 1/68} {freq: 0, ln: 68, col: 0}
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After wn_unroll"
FUNC_ENTRY <1,50,main> {line: 1/13} {freq: 0, ln: 13, col: 0}
BODY
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 1/13} {freq: 0, ln: 13, col: 0}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13} {freq: 0, ln: 13, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17} {freq: 0, ln: 17, col: 0}
 WHILE_DO {line: 1/17} {freq: 0, ln: 17, col: 0}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/17} {freq: 0, ln: 17, col: 0}
 LABEL L514 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19} {freq: 0, ln: 19, col: 0}
  WHILE_DO {line: 1/19} {freq: 0, ln: 19, col: 0}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/19} {freq: 0, ln: 19, col: 0}
  LABEL L1026 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21} {freq: 0, ln: 21, col: 0}
   WHILE_DO {line: 1/21} {freq: 0, ln: 21, col: 0}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/21} {freq: 0, ln: 21, col: 0}
   LABEL L1538 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
       I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
        I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
        I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
       I4ADD
      I4ADD
      I4INTCONST 10 (0xa)
     I4REM
      U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
         I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
          I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
          I8INTCONST 128 (0x80)
         U8MPY
        U8ADD
         I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
         U8INTCONST 2048 (0x800)
        U8MPY
       U8ADD
       U8INTCONST 4 (0x4)
      U8MPY
     U8ADD
    I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22} {freq: 0, ln: 22, col: 0}
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/21} {freq: 0, ln: 21, col: 0}
   LABEL L1282 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
    END_BLOCK
   GOTO L1794 {line: 1/21} {freq: 0, ln: 21, col: 0}
  LABEL L1794 0 {line: 1/21} {freq: 0, ln: 21, col: 0}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/19} {freq: 0, ln: 19, col: 0}
  LABEL L770 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
   END_BLOCK
  GOTO L2050 {line: 1/19} {freq: 0, ln: 19, col: 0}
 LABEL L2050 0 {line: 1/19} {freq: 0, ln: 19, col: 0}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/17} {freq: 0, ln: 17, col: 0}
 LABEL L258 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
  END_BLOCK
 GOTO L2306 {line: 1/17} {freq: 0, ln: 17, col: 0}
LABEL L2306 0 {line: 1/17} {freq: 0, ln: 17, col: 0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IF {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
   U8PARM 2 T<57,anon_ptr.,8> #  by_value 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    U4INTCONST 0 (0x0)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
    I4INTCONST -2 (0xfffffffffffffffe)
   I4PARM 2 T<4,.predef_I4,4> #  by_value 
  VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_IF
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IF {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4I4LDID 0 <2,6,__acch_temp__is_pcreate> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
   U8PARM 2 T<53,.anonymous.1,16> #  by_value 
    U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
   U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
    U4INTCONST 65536 (0x10000)
   U4PARM 2 T<8,.predef_U4,4> #  by_value 
  VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_IF
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   I4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0} {freq: 0, ln: 0, col: 0}
LABEL L8962 0 {line: 1/48} {freq: 0, ln: 48, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/48} {freq: 0, ln: 48, col: 0}
  I4INTCONST 0 (0x0)
 I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49} {freq: 0, ln: 49, col: 0}
 WHILE_DO {line: 1/49} {freq: 0, ln: 49, col: 0}
 COMP
   I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 1/49} {freq: 0, ln: 49, col: 0}
 LABEL L2818 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
   I4INTCONST 0 (0x0)
  I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51} {freq: 0, ln: 51, col: 0}
  WHILE_DO {line: 1/51} {freq: 0, ln: 51, col: 0}
  COMP
    I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 1/51} {freq: 0, ln: 51, col: 0}
  LABEL L3330 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
    I4INTCONST 0 (0x0)
   I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53} {freq: 0, ln: 53, col: 0}
   WHILE_DO {line: 1/53} {freq: 0, ln: 53, col: 0}
   COMP
     I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 1/53} {freq: 0, ln: 53, col: 0}
   LABEL L3842 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
    IF {line: 1/55} {freq: 0, ln: 55, col: 0}
        U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
           I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
            I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
            I8INTCONST 128 (0x80)
           U8MPY
          U8ADD
           I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
           U8INTCONST 2048 (0x800)
          U8MPY
         U8ADD
         U8INTCONST 4 (0x4)
        U8MPY
       U8ADD
      I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
          U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
             I8I4LDID 0 <2,1,i> T<4,.predef_I4,4>
              I8I4LDID 0 <2,2,j> T<4,.predef_I4,4>
              I8INTCONST 128 (0x80)
             U8MPY
            U8ADD
             I8I4LDID 0 <2,3,k> T<4,.predef_I4,4>
             U8INTCONST 2048 (0x800)
            U8MPY
           U8ADD
           U8INTCONST 4 (0x4)
          U8MPY
         U8ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
     I4I4NE
    THEN
     BLOCK {line: 1/55} {freq: 0, ln: 55, col: 0}
       I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
       I4INTCONST 1 (0x1)
      I4ADD
     I4STID 0 <2,4,error> T<4,.predef_I4,4> {line: 1/55} {freq: 0, ln: 55, col: 0}
     END_BLOCK
    ELSE
     BLOCK {line: 1/55} {freq: 0, ln: 55, col: 0}
     END_BLOCK
    END_IF
      I4I4LDID 0 <2,1,i> T<4,.predef_I4,4>
      I4INTCONST 1 (0x1)
     I4ADD
    I4STID 0 <2,1,i> T<4,.predef_I4,4> {line: 1/53} {freq: 0, ln: 53, col: 0}
   LABEL L3586 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
    END_BLOCK
   GOTO L4098 {line: 1/53} {freq: 0, ln: 53, col: 0}
  LABEL L4098 0 {line: 1/53} {freq: 0, ln: 53, col: 0}
     I4I4LDID 0 <2,2,j> T<4,.predef_I4,4>
     I4INTCONST 1 (0x1)
    I4ADD
   I4STID 0 <2,2,j> T<4,.predef_I4,4> {line: 1/51} {freq: 0, ln: 51, col: 0}
  LABEL L3074 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
   END_BLOCK
  GOTO L4354 {line: 1/51} {freq: 0, ln: 51, col: 0}
 LABEL L4354 0 {line: 1/51} {freq: 0, ln: 51, col: 0}
    I4I4LDID 0 <2,3,k> T<4,.predef_I4,4>
    I4INTCONST 1 (0x1)
   I4ADD
  I4STID 0 <2,3,k> T<4,.predef_I4,4> {line: 1/49} {freq: 0, ln: 49, col: 0}
 LABEL L2562 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
  END_BLOCK
 GOTO L4610 {line: 1/49} {freq: 0, ln: 49, col: 0}
LABEL L4610 0 {line: 1/49} {freq: 0, ln: 49, col: 0}
 IF {line: 1/61} {freq: 0, ln: 61, col: 0}
   I4I4LDID 0 <2,4,error> T<4,.predef_I4,4>
   I4INTCONST 0 (0x0)
  I4I4EQ
 THEN
  BLOCK {line: 1/61} {freq: 0, ln: 61, col: 0}
    U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61} {freq: 0, ln: 61, col: 0}
  END_BLOCK
 ELSE
  BLOCK {line: 1/61} {freq: 0, ln: 61, col: 0}
    U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
   U8PARM 2 T<58,anon_ptr.,8> #  by_value 
  VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61} {freq: 0, ln: 61, col: 0}
  END_BLOCK
 END_IF
  I4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68} {freq: 0, ln: 68, col: 0}
 RETURN {line: 1/68} {freq: 0, ln: 68, col: 0}
 END_BLOCK
=======================================================================
!!! DevWarn during Global Optimization -- Create CFG: Should use ST_pu_type instead
=======================================================================
WN_Lower: "Lowering to CG"
flags are:
LOWER_DO_LOOP LOWER_DO_WHILE LOWER_WHILE_DO LOWER_IF LOWER_COMPLEX LOWER_ARRAY LOWER_CALL LOWER_IO_STATEMENT LOWER_MSTORE LOWER_CVT LOWER_MP LOWER_COMPGOTO LOWER_MADD LOWER_INTRINSIC LOWER_INLINE_INTRINSIC <unrecognized> LOWER_INL_STACK_INTRINSIC LOWER_PREFETCH_MAPS LOWER_ALIAS_MAPS LOWER_DEPGRAPH_MAPS LOWER_PARITY_MAPS LOWER_PICCALL LOWER_ASSERT LOWER_FREQUENCY_MAPS LOWER_FORMAL_REF LOWER_UPLEVEL LOWER_ENTRY_FORMAL_REF LOWER_SHORTCIRCUIT LOWER_MLDID_MSTID LOWER_BIT_FIELD_ID <unrecognized> LOWER_TO_MEMLIB <unrecognized> <unrecognized> LOWER_TO_CG <unrecognized> 
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
   U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
   U4INTCONST 508 (0x1fc)
  U8ADD
 U8STID 72 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 72 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 0/0}
LABEL L10498 0 {line: 0/0}
  LOOP_INFO 0 1 0
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 8 (0x8)
  END_LOOP_INFO
  I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
 I4STID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 0/0}
  U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 0/0}
LABEL L11266 0 {line: 0/0}
  LOOP_INFO 0 2 0
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 16 (0x10)
  END_LOOP_INFO
  I4I4LDID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
 I4STID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 0/0}
  U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
LABEL L12034 0 {line: 0/0}
  LOOP_INFO 0 3 1
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 128 (0x80)
  END_LOOP_INFO
   I4I4LDID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
   U4INTCONST 10 (0xa)
  I4REM
  U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
   I4I4LDID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 1/21}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 4 (0x4)
  U8ADD
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/21}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
  I4I8LE
 TRUEBR L12034 {line: 0/0}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 1/19}
   U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/19}
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/19}
   I4I4LDID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 1/19}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 15 (0xf)
  I4I4LE
 TRUEBR L11266 {line: 0/0}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 1/17}
   U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/17}
   U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/17}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 7 (0x7)
  I4I4LE
 TRUEBR L10498 {line: 0/0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate {line: 0/0}
   I4I4LDID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate
   U4INTCONST 0 (0x0)
  I4I4EQ
 FALSEBR L17154 {line: 0/0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0}
LABEL L17154 0 {line: 0/0}
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate {line: 0/0}
   I4I4LDID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate
   U4INTCONST 0 (0x0)
  I4I4EQ
 FALSEBR L17410 {line: 0/0}
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
LABEL L17410 0 {line: 0/0}
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0}
   U4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0}
   U4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0}
   U4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0}
   U4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
  U4INTCONST 0 (0x0)
 U8STID 64 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 72 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 0/0}
LABEL L14082 0 {line: 0/0}
  LOOP_INFO 0 1 0
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 8 (0x8)
  END_LOOP_INFO
  U8U8LDID 64 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 75 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
   U8U8LDID 75 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
  U8ADD
 U8STID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 0/0}
LABEL L14850 0 {line: 0/0}
  LOOP_INFO 0 2 0
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 16 (0x10)
  END_LOOP_INFO
  U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
LABEL L15618 0 {line: 0/0}
  LOOP_INFO 0 3 1
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 128 (0x80)
  END_LOOP_INFO
    U8U8LDID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
      U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
     I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
     U4INTCONST 3 (0x3)
    I4MPY
    U4INTCONST 2 (0x2)
   I4ADD
  I4I4NE
 FALSEBR L17666 {line: 1/55}
   I4I4LDID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error {line: 1/55}
LABEL L17666 0 {line: 1/53}
   U8U8LDID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 4 (0x4)
  U8ADD
 U8STID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/53}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 4 (0x4)
  U8ADD
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/53}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
  I4I8LE
 TRUEBR L15618 {line: 0/0}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 1/51}
   U8U8LDID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/51}
   U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/51}
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/51}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 15 (0xf)
  I4I4LE
 TRUEBR L14850 {line: 0/0}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 1/49}
   U8U8LDID 75 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 64 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/49}
   U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/49}
   U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/49}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 7 (0x7)
  I4I4LE
 TRUEBR L14082 {line: 0/0}
   I4I4LDID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error
   U4INTCONST 0 (0x0)
  I4I4EQ
 FALSEBR L16898 {line: 1/61}
   U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
  U8PARM 2 T<58,anon_ptr.,8> #  by_value 
 VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
 GOTO L17922 {line: 1/61}
LABEL L16898 0 {line: 1/61}
   U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
  U8PARM 2 T<58,anon_ptr.,8> #  by_value 
 VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
LABEL L17922 0 {line: 1/68}
  U4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68}
 RETURN {line: 1/68}
 END_BLOCK
=======================================================================
!!! DevWarn during Lowering: lower_call(): line 0, parm #0 type mismatch (WN_rtype(parm) = U8) (cannonical TY_mtype(parm))) M)
!!! DevWarn during Lowering: lower_call(): line 0, parm #0 type mismatch (WN_rtype(parm) = U8) (cannonical TY_mtype(parm))) M)
!!! DevWarn during Lowering: lower_call(): line 0, parm #0 type mismatch (WN_rtype(parm) = U8) (cannonical TY_mtype(parm))) M)
!!! DevWarn during Lowering: lower_call(): line 0, parm #0 type mismatch (WN_rtype(parm) = U8) (cannonical TY_mtype(parm))) M)
!!! DevWarn during Lowering: lower_call(): line 0, parm #0 type mismatch (WN_rtype(parm) = U8) (cannonical TY_mtype(parm))) M)
!!! DevWarn during Lowering: lower_call(): line 0, parm #1 type mismatch (WN_rtype(parm) = U8) (cannonical TY_mtype(parm))) M)
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,50,main> {line: 1/13}
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 1/13}
 PRAGMA 0 120 <null-st> 0 (0x0) # PREAMBLE_END {line: 1/13}
  U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8LDA 508 <1,51,x> T<56,anon_ptr.,8>
 U8STID 72 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 72 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 0/0}
LABEL L10498 0 {line: 0/0}
  LOOP_INFO 0 1 0
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 8 (0x8)
  END_LOOP_INFO
  I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
 I4STID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 0/0}
  U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 0/0}
LABEL L11266 0 {line: 0/0}
  LOOP_INFO 0 2 0
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 16 (0x10)
  END_LOOP_INFO
  I4I4LDID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
 I4STID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 0/0}
  U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
LABEL L12034 0 {line: 0/0}
  LOOP_INFO 0 3 1
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 128 (0x80)
  END_LOOP_INFO
   I4I4LDID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
   U4INTCONST 10 (0xa)
  I4REM
  U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/22}
   I4I4LDID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 53 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 1/21}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 4 (0x4)
  U8ADD
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/21}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
  I4I8LE
 TRUEBR L12034 {line: 0/0}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 1/19}
   U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/19}
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/19}
   I4I4LDID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg>
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 49 <1,4,.preg_I4> T<4,.predef_I4,4> # <preg> {line: 1/19}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 15 (0xf)
  I4I4LE
 TRUEBR L11266 {line: 0/0}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 1/17}
   U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/17}
   U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/17}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 7 (0x7)
  I4I4LE
 TRUEBR L10498 {line: 0/0}
  U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
 U8STID 0 <2,15,.SP> T<53,.anonymous.1,16> {line: 0/0}
  U4INTCONST 0 (0x0)
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 6 <1,5,.preg_I8> T<5,.predef_I8,8> # $r6 {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 7 <1,5,.preg_I8> T<5,.predef_I8,8> # $r7 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate {line: 0/0}
   I4I4LDID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate
   U4INTCONST 0 (0x0)
  I4I4EQ
 FALSEBR L17154 {line: 0/0}
  U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
 U8STID 0 <2,15,.SP> T<53,.anonymous.1,16> {line: 0/0}
  U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 6 <1,5,.preg_I8> T<5,.predef_I8,8> # $r6 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
  U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
 U8STID 0 <2,15,.SP> T<53,.anonymous.1,16> {line: 0/0}
  U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 6 <1,5,.preg_I8> T<5,.predef_I8,8> # $r6 {line: 0/0}
  U4INTCONST 0 (0x0)
 I8STID 7 <1,5,.preg_I8> T<5,.predef_I8,8> # $r7 {line: 0/0}
  I4INTCONST -2 (0xfffffffffffffffe)
 I8STID 8 <1,5,.preg_I8> T<5,.predef_I8,8> # $r8 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <1,51,x> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,59,__accr_memin_h2d> # flags 0x76 {line: 0/0}
LABEL L17154 0 {line: 0/0}
  U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
 U8STID 0 <2,15,.SP> T<53,.anonymous.1,16> {line: 0/0}
  U4INTCONST 0 (0x0)
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 6 <1,5,.preg_I8> T<5,.predef_I8,8> # $r6 {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 7 <1,5,.preg_I8> T<5,.predef_I8,8> # $r7 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 I4CALL 118 <1,57,__accr_present_create> # flags 0x76 {line: 0/0}
  I4I4LDID 1 <1,49,.preg_return_val> T<4,.predef_I4,4> # $r1
 I4STID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate {line: 0/0}
   I4I4LDID 76 <1,4,.preg_I4> T<4,.predef_I4,4> # __acch_temp__is_pcreate
   U4INTCONST 0 (0x0)
  I4I4EQ
 FALSEBR L17410 {line: 0/0}
  U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
 U8STID 0 <2,15,.SP> T<53,.anonymous.1,16> {line: 0/0}
  U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 6 <1,5,.preg_I8> T<5,.predef_I8,8> # $r6 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
 VCALL 118 <1,58,__accr_malloc_on_device> # flags 0x76 {line: 0/0}
LABEL L17410 0 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
 VCALL 118 <1,60,__accr_set_default_gang_vector> # flags 0x76 {line: 0/0}
  U4INTCONST 8 (0x8)
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U4INTCONST 8 (0x8)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,61,__accr_set_gang_num_x> # flags 0x76 {line: 0/0}
  U4INTCONST 16 (0x10)
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U4INTCONST 16 (0x10)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,62,__accr_set_gang_num_y> # flags 0x76 {line: 0/0}
  U4INTCONST 2 (0x2)
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U4INTCONST 2 (0x2)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,63,__accr_set_vector_num_y> # flags 0x76 {line: 0/0}
  U4INTCONST 128 (0x80)
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U4INTCONST 128 (0x80)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,64,__accr_set_vector_num_x> # flags 0x76 {line: 0/0}
  U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8LDA 0 <2,7,__device_x> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
  U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8LDA 0 <2,8,__device_y> T<67,anon_ptr.,8>
  U8PARM 1 T<67,anon_ptr.,8> #  by_reference 
 VCALL 118 <1,66,__accr_push_kernel_param_pointer> # flags 0x76 {line: 0/0}
  U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
 I8STID 6 <1,5,.preg_I8> T<5,.predef_I8,8> # $r6 {line: 0/0}
  I4INTCONST -2 (0xfffffffffffffffe)
 I8STID 7 <1,5,.preg_I8> T<5,.predef_I8,8> # $r7 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8LDA 0 <1,68,(17_bytes)_"__accrg_main_1_1\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   U8LDA 0 <1,69,(20_bytes)_"multi_array.w2c.ptx\000"> T<70,anon_ptr.,8>
  U8PARM 2 T<70,anon_ptr.,8> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 16502 <1,67,__accr_launchkernel> # flags 0x4076 {line: 0/0}
  U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
 U8STID 0 <2,15,.SP> T<53,.anonymous.1,16> {line: 0/0}
  U4INTCONST 65536 (0x10000)
 I8STID 6 <1,5,.preg_I8> T<5,.predef_I8,8> # $r6 {line: 0/0}
  U4INTCONST 0 (0x0)
 I8STID 7 <1,5,.preg_I8> T<5,.predef_I8,8> # $r7 {line: 0/0}
  I4INTCONST -2 (0xfffffffffffffffe)
 I8STID 8 <1,5,.preg_I8> T<5,.predef_I8,8> # $r8 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
   U8U8LDID 0 <1,52,y> T<53,.anonymous.1,16>
  U8PARM 2 T<53,.anonymous.1,16> #  by_value 
   U4INTCONST 65536 (0x10000)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   U4INTCONST 0 (0x0)
  U4PARM 2 T<8,.predef_U4,4> #  by_value 
   I4INTCONST -2 (0xfffffffffffffffe)
  I4PARM 2 T<4,.predef_I4,4> #  by_value 
 VCALL 118 <1,70,__accr_memout_d2h> # flags 0x76 {line: 0/0}
  U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <2,7,__device_x> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
  U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 0/0}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 0/0}
   U8U8LDID 0 <2,8,__device_y> T<57,anon_ptr.,8>
  U8PARM 2 T<57,anon_ptr.,8> #  by_value 
 VCALL 118 <1,71,__accr_free_on_device> # flags 0x76 {line: 0/0}
  U4INTCONST 0 (0x0)
 U8STID 64 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8LDA 0 <1,51,x> T<56,anon_ptr.,8>
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 72 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 0/0}
LABEL L14082 0 {line: 0/0}
  LOOP_INFO 0 1 0
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 8 (0x8)
  END_LOOP_INFO
  U8U8LDID 64 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 75 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
   U8U8LDID 75 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U8LDA 0 <1,52,y> T<56,anon_ptr.,8>
  U8ADD
 U8STID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U4INTCONST 0 (0x0)
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 0/0}
LABEL L14850 0 {line: 0/0}
  LOOP_INFO 0 2 0
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 16 (0x10)
  END_LOOP_INFO
  U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
  U8U8LDID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
 U8STID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 0/0}
LABEL L15618 0 {line: 0/0}
  LOOP_INFO 0 3 1
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 128 (0x80)
  END_LOOP_INFO
    U8U8LDID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
      U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
     I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
     U4INTCONST 3 (0x3)
    I4MPY
    U4INTCONST 2 (0x2)
   I4ADD
  I4I4NE
 FALSEBR L17666 {line: 1/55}
   I4I4LDID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error {line: 1/55}
LABEL L17666 0 {line: 1/53}
   U8U8LDID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 4 (0x4)
  U8ADD
 U8STID 73 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/53}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 4 (0x4)
  U8ADD
 U8STID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/53}
   U8U8LDID 67 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
  I4I8LE
 TRUEBR L15618 {line: 0/0}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j {line: 1/51}
   U8U8LDID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 74 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/51}
   U8U8LDID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 68 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/51}
   U8U8LDID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 512 (0x200)
  U8ADD
 U8STID 70 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/51}
   I4I4LDID 78 <1,4,.preg_I4> T<4,.predef_I4,4> # j
   U4INTCONST 15 (0xf)
  I4I4LE
 TRUEBR L14850 {line: 0/0}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 1 (0x1)
  I4ADD
 I4STID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k {line: 1/49}
   U8U8LDID 75 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 64 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/49}
   U8U8LDID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 69 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/49}
   U8U8LDID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg>
   U4INTCONST 8192 (0x2000)
  U8ADD
 U8STID 71 <1,9,.preg_U8> T<9,.predef_U8,8> # <preg> {line: 1/49}
   I4I4LDID 79 <1,4,.preg_I4> T<4,.predef_I4,4> # k
   U4INTCONST 7 (0x7)
  I4I4LE
 TRUEBR L14082 {line: 0/0}
   I4I4LDID 77 <1,4,.preg_I4> T<4,.predef_I4,4> # error
   U4INTCONST 0 (0x0)
  I4I4EQ
 FALSEBR L16898 {line: 1/61}
  U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 1/61}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 1/61}
   U8LDA 0 <1,54,(15_bytes)_"Test_success!\n\000"> T<62,anon_ptr.,8>
  U8PARM 2 T<58,anon_ptr.,8> #  by_value 
 VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
 GOTO L17922 {line: 1/61}
LABEL L16898 0 {line: 1/61}
  U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
 I8STID 5 <1,5,.preg_I8> T<5,.predef_I8,8> # $r5 {line: 1/61}
  I4INTCONST 0 (0x0)
 I4STID 1 <1,4,.preg_I4> T<4,.predef_I4,4> # $r1 {line: 1/61}
   U8LDA 0 <1,55,(14_bytes)_"Test_failed!\n\000"> T<64,anon_ptr.,8>
  U8PARM 2 T<58,anon_ptr.,8> #  by_value 
 VCALL 126 <1,53,printf> # flags 0x7e {line: 1/61}
LABEL L17922 0 {line: 1/68}
  U4INTCONST 0 (0x0)
 I4STID 1 <1,5,.preg_I8> T<4,.predef_I4,4> # $r1 {line: 1/68}
 RETURN {line: 1/68}
 END_BLOCK
=======================================================================
!!! DevWarn during Assembly: Redundant Copy instruction in BB:40 (PC=0x64)
[  22, 9] TN250(%rsi) :- mov64 TN248(%rsi) ; copy
!!! DevWarn during Assembly: Redundant Copy instruction in BB:40 (PC=0x74)
[  22,11] TN249(%r8) :- mov64 TN247(%r8) ; copy
!!! DevWarn during Assembly: Redundant Copy instruction in BB:8 (PC=0x104)
[  17, 0] TN239(%rax) :- mov64 GTN1(%rax) ; copy
!!! DevWarn during Assembly: Redundant Copy instruction in BB:12 (PC=0x12c)
[  17, 0] TN240(%rax) :- mov64 GTN1(%rax) ; copy
=======================================================================
WN_Lower: "Fast exponents lowering"
flags are:
<unrecognized> 
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0}
 WHILE_DO {line: 0/0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
  WHILE_DO {line: 0/0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
   WHILE_DO {line: 0/0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0}
    IF {line: 0/0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0}
 WHILE_DO {line: 0/0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
  WHILE_DO {line: 0/0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
   WHILE_DO {line: 0/0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0}
    IF {line: 0/0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "W2C Lowering"
flags are:
LOWER_MP <unrecognized> 
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0}
 WHILE_DO {line: 0/0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
  WHILE_DO {line: 0/0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
   WHILE_DO {line: 0/0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0}
    IF {line: 0/0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0}
 WHILE_DO {line: 0/0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
  WHILE_DO {line: 0/0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
   WHILE_DO {line: 0/0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0}
    IF {line: 0/0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "Before ACC Lowering"
flags are:
<unrecognized> 
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0}
 WHILE_DO {line: 0/0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
  WHILE_DO {line: 0/0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
   WHILE_DO {line: 0/0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0}
    IF {line: 0/0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
 END_BLOCK
 BLOCK {line: 0/0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0}
 WHILE_DO {line: 0/0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
  WHILE_DO {line: 0/0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
   WHILE_DO {line: 0/0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0}
    IF {line: 0/0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "Pre_Opt"
flags are:
LOWER_COMPLEX LOWER_ARRAY LOWER_ENTRY_EXIT LOWER_IO_STATEMENT LOWER_INLINE_INTRINSIC LOWER_PREFETCH_MAPS LOWER_ALIAS_MAPS LOWER_DEPGRAPH_MAPS LOWER_PARITY_MAPS LOWER_BASE_INDEX LOWER_FREQUENCY_MAPS LOWER_UPLEVEL LOWER_SHORTCIRCUIT LOWER_BIT_FIELD_ID LOWER_TO_MEMLIB <unrecognized> 
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
  WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    IF {line: 0/0} {freq: 0, ln: 0, col: 0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43} {freq: 0, ln: 43, col: 0}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43} {freq: 0, ln: 43, col: 0}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After lowering"
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  U8U8LDID 5 <1,9,.preg_U8> T<9,.predef_U8,8> # $r5
 U8STID 0 <3,1,__d_x> T<57,anon_ptr.,8> {line: 0/0} {freq: 0, ln: 0, col: 0}
  U8U8LDID 6 <1,9,.preg_U8> T<9,.predef_U8,8> # $r6
 U8STID 0 <3,2,__d_y> T<57,anon_ptr.,8> {line: 0/0} {freq: 0, ln: 0, col: 0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
  WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    IF {line: 0/0} {freq: 0, ln: 0, col: 0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43} {freq: 0, ln: 43, col: 0}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43} {freq: 0, ln: 43, col: 0}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_BLOCK
=======================================================================
=======================================================================
WN_Lower: "After wn_unroll"
FUNC_ENTRY <1,65,__accrg_main_1_1> {line: 0/0} {freq: 0, ln: 0, col: 0}
 IDNAME 0 <3,1,__d_x>
 IDNAME 0 <3,2,__d_y>
BODY
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
 END_BLOCK
 BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
  U8U8LDID 5 <1,9,.preg_U8> T<9,.predef_U8,8> # $r5
 U8STID 0 <3,1,__d_x> T<57,anon_ptr.,8> {line: 0/0} {freq: 0, ln: 0, col: 0}
  U8U8LDID 6 <1,9,.preg_U8> T<9,.predef_U8,8> # $r6
 U8STID 0 <3,2,__d_y> T<57,anon_ptr.,8> {line: 0/0} {freq: 0, ln: 0, col: 0}
  U4U4LDID 0 <3,6,__nv50_blockIdx_x> T<8,.predef_U4,4>
 I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   U4U4LDID 0 <3,13,__nv50_griddim_y> T<8,.predef_U4,4>
  U4MPY
 U4STID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
 WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
 COMP
   I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
   I4INTCONST 7 (0x7)
  I4I4LE
 BODY
  BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U4U4LDID 0 <3,7,__nv50_blockIdx_y> T<8,.predef_U4,4>
    U4U4LDID 0 <3,10,__nv50_blockdim_y> T<8,.predef_U4,4>
   I4MPY
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    U4U4LDID 0 <3,4,__nv50_threadIdx_y> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
  WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
  COMP
    I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
    I4INTCONST 15 (0xf)
   I4I4LE
  BODY
   BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    U4U4LDID 0 <3,3,__nv50_threadIdx_x> T<8,.predef_U4,4>
   I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   WHILE_DO {line: 0/0} {freq: 0, ln: 0, col: 0}
   COMP
     I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
     I4INTCONST 127 (0x7f)
    I4I4LE
   BODY
    BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
    IF {line: 0/0} {freq: 0, ln: 0, col: 0}
       I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
       I4INTCONST 0 (0x0)
      I4I4GE
        I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
        I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
        I4INTCONST 0 (0x0)
       I4I4GE
      I4LAND
     I4LAND
    THEN
     BLOCK {line: 0/0} {freq: 0, ln: 0, col: 0}
          U8U8LDID 0 <3,1,__d_x> T<57,anon_ptr.,8>
            I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
              I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
              I4INTCONST 2048 (0x800)
             I4MPY
              I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
              I4INTCONST 128 (0x80)
             I4MPY
            I4ADD
           I4ADD
           U4INTCONST 4 (0x4)
          U4MPY
         U4ADD
        I4I4ILOAD 0 T<4,.predef_I4,4> T<57,anon_ptr.,8>
        I4INTCONST 3 (0x3)
       I4MPY
       I4INTCONST 2 (0x2)
      I4ADD
       U8U8LDID 0 <3,2,__d_y> T<57,anon_ptr.,8>
         I8I4LDID 0 <3,17,i> T<4,.predef_I4,4>
           I8I4LDID 0 <3,15,k> T<4,.predef_I4,4>
           I4INTCONST 2048 (0x800)
          I4MPY
           I8I4LDID 0 <3,16,j> T<4,.predef_I4,4>
           I4INTCONST 128 (0x80)
          I4MPY
         I4ADD
        I4ADD
        U4INTCONST 4 (0x4)
       U4MPY
      U4ADD
     I4ISTORE 0 T<57,anon_ptr.,8> {line: 1/43} {freq: 0, ln: 43, col: 0}
     END_BLOCK
    ELSE
     BLOCK {line: 1/43} {freq: 0, ln: 43, col: 0}
     END_BLOCK
    END_IF
      I4I4LDID 0 <3,17,i> T<4,.predef_I4,4>
      U4U4LDID 0 <3,9,__nv50_blockdim_x> T<8,.predef_U4,4>
     I4ADD
    I4STID 0 <3,17,i> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
    END_BLOCK
     I4I4LDID 0 <3,16,j> T<4,.predef_I4,4>
     U4U4LDID 0 <3,18,__acc_tmp_0> T<8,.predef_U4,4>
    I4ADD
   I4STID 0 <3,16,j> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
   END_BLOCK
    I4I4LDID 0 <3,15,k> T<4,.predef_I4,4>
    U4U4LDID 0 <3,12,__nv50_griddim_x> T<8,.predef_U4,4>
   I4ADD
  I4STID 0 <3,15,k> T<4,.predef_I4,4> {line: 0/0} {freq: 0, ln: 0, col: 0}
  END_BLOCK
 END_BLOCK
=======================================================================
!!! DevWarn during Global Optimization -- Create CFG: Count limit reached on the following DevWarn:
!!! DevWarn during Global Optimization -- Create CFG: Should use ST_pu_type instead
