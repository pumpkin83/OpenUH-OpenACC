/*
 * Copyright 2005, 2006 PathScale, Inc.  All Rights Reserved.
 */

/* config.h.  Generated by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Define to 1 if you have the <alloca.h> header file. */
#define HAVE_ALLOCA_H 1

/* Define 1 if want producer to build with 32/64bit section offsets per dwarf3
   */
/* #undef HAVE_DWARF2_99_EXTENSION */

/* Define to 1 if the elf64_getehdr function is in libelf.a. */
#if defined(BUILD_OS_DARWIN)
#undef HAVE_ELF64_GETEHDR
#else /* ! defined(BUILD_OS_DARWIN) */
#define HAVE_ELF64_GETEHDR 1
#endif /* ! defined(BUILD_OS_DARWIN) */

/* Define to 1 if the elf64_getshdr function is in libelf.a. */
#if defined(BUILD_OS_DARWIN)
#undef HAVE_ELF64_GETSHDR
#else /* ! defined(BUILD_OS_DARWIN) */
#define HAVE_ELF64_GETSHDR 1
#endif /* ! defined(BUILD_OS_DARWIN) */

/* Define to 1 if you have the <elfaccess.h> header file. */
/* #undef HAVE_ELFACCESS_H */

/* Define to 1 if you have the <elf.h> header file. */
#if defined(BUILD_OS_DARWIN)
#undef HAVE_ELF_H
#else /* KEY Mac port */
#define HAVE_ELF_H 1
#endif /* KEY Mac port */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <libelf.h> header file. */
/* #undef HAVE_LIBELF_H */

/* Define to 1 if you have the <libelf/libelf.h> header file. */
#define HAVE_LIBELF_LIBELF_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define 1 if want producer to build with only 32bit section offsets per
   strict dwarf2 */
#define HAVE_OLD_DWARF2_32BIT_OFFSET 1

/* Define 1 if R_IA_64_DIR32LSB is defined (might be enum value). */
#define HAVE_R_IA64_DIR32LSB 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/ia64/elf.h> header file. */
/* #undef HAVE_SYS_IA64_ELF_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* See if __uint32_t is predefined in the compiler. */
/* #undef HAVE___UINT32_T */

/* Define 1 if __uint32_t is in sgidefs.h. */
/* #undef HAVE___UINT32_T_IN_SGIDEFS_H */
#ifdef __MINGW32__
#define HAVE___UINT32_T_IN_SGIDEFS_H 1
#endif /* __MINGW32__ */

/* Define 1 if sys/types.h defines __uint32_t. */
#define HAVE___UINT32_T_IN_SYS_TYPES_H 1

/* See if __uint64_t is predefined in the compiler. */
/* #undef HAVE___UINT64_T */

/* Define 1 if __uint64_t is in sgidefs.h. */
/* #undef HAVE___UINT64_T_IN_SGIDEFS_H */
#ifdef __MINGW32__
#define HAVE___UINT64_T_IN_SGIDEFS_H 1
#endif /* __MINGW32__ */

/* Define 1 if sys/types.h defines __uint64_t. */
#define HAVE___UINT64_T_IN_SYS_TYPES_H 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */
