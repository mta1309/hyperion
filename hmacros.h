/* HMACROS.H    (c) Copyright Roger Bowler, 1999-2012                */
/*               Hercules macros...                                  */
/*                                                                   */
/*   Released under "The Q Public License Version 1"                 */
/*   (http://www.hercules-390.org/herclic.html) as modifications to  */
/*   Hercules.                                                       */

//      This header auto-#included by 'hercules.h'...
//
//      The <config.h> header and other required headers are
//      presumed to have already been #included ahead of it...


#ifndef _HMACROS_H
#define _HMACROS_H

#include "hercules.h"

/*-------------------------------------------------------------------*/
/*      Define INLINE attributes by compiler                         */
/*-------------------------------------------------------------------*/
#if !defined(INLINE)
  #if defined(__GNUC__)
    #define INLINE          __inline__ __attribute__((always_inline))
  #elif defined(_MSVC_)
    #define INLINE          __forceinline
  #else
    #define INLINE          __inline
  #endif
#endif

/*-------------------------------------------------------------------*/
/*      Use '__noop()' to disable code generating macros             */
/*-------------------------------------------------------------------*/
#if !defined(_MSVC_)
  #if !defined(__noop)
    #define __noop(...)     do{;}while(0)   /*  (i.e. do nothing)    */
  #endif
#endif

/*-------------------------------------------------------------------*/
/*      Round a value up to the next boundary                        */
/*-------------------------------------------------------------------*/
#define ROUND_UP(x,y)       ((((x)+((y)-1))/(y))*(y))

/*-------------------------------------------------------------------*/
/*      Define min/max macros                                        */
/*-------------------------------------------------------------------*/
#if !defined(min)
  #define min(_x, _y)       ((_x) < (_y) ? (_x) : (_y))
#endif
#if !defined(max)
  #define max(_x, _y)       ((_x) > (_y) ? (_x) : (_y))
#endif
#if !defined(MIN)
  #define MIN(_x,_y)        min((_x),(_y))
#endif
#if !defined(MAX)
  #define MAX(_x,_y)        max((_x),(_y))
#endif
/*-------------------------------------------------------------------*/
/*      MINMAX        ensures var x remains within range y to z      */
/*-------------------------------------------------------------------*/
#if !defined(MINMAX)
  #define  MINMAX(_x,_y,_z) ((_x) = MIN(MAX((_x),(_y)),(_z)))
#endif

/*-------------------------------------------------------------------*/
/*      some handy array/struct macros...                            */
/*-------------------------------------------------------------------*/
#ifndef   _countof
  #define _countof(x)       ( sizeof(x) / sizeof(x[0]) )
#endif
#ifndef   arraysize
  #define arraysize(x)      _countof(x)
#endif
#ifndef   sizeof_member
  #define sizeof_member(_struct,_member) sizeof(((_struct*)0)->_member)
#endif
#ifndef   offsetof
  #define offsetof(_struct,_member)   (size_t)&(((_struct*)0)->_member)
#endif

/*-------------------------------------------------------------------*/
/*      CASSERT macro       a compile time assertion check           */
/*-------------------------------------------------------------------*/
/** 
 *  Validates at compile time whether condition is true without
 *  generating code. It can be used at any point in a source file
 *  where typedefs are legal.
 *
 *  On success, compilation proceeds normally.
 *
 *  Sample usage:
 *
 *      CASSERT(sizeof(struct foo) == 76, demo_c)
 *
 *  On failure, attempts to typedef an array type of negative size
 *  resulting in a compiler error message that might look something
 *  like the following:
 *
 *      demo.c:32: error: size of array 'assertion_failed_demo_c_32' is negative
 *
 *  The offending line itself will look something like this:
 *
 *      typedef assertion_failed_demo_c_32[-1]
 *
 *  where demo_c is the content of the second parameter which should
 *  typically be related in some obvious way to the containing file
 *  name, 32 is the line number in the file on which the assertion
 *  appears, and -1 is the result of a calculation based on the cond
 *  failing.
 *
 *  \param cond         The predicate to test. It should evaluate
 *                      to something which can be coerced into a
 *                      normal C boolean.
 *
 *  \param file         A sequence of legal identifier characters
 *                      that should uniquely identify the source
 *                      file where the CASSERT statement appears.
 */
