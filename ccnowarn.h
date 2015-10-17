/* ccnowarn.h (C) Copyright "Fish" (David B. Trout), 2011            */
/*                                                                   */
/*             Released under "The Q Public License Version 1"       */
/*             (http://www.hercules-390.org/herclic.html)            */
/*             as modifications to Hercules.                         */

/*-------------------------------------------------------------------*/
/* The "DISABLE_xxx_WARNING" and "ENABLE_xxx_WARNING" macros allow   */
/* you to temporarily suppress certain harmless compiler warnings.   */
/* Use the "_DISABLE" macro before the source statement which is     */
/* causing the problem and the "_ENABLE" macro shortly afterwards.   */
/* PLEASE DO NOT GO OVERBOARD (overdo or overuse) THE SUPPRESSION    */
/* OF WARNINGS! Most warnings are actually bugs waiting to happen.   */
/* The "DISABLE_xxx_WARNING" and "ENABLE_xxx_WARNING" macros are     */
/* only meant as a temporary measure until the warning itself can    */
/* be properly investigated and resolved.                            */
/*-------------------------------------------------------------------*/

#include "ccfixme.h"      /* need HAVE_GCC_DIAG_PRAGMA, QPRAGMA, etc */

#ifndef _CCNOWARN_H_
#define _CCNOWARN_H_

  /*---------------------------------------------------------------*/
  /*                            MSVC                               */
  /*---------------------------------------------------------------*/

  #if defined( _MSVC_ )

    #define DISABLE_MSVC_WARNING( _num )    __pragma( warning( disable : _num ) )
    #define ENABLE_MSVC_WARNING( _num )     __pragma( warning( default : _num ) )

    #define PUSH_MSVC_WARNINGS()            __pragma( warning( push ))
    #define POP_MSVC_WARNINGS()             __pragma( warning( pop  ))

    /* Globally disable some uninteresting MSVC compiler warnings */

    DISABLE_MSVC_WARNING( 4127 ) // "conditional expression is constant"
    DISABLE_MSVC_WARNING( 4142 ) // "benign redefinition of type"
    DISABLE_MSVC_WARNING( 4146 ) // "unary minus operator applied to unsigned type, result still unsigned"
    DISABLE_MSVC_WARNING( 4200 ) // "nonstandard extension used : zero-sized array in struct/union"
    DISABLE_MSVC_WARNING( 4244 ) // (floating-point only?) "conversion from 'x' to 'y', possible loss of data"
    DISABLE_MSVC_WARNING( 4267 ) // "conversion from size_t to int possible loss of data"
    DISABLE_MSVC_WARNING( 4748 ) // "/GS can not protect parameters and local variables from local buffer overrun because optimizations are disabled in function"

  #endif /* defined( _MSVC_ ) */

  #ifndef   PUSH_MSVC_WARNINGS
    #define PUSH_MSVC_WARNINGS()            /* (do nothing) */
    #define POP_MSVC_WARNINGS()             /* (do nothing) */
    #define DISABLE_MSVC_WARNING( _str )    /* (do nothing) */
    #define ENABLE_MSVC_WARNING(  _str )    /* (do nothing) */
  #endif

  /*---------------------------------------------------------------*/
  /*                       GCC or CLANG                            */
  /*---------------------------------------------------------------*/

  #if defined( __GNUC__ ) || defined( _clang_ )
    #if defined( HAVE_GCC_DIAG_PRAGMA )

      #define DISABLE_GCC_WARNING( _str )   QPRAGMA( GCC diagnostic ignored _str )
      #define ENABLE_GCC_WARNING(  _str )   QPRAGMA( GCC diagnostic warning _str )

      #define PUSH_GCC_WARNINGS()           QPRAGMA( GCC diagnostic push )
      #define POP_GCC_WARNINGS()            QPRAGMA( GCC diagnostic pop  )

      /* Globally disable some rather annoying GCC compiler warnings which */
      /* frequently occurs due to our build multiple architectures design. */

      #if GCC_VERSION >= 40304
        /* 'xxxxxxxx' defined but not used */
        DISABLE_GCC_WARNING( "-Wunused-function" )
      #endif

      #if GCC_VERSION >= 40600
        /* variable 'xxx' set but not used */
        DISABLE_GCC_WARNING( "-Wunused-but-set-variable" )
      #endif

    #endif /* defined( HAVE_GCC_DIAG_PRAGMA ) */
  #endif /* defined( __GNUC__ ) || defined( _clang_ ) */

  #ifndef   DISABLE_GCC_WARNING
    #define DISABLE_GCC_WARNING( _str )     /* (do nothing) */
    #define ENABLE_GCC_WARNING(  _str )     /* (do nothing) */
    #define PUSH_GCC_WARNINGS()             /* (do nothing) */
    #define POP_GCC_WARNINGS()              /* (do nothing) */
  #endif

  /*---------------------------------------------------------------*/
  /*            define support for other compilers here            */
  /*---------------------------------------------------------------*/

  /* Don't forget to define all of the "FIXME" et al. macros too!  */

#endif /* _CCNOWARN_H_ */