#if !defined(CASSERT)
#define CASSERT(cond, file)     _CASSERT_LINE(cond,__LINE__,file)
#define _CASSERT_PASTE(a,b)     a##b
#define _CASSERT_LINE(cond, line, file) \
  typedef char _CASSERT_PASTE(assertion_failed_##file##_,line)[2*!!(cond)-1];
#endif

/*-------------------------------------------------------------------*/
/*      compiler optimization hints         (for performance)        */
/*-------------------------------------------------------------------*/

#undef likely
#undef unlikely

#ifdef _MSVC_

  #define likely(_c)      ( (_c) ? ( __assume((_c)), 1 ) :                    0   )
  #define unlikely(_c)    ( (_c) ?                   1   : ( __assume(!(_c)), 0 ) )

#else // !_MSVC_

  #if __GNUC__ >= 3
    #define likely(_c)    __builtin_expect((_c),1)
    #define unlikely(_c)  __builtin_expect((_c),0)
  #else
    #define likely(_c)    (_c)
    #define unlikely(_c)  (_c)
  #endif

#endif // _MSVC_

/*-------------------------------------------------------------------*/
/*      _MSVC_  portability macros                                   */
/*-------------------------------------------------------------------*/

/* PROGRAMMING NOTE: the following 'tape' portability macros are
   only for physical (SCSI) tape devices, not emulated aws files */

#ifdef _MSVC_
  #define  open_tape            w32_open_tape
  #define  read_tape            w32_read_tape
  #define  write_tape           w32_write_tape
  #define  ioctl_tape           w32_ioctl_tape
  #define  close_tape           w32_close_tape
#else
  #define  open_tape            open
  #define  read_tape            read
  #define  write_tape           write
  #define  ioctl_tape           ioctl
  #define  close_tape           close
#endif

#ifdef _MSVC_
  #define  create_pipe(a)       socketpair(AF_INET,SOCK_STREAM,IPPROTO_IP,a)
  #define  read_pipe(f,b,n)     recv(f,b,n,0)
  #define  write_pipe(f,b,n)    send(f,b,(int)n,0)
  #define  close_pipe(f)        closesocket(f)
#else
  #define  create_pipe(f)       pipe(f)
  #define  read_pipe(f,b,n)     read(f,b,n)
  #define  write_pipe(f,b,n)    write(f,b,n)
  #define  close_pipe(f)        close(f)
#endif

#ifdef _MSVC_
  #define  socket               w32_socket
/* Now defined in hsocket.h
  int read_socket(int fd, char *ptr, int nbytes);
  int write_socket(int fd, const char *ptr, int nbytes);
*/
  #define  close_socket(f)      closesocket(f)
  #define  hif_nametoindex      w32_if_nametoindex
  #define  hinet_ntop           w32_inet_ntop
  #define  hinet_pton           w32_inet_pton
#else
/* Now defined in hsocket.h
  int read_socket(int fd, char *ptr, int nbytes);
  int write_socket(int fd, const char *ptr, int nbytes);
*/
  #define  close_socket(f)      close(f)
  #define  hif_nametoindex      if_nametoindex
  #define  hinet_ntop           inet_ntop
  #define  hinet_pton           inet_pton
#endif

#ifdef _MSVC_
  #undef   FD_SET
  #undef   FD_ISSET
  #define  FD_SET               w32_FD_SET
  #define  FD_ISSET             w32_FD_ISSET
  #define  select(n,r,w,e,t)    w32_select((n),(r),(w),(e),(t),__FILE__,__LINE__)
  #define  fdopen               w32_fdopen
  #define  fwrite               w32_fwrite
  #define  fprintf              w32_fprintf
  #define  fclose               w32_fclose
  #define  basename             w32_basename
  #define  dirname              w32_dirname
#ifndef strcasestr
  #define  strcasestr           w32_strcasestr
#endif
#endif

#ifdef _MSVC_
  #define  fdatasync            _commit
  #define  atoll                _atoi64
#else /* !_MSVC_ */
  #if !defined(HAVE_FDATASYNC_SUPPORTED)
    #ifdef HAVE_FSYNC
      #define  fdatasync        fsync
    #else
      #error Required 'fdatasync' function is missing and alternate 'fsync' function also missing
    #endif
  #endif
  #define  atoll(s)             strtoll(s,NULL,0)
#endif

/*-------------------------------------------------------------------*/
/* Portable macro for copying 'va_list' variable arguments variable  */
/*-------------------------------------------------------------------*/

// ZZ FIXME: this should probably be handled in configure.ac...

#if !defined( va_copy )
  #if defined( __va_copy )
    #define  va_copy            __va_copy
  #elif defined( _MSVC_ )
    #define  va_copy(to,from)   (to) = (from)
  #else
    #define  va_copy(to,from)   memcpy((to),(from),sizeof(va_list))
  #endif
#endif

/*-------------------------------------------------------------------*/
/* Handle newer RUSAGE_THREAD definition for older Linux levels      */
/* before 2.6.4 along with *nix systems not supporting...            */
/*-------------------------------------------------------------------*/

// ZZ FIXME: this should probably be handled in configure.ac...

#if !defined(RUSAGE_THREAD) && !defined(_MSVC_)
  #define   RUSAGE_THREAD       thread_id()
#endif

/*-------------------------------------------------------------------*/
/*      Some handy quantity definitions                              */
/*-------------------------------------------------------------------*/
#define  ONE_KILOBYTE   ((U32)                     (1024))  /* 2^10 (16^2)  * 4  */
#define  TWO_KILOBYTE   ((U32)(2           *        1024))  /* 2^11 (16^2)  * 8  */
#define  FOUR_KILOBYTE  ((U32)(4           *        1024))  /* 2^12 (16^3)       */
#define  _64_KILOBYTE   ((U32)(64          *        1024))  /* 2^16 (16^4)       */
#define  HALF_MEGABYTE  ((U32)(512         *        1024))  /* 2^19 (16^4)  * 8  */
#define  ONE_MEGABYTE   ((U32)(1024        *        1024))  /* 2^20 (16^5)       */
#define  ONE_GIGABYTE   ((U64)ONE_MEGABYTE * (U64) (1024))  /* 2^30 (16^7)  * 4  */
#define  ONE_TERABYTE   (ONE_GIGABYTE      * (U64) (1024))  /* 2^40 (16^10)      */
#define  ONE_PETABYTE   (ONE_TERABYTE      * (U64) (1024))  /* 2^50 (16^12) * 4  */
#define  ONE_EXABYTE    (ONE_PETABYTE      * (U64) (1024))  /* 2^60 (16^15)      */
#if defined(U128)
#define  ONE_ZETTABYTE  (ONE_EXABYTE       * (U128)(1024))  /* 2^70 (16^17) * 4  */
#define  ONE_YOTTABYTE  (ONE_ZETTABYTE     * (U128)(1024))  /* 2^80 (16^20)      */
#endif

#define  SHIFT_KILOBYTE     10
#define  SHIFT_64KBYTE      16
#define  SHIFT_MEGABYTE     20
#define  SHIFT_GIGABYTE     30
#define  SHIFT_TERABYTE     40
#define  SHIFT_PETABYTE     50
#define  SHIFT_EXABYTE      60
#define  SHIFT_ZETTABYTE    70
#define  SHIFT_YOTTABYTE    80

/* IEC Binary Prefixes, etc */
#define  ONE_KIBIBYTE  ((U32)                     (1024))  /* 2^10 (16^2)  * 4  */
#define  TWO_KIBIBYTE  ((U32)(2           *        1024))  /* 2^11 (16^2)  * 8  */
#define  FOUR_KIBIBYTE ((U32)(4           *        1024))  /* 2^12 (16^3)       */
#define  _64_KIBIBYTE  ((U32)(64          *        1024))  /* 2^16 (16^4)       */
#define  HALF_MEBIBYTE ((U32)(512         *        1024))  /* 2^19 (16^4)  * 8  */
#define  ONE_MEBIBYTE  ((U32)(1024        *        1024))  /* 2^20 (16^5)       */
#define  ONE_GIBIBYTE  ((U64)ONE_MEBIBYTE * (U64) (1024))  /* 2^30 (16^7)  * 4  */
#define  ONE_TEBIBYTE  (ONE_GIBIBYTE      * (U64) (1024))  /* 2^40 (16^10)      */
#define  ONE_PEBIBYTE  (ONE_TEBIBYTE      * (U64) (1024))  /* 2^50 (16^12) * 4  */
#define  ONE_EXBIBYTE  (ONE_PEBIBYTE      * (U64) (1024))  /* 2^60 (16^15)      */
#if defined(U128)
#define  ONE_ZEBIBYTE  (ONE_EXBIBYTE      * (U128)(1024))  /* 2^70 (16^17) * 4  */
#define  ONE_YOBIBYTE  (ONE_ZEBIBYTE      * (U128)(1024))  /* 2^80 (16^20)      */
#endif

#define  SHIFT_KIBIBYTE     10
#define  SHIFT_MEBIBYTE     20
#define  SHIFT_GIBIBYTE     30
#define  SHIFT_TEBIBYTE     40
#define  SHIFT_PEBIBYTE     50
#define  SHIFT_EXBIBYTE     60
#define  SHIFT_ZEBIBYTE     70
#define  SHIFT_YOBIBYTE     80

/* US VERSIONS */
#define ONE_HUNDRED     ((U32)                        (100))    /* zeros = 2  */
#define ONE_THOUSAND    ((U32)                       (1000))    /* zeros = 3  */
#define ONE_MILLION     ((U32)(1000           *       1000))    /* zeros = 6  */
#define ONE_BILLION     ((U64)ONE_MILLION     * (U64)(1000))    /* zeros = 9  */
#define ONE_TRILLION    ((U64)ONE_BILLION     * (U64)(1000))    /* zeros = 12 */
#define ONE_QUADRILLION ((U64)ONE_TRILLION    * (U64)(1000))    /* zeros = 15 */
#define ONE_QUINTILLION ((U64)ONE_QUADRILLION * (U64)(1000))    /* zeros = 18 */
#define ONE_SEXTILLION  ((U64)ONE_QUINTILLION * (U64)(1000))    /* zeros = 21 */
#define ONE_SEPTILLION  ((U64)ONE_SEXTILLION  * (U64)(1000))    /* zeros = 24 */

/*-------------------------------------------------------------------*/
/*      Some handy memory/string comparison macros                   */
/*-------------------------------------------------------------------*/
#define mem_eq(_a,_b,_n)            (!memcmp(_a,_b,_n))
#define mem_ne(_a,_b,_n)            ( memcmp(_a,_b,_n))

#define str_eq(_a,_b)               (!strcmp(_a,_b))
#define str_ne(_a,_b)               ( strcmp(_a,_b))

#define str_eq_n(_a,_b,_n)          (!strncmp(_a,_b,_n))
#define str_ne_n(_a,_b,_n)          ( strncmp(_a,_b,_n))

#define str_caseless_eq(_a,_b)      (!strcasecmp(_a,_b))
#define str_caseless_ne(_a,_b)      ( strcasecmp(_a,_b))

#define str_caseless_eq_n(_a,_b,_n) (!strncasecmp(_a,_b,_n))
#define str_caseless_ne_n(_a,_b,_n) ( strncasecmp(_a,_b,_n))

/*-------------------------------------------------------------------*/
/*      Large File Support portability                               */
/*-------------------------------------------------------------------*/

#ifdef _MSVC_
  /* "Native" 64-bit Large File Support */
  #define    off_t              __int64
  #if (_MSC_VER >= VS2005)
    #define  ftruncate          _chsize_s
    #define  ftell              _ftelli64
    #define  fseek              _fseeki64
  #else // (_MSC_VER < VS2005)
    #define  ftruncate          w32_ftrunc64
    #define  ftell              w32_ftelli64
    #define  fseek              w32_fseeki64
  #endif
  #define    lseek              _lseeki64
  #define    fstat              _fstati64
  #define    stat               _stati64
#elif defined(_LFS_LARGEFILE) || ( defined(SIZEOF_OFF_T) && SIZEOF_OFF_T > 4 )
  /* Native 64-bit Large File Support */
  #if defined(HAVE_FSEEKO)
    #define  ftell              ftello
    #define  fseek              fseeko
  #else
    #if defined(SIZEOF_LONG) && SIZEOF_LONG <= 4
      #warning fseek/ftell use offset arguments of insufficient size
    #endif
  #endif
#elif defined(_LFS64_LARGEFILE)
  /* Transitional 64-bit Large File Support */
  #define    off_t              off64_t
  #define    ftruncate          ftruncate64
  #define    ftell              ftello64
  #define    fseek              fseeko64
  #define    lseek              lseek64
  #define    fstat              fstat64
  #define    stat               stat64
#else // !defined(_LFS_LARGEFILE) && !defined(_LFS64_LARGEFILE) && (!defined(SIZEOF_OFF_T) || SIZEOF_OFF_T <= 4)
  /* No 64-bit Large File Support at all */
  #warning Large File Support missing
#endif
// Hercules low-level file open...
// PROGRAMMING NOTE: the "##" preceding "__VA_ARGS__" is required for compat-
//                   ibility with gcc/MSVC compilers and must not be removed
#ifdef _MSVC_
  #define   HOPEN(_p,_o,...)    w32_hopen ((_p),(_o), ## __VA_ARGS__)
#else
  #define   HOPEN(_p,_o,...)    hopen     ((_p),(_o), ## __VA_ARGS__)
#endif

/*-------------------------------------------------------------------*/
/*      Macro for command parsing with variable length               */
/*-------------------------------------------------------------------*/
#define  CMD(str,cmd,min) ( strcaseabbrev(#cmd,str,min) )

#define  NCMP(_lvar,_rvar,_svar) ( !memcmp( _lvar, _rvar, _svar ) )
#define  SNCMP(_lvar,_rvar,_svar) ( !strncasecmp( _lvar, _rvar, _svar ) )

/*-------------------------------------------------------------------*/
/*      Script processing constants                                  */
/*-------------------------------------------------------------------*/
#define  MAX_SCRIPT_STMT    1024        /* Max script stmt length    */
#define  MAX_SCRIPT_DEPTH   10          /* Max script nesting depth  */

/*-------------------------------------------------------------------*/
/*      Debugging / Tracing macros.                                  */
/*-------------------------------------------------------------------*/
#define MLVL( _lvl) \
    (sysblk.msglvl & (MLVL_ ## _lvl))

/* Obsolete NLS support macro */
#define _(_string)  _string

/* Opcode routing table function pointer */
typedef void (ATTR_REGPARM(2)*FUNC)();

/* Program Interrupt function pointer */
typedef void (ATTR_REGPARM(2) *pi_func) (REGS *regs, int pcode);

/* trace_br function */
typedef U32  (*s390_trace_br_func) (int amode,  U32 ia, REGS *regs);
typedef U64  (*z900_trace_br_func) (int amode,  U64 ia, REGS *regs);

/* qsort comparison function typedef */
typedef int CMPFUNC(const void*, const void*);

/*-------------------------------------------------------------------*/
/*      CPU state related macros and constants                       */
/*-------------------------------------------------------------------*/

/* Definitions for CPU state */
#define CPUSTATE_STARTED        1       /* CPU is started            */
#define CPUSTATE_STOPPING       2       /* CPU is stopping           */
#define CPUSTATE_STOPPED        3       /* CPU is stopped            */

#define IS_CPU_ONLINE(_cpu) \
  (sysblk.regs[(_cpu)] != NULL)

/* Instruction count for a CPU */
#define INSTCOUNT(_regs) \
 ((_regs)->hostregs->prevcount + (_regs)->hostregs->instcount)

/*-------------------------------------------------------------------*/
/*      Obtain/Release mainlock                                      */
/*      mainlock is only obtained by a CPU thread                    */
/*-------------------------------------------------------------------*/

#define OBTAIN_MAINLOCK(_regs) \
 do { \
  if ((_regs)->hostregs->cpubit != (_regs)->sysblk->started_mask) { \
   obtain_lock(&(_regs)->sysblk->mainlock); \
   (_regs)->sysblk->mainowner = regs->hostregs->cpuad; \
  } \
 } while (0)

#define RELEASE_MAINLOCK(_regs) \
 do { \
   if ((_regs)->sysblk->mainowner == (_regs)->hostregs->cpuad) { \
     (_regs)->sysblk->mainowner = LOCK_OWNER_NONE; \
     release_lock(&(_regs)->sysblk->mainlock); \
   } \
 } while (0)

/*-------------------------------------------------------------------*/
/*      Obtain/Release crwlock                                       */
/*      crwlock can be obtained by any thread                        */
/*-------------------------------------------------------------------*/

#define OBTAIN_CRWLOCK()    obtain_lock( &sysblk.crwlock )
#define RELEASE_CRWLOCK()   release_lock( &sysblk.crwlock )

/*-------------------------------------------------------------------*/
/*      Obtain/Release intlock                                       */
/*      intlock can be obtained by any thread                        */
/*      if obtained by a cpu thread, check to see                    */
/*      if synchronize_cpus is in progress.                          */
/*-------------------------------------------------------------------*/

#define OBTAIN_INTLOCK(_iregs) \
 do { \
   REGS *_regs = (_iregs); \
   if ((_regs)) \
     (_regs)->hostregs->intwait = 1; \
   obtain_lock (&sysblk.intlock); \
   if ((_regs)) { \
     while (sysblk.syncing) { \
       sysblk.sync_mask &= ~(_regs)->hostregs->cpubit; \
       if (!sysblk.sync_mask) \
         signal_condition(&sysblk.sync_cond); \
       wait_condition(&sysblk.sync_bc_cond, &sysblk.intlock); \
     } \
     (_regs)->hostregs->intwait = 0; \
     sysblk.intowner = (_regs)->hostregs->cpuad; \
   } else \
     sysblk.intowner = LOCK_OWNER_OTHER; \
 } while (0)

#define RELEASE_INTLOCK(_regs) \
 do { \
   sysblk.intowner = LOCK_OWNER_NONE; \
   release_lock(&sysblk.intlock); \
 } while (0)

/*-------------------------------------------------------------------*/
/* Returns when all other CPU threads are blocked on intlock         */
/*-------------------------------------------------------------------*/
#ifdef OPTION_SYNCIO
  #define AT_SYNCPOINT(_regs)    ((_regs)->intwait || (_regs)->syncio)
#else // OPTION_NOSYNCIO
  #define AT_SYNCPOINT(_regs)    ((_regs)->intwait)
#endif // OPTION_SYNCIO

/*-------------------------------------------------------------------*/
/*      Synchronize CPUS                                             */
/*      Locks used: INTLOCK(regs)                                    */
/*-------------------------------------------------------------------*/
#define SYNCHRONIZE_CPUS(_regs) \
        synchronize_cpus(_regs)

/*-------------------------------------------------------------------*/
/*      Macros to signal interrupt condition to a CPU[s]             */
/*-------------------------------------------------------------------*/

#define WAKEUP_CPU(_regs) \
 do { \
   signal_condition(&(_regs)->intcond); \
 } while (0)

#define WAKEUP_CPU_MASK(_mask) \
 do { \
   int i; \
   CPU_BITMAP mask = (_mask); \
   for (i = 0; mask; i++) { \
     if (mask & 1) \
     { \
       signal_condition(&sysblk.regs[i]->intcond); \
       break; \
     } \
     mask >>= 1; \
   } \
 } while (0)

#define WAKEUP_CPUS_MASK(_mask) \
 do { \
   int i; \
   CPU_BITMAP mask = (_mask); \
   for (i = 0; mask; i++) { \
     if (mask & 1) \
       signal_condition(&sysblk.regs[i]->intcond); \
     mask >>= 1; \
   } \
 } while (0)

/*-------------------------------------------------------------------*/
/*      Macros to queue/dequeue device on I/O interrupt queue        */
/*      sysblk.iointqlk ALWAYS needed to examine sysblk.iointq       */
/*-------------------------------------------------------------------*/


#define QUEUE_IO_INTERRUPT(_io) \
 do { \
   obtain_lock(&sysblk.iointqlk); \
   QUEUE_IO_INTERRUPT_QLOCKED((_io)); \
   release_lock(&sysblk.iointqlk); \
 } while (0)

#define QUEUE_IO_INTERRUPT_QLOCKED(_io)                               \
 do {                                                                 \
    IOINT *prev;                                                      \
    for (prev = (IOINT *)&sysblk.iointq;                              \
         prev->next != NULL                                           \
         && prev->next != (_io)                                       \
         && prev->next->priority >= (_io)->dev->priority;             \
         prev = prev->next);                                          \
    if (prev->next != (_io))                                          \
        (_io)->next     = prev->next,                                 \
        prev->next      = (_io),                                      \
        (_io)->priority = (_io)->dev->priority;                       \
         if ((_io)->pending)     (_io)->dev->pending     = 1;         \
    else if ((_io)->pcipending)  (_io)->dev->pcipending  = 1;         \
    else if ((_io)->attnpending) (_io)->dev->attnpending = 1;         \
 } while (0)

#define DEQUEUE_IO_INTERRUPT(_io) \
 do { \
   obtain_lock(&sysblk.iointqlk); \
   DEQUEUE_IO_INTERRUPT_QLOCKED((_io)); \
   release_lock(&sysblk.iointqlk); \
 } while (0)

#define DEQUEUE_IO_INTERRUPT_QLOCKED(_io) \
 do { \
   IOINT *prev; \
   for (prev = (IOINT *)&sysblk.iointq; prev->next != NULL; prev = prev->next) \
     if (prev->next == (_io)) { \
       prev->next = (_io)->next; \
            if ((_io)->pending)     (_io)->dev->pending     = 0; \
       else if ((_io)->pcipending)  (_io)->dev->pcipending  = 0; \
       else if ((_io)->attnpending) (_io)->dev->attnpending = 0; \
       break; \
     } \
 } while (0)

/*    NOTE: sysblk.iointqlk needed to examine sysblk.iointq,
      sysblk.intlock (which MUST be held before calling these
      macros) needed in order to set/reset IC_IOPENDING flag
*/
#define UPDATE_IC_IOPENDING() \
 do { \
   obtain_lock(&sysblk.iointqlk); \
   UPDATE_IC_IOPENDING_QLOCKED(); \
   release_lock(&sysblk.iointqlk); \
 } while (0)

#define UPDATE_IC_IOPENDING_QLOCKED() \
 do { \
   if (sysblk.iointq == NULL) \
     OFF_IC_IOPENDING; \
   else { \
     ON_IC_IOPENDING; \
   } \
 } while (0)

/*-------------------------------------------------------------------*/
/*      Handy utility macro for channel.c                            */
/*-------------------------------------------------------------------*/

#define IS_CCW_IMMEDIATE(_dev,_code) \
  ( \
    ( (_dev)->hnd->immed && (_dev)->hnd->immed[(_code)]) \
    || ( (_dev)->immed      && (_dev)->immed[(_code)]) \
    || IS_CCW_NOP((_dev)->code) \
  )

/*-------------------------------------------------------------------*/
/*      Macro to check if DEVBLK is for an existing device           */
/*-------------------------------------------------------------------*/

#if defined(_FEATURE_INTEGRATED_3270_CONSOLE)
  #define IS_DEV(_dev) \
    ((_dev)->allocated && (((_dev)->pmcw.flag5 & PMCW5_V) || (_dev) == sysblk.sysgdev))
#else // !defined(_FEATURE_INTEGRATED_3270_CONSOLE)
  #define IS_DEV(_dev) \
    ((_dev)->allocated && ((_dev)->pmcw.flag5 & PMCW5_V))
#endif // defined(_FEATURE_INTEGRATED_3270_CONSOLE)

/*-------------------------------------------------------------------*/
/*      HDL macro to call optional function override                 */
/*-------------------------------------------------------------------*/

#if defined(OPTION_DYNAMIC_LOAD)
  #define HDC1(_func, _arg1) \
    ((_func) ? (_func) ((_arg1)) : (NULL))
  #define HDC2(_func, _arg1,_arg2) \
    ((_func) ? (_func) ((_arg1),(_arg2)) : (NULL))
  #define HDC3(_func, _arg1,_arg2,_arg3) \
    ((_func) ? (_func) ((_arg1),(_arg2),(_arg3)) : (NULL))
  #define HDC4(_func, _arg1,_arg2,_arg3,_arg4) \
    ((_func) ? (_func) ((_arg1),(_arg2),(_arg3),(_arg4)) : (NULL))
  #define HDC5(_func, _arg1,_arg2,_arg3,_arg4,_arg5) \
    ((_func) ? (_func) ((_arg1),(_arg2),(_arg3),(_arg4),(_arg5)) : (NULL))
  #define HDC6(_func, _arg1,_arg2,_arg3,_arg4,_arg5,_arg6) \
    ((_func) ? (_func) ((_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6)) : (NULL))
#else
  #define HDC1(_func, _arg1) \
    (NULL)
  #define HDC2(_func, _arg1,_arg2) \
    (NULL)
  #define HDC3(_func, _arg1,_arg2,_arg3) \
    (NULL)
  #define HDC4(_func, _arg1,_arg2,_arg3,_arg4) \
    (NULL)
  #define HDC5(_func, _arg1,_arg2,_arg3,_arg4,_arg5) \
    (NULL)
  #define HDC6(_func, _arg1,_arg2,_arg3,_arg4,_arg5,_arg6) \
    (NULL)
#endif

/*-------------------------------------------------------------------*/
/*      Sleep for as long as we like   (whole number of seconds)     */
/*-------------------------------------------------------------------*/

#define SLEEP(_n) \
 do { \
   unsigned int rc = (_n); \
   while (rc) \
     if ((rc = sleep (rc))) \
       sched_yield(); \
 } while (0)

/*-------------------------------------------------------------------*/
/*      CRASH                       (with hopefully a dump)          */
/*-------------------------------------------------------------------*/

#ifdef _MSVC_
  #define CRASH() \
    do { \
      BYTE *p = NULL; \
      *p=0; \
    } while (0)
#else
  #define CRASH() \
    do { \
      abort(); \
    } while (0)
#endif

/*-------------------------------------------------------------------*/
/*      Perform standard utility initialization                      */
/*-------------------------------------------------------------------*/

#if !defined(EXTERNALGUI)
  #define INITIALIZE_EXTERNAL_GUI()
#else
  #define INITIALIZE_EXTERNAL_GUI() \
  do { \
    if (argc >= 1 && strncmp(argv[argc-1],"EXTERNALGUI",11) == 0) { \
        extgui = 1; \
        argv[argc-1] = NULL; \
        argc--; \
        setvbuf(stderr, NULL, _IONBF, 0); \
        setvbuf(stdout, NULL, _IONBF, 0); \
    } \
  } while (0)
#endif

#if defined(OPTION_MSGLCK)
 #define INIT_MSGLCK initialize_lock (&sysblk.msglock);
#else
 #define INIT_MSGLCK
#endif

#if defined(OPTION_CONFIG_SYMBOLS)
#define INIT_UTILMSGLVL \
    do \
    { \
        char *msglvl = (char *)get_symbol( "HERCULES_UTIL_MSGLVL" );\
        if ( msglvl != NULL ) \
        { \
            sysblk.emsg = EMSG_ON; \
            if ( strcasestr(msglvl, "all") ) \
                sysblk.msglvl |= MLVL_ANY; \
            if ( strcasestr(msglvl, "debug") ) \
                sysblk.msglvl |= MLVL_DEBUG | MLVL_DEVICES; \
            if ( strcasestr(msglvl, "verbose") )\
                sysblk.msglvl |= MLVL_VERBOSE; \
            if ( strcasestr(msglvl, "tape") ) \
                sysblk.msglvl |= MLVL_TAPE; \
            if ( strcasestr(msglvl, "dasd") ) \
                sysblk.msglvl |= MLVL_DASD; \
            if ( strcasestr(msglvl, "time") ) \
                sysblk.emsg |= EMSG_TS; \
        } \
    } while (0)
#else
#define INIT_UTILMSGLVL
#endif

#define INITIALIZE_UTILITY(name) \
  do { \
    SET_THREAD_NAME(name); \
    INITIALIZE_EXTERNAL_GUI(); \
    memset (&sysblk, 0, sizeof(SYSBLK)); \
    INIT_MSGLCK \
    initialize_detach_attr (DETACHED); \
    initialize_join_attr   (JOINABLE); \
    set_codepage(NULL); \
    init_hostinfo( &hostinfo ); \
    INIT_UTILMSGLVL; \
  } while (0)

/*-------------------------------------------------------------------*/
/*      Assign name to thread           (debugging aid)              */
/*-------------------------------------------------------------------*/

#ifdef _MSVC_
  #define  SET_THREAD_NAME_ID(t,n)  w32_set_thread_name((t),(n))
  #define  SET_THREAD_NAME(n)       SET_THREAD_NAME_ID(GetCurrentThreadId(),(n))
#else
  #define  SET_THREAD_NAME_ID(t,n)
  #define  SET_THREAD_NAME(n)
#endif

#if !defined(NO_SETUID)

/* SETMODE(INIT)
 *   sets the saved uid to the effective uid, and
 *   sets the effective uid to the real uid, such
 *   that the program is running with normal user
 *   attributes, other then that it may switch to
 *   the saved uid by SETMODE(ROOT). This call is
 *   usually made upon entry to the setuid program.
 *
 * SETMODE(ROOT)
 *   sets the saved uid to the real uid, and
 *   sets the real and effective uid to the saved uid.
 *   A setuid root program will enter 'root mode' and
 *   will have all the appropriate access.
 *
 * SETMODE(USER)
 *   sets the real and effective uid to the uid of the
 *   caller.  The saved uid will be the effective uid
 *   upon entry to the program (as before SETMODE(INIT))
 *
 * SETMODE(TERM)
 *   sets real, effective and saved uid to the real uid
 *   upon entry to the program.  This call will revoke
 *   any setuid access that the thread/process has.  It
 *   is important to issue this call before an exec to a
 *   shell or other program that could introduce integrity
 *   exposures when running with root access.
 */

#if defined(HAVE_SYS_CAPABILITY_H) && defined(HAVE_SYS_PRCTL_H) && defined(OPTION_CAPABILITIES)

#define SETMODE(_x)
#define DROP_PRIVILEGES(_capa) drop_privileges(_capa)
#define DROP_ALL_CAPS() drop_all_caps()

#else

#define DROP_PRIVILEGES(_capa)
#define DROP_ALL_CAPS()

#if defined(HAVE_SETRESUID)

#define _SETMODE_INIT \
do { \
    getresuid(&sysblk.ruid,&sysblk.euid,&sysblk.suid); \
    getresgid(&sysblk.rgid,&sysblk.egid,&sysblk.sgid); \
    setresuid(sysblk.ruid,sysblk.ruid,sysblk.euid); \
    setresgid(sysblk.rgid,sysblk.rgid,sysblk.egid); \
} while(0)

#define _SETMODE_ROOT \
do { \
    setresuid(sysblk.suid,sysblk.suid,sysblk.ruid); \
} while(0)

#define _SETMODE_USER \
do { \
    setresuid(sysblk.ruid,sysblk.ruid,sysblk.suid); \
} while(0)

#define _SETMODE_TERM \
do { \
    setresuid(sysblk.ruid,sysblk.ruid,sysblk.ruid); \
    setresgid(sysblk.rgid,sysblk.rgid,sysblk.rgid); \
} while(0)

#elif defined(HAVE_SETREUID)

#define _SETMODE_INIT \
do { \
    sysblk.ruid = getuid(); \
    sysblk.euid = geteuid(); \
    sysblk.rgid = getgid(); \
    sysblk.egid = getegid(); \
    setreuid(sysblk.euid, sysblk.ruid); \
    setregid(sysblk.egid, sysblk.rgid); \
} while (0)

#define _SETMODE_ROOT \
do { \
    setreuid(sysblk.ruid, sysblk.euid); \
    setregid(sysblk.rgid, sysblk.egid); \
} while (0)

#define _SETMODE_USER \
do { \
    setregid(sysblk.egid, sysblk.rgid); \
    setreuid(sysblk.euid, sysblk.ruid); \
} while (0)

#define _SETMODE_TERM \
do { \
    setuid(sysblk.ruid); \
    setgid(sysblk.rgid); \
} while (0)

#else /* defined(HAVE_SETRESUID) || defined(HAVE_SETEREUID) */

#error Cannot figure out how to swap effective UID/GID, maybe you should define NO_SETUID?

#endif /* defined(HAVE_SETREUID) || defined(HAVE_SETRESUID) */

#define SETMODE(_func) _SETMODE_ ## _func

#endif /* !defined(HAVE_SYS_CAPABILITY_H) */

#else /* !defined(NO_SETUID) */

#define SETMODE(_func)
#define DROP_PRIVILEGES(_capa)
#define DROP_ALL_CAPS()

#endif /* !defined(NO_SETUID) */

/*-------------------------------------------------------------------*/
/*      Pipe signaling            (thread signaling via pipe)        */
/*-------------------------------------------------------------------*/

#define RECV_PIPE_SIGNAL( rfd, lock, flag ) \
  do { \
    int f; int saved_errno=get_HSO_errno(); BYTE c=0; \
    obtain_lock(&(lock)); \
    if ((f=(flag))>=1) (flag)=0; \
    release_lock(&(lock)); \
    if (f>=1) \
      VERIFY(read_pipe((rfd),&c,1)==1); \
    set_HSO_errno(saved_errno); \
  } while (0)

#define SEND_PIPE_SIGNAL( wfd, lock, flag ) \
  do { \
    int f; int saved_errno=get_HSO_errno(); BYTE c=0; \
    obtain_lock(&(lock)); \
    if ((f=(flag))<=0) (flag)=1; \
    release_lock(&(lock)); \
    if (f<=0) \
      VERIFY(write_pipe((wfd),&c,1)==1); \
    set_HSO_errno(saved_errno); \
  } while (0)

#define SUPPORT_WAKEUP_SELECT_VIA_PIPE( pipe_rfd, maxfd, prset ) \
  FD_SET((pipe_rfd),(prset)); \
  (maxfd)=(maxfd)>(pipe_rfd)?(maxfd):(pipe_rfd)

#define SUPPORT_WAKEUP_CONSOLE_SELECT_VIA_PIPE( maxfd, prset )  SUPPORT_WAKEUP_SELECT_VIA_PIPE( sysblk.cnslrpipe, (maxfd), (prset) )
#define SUPPORT_WAKEUP_SOCKDEV_SELECT_VIA_PIPE( maxfd, prset )  SUPPORT_WAKEUP_SELECT_VIA_PIPE( sysblk.sockrpipe, (maxfd), (prset) )

#define RECV_CONSOLE_THREAD_PIPE_SIGNAL()  RECV_PIPE_SIGNAL( sysblk.cnslrpipe, sysblk.cnslpipe_lock, sysblk.cnslpipe_flag )
#define RECV_SOCKDEV_THREAD_PIPE_SIGNAL()  RECV_PIPE_SIGNAL( sysblk.sockrpipe, sysblk.sockpipe_lock, sysblk.sockpipe_flag )
#define SIGNAL_CONSOLE_THREAD()            SEND_PIPE_SIGNAL( sysblk.cnslwpipe, sysblk.cnslpipe_lock, sysblk.cnslpipe_flag )
#define SIGNAL_SOCKDEV_THREAD()            SEND_PIPE_SIGNAL( sysblk.sockwpipe, sysblk.sockpipe_lock, sysblk.sockpipe_flag )

#endif // _HMACROS_H
