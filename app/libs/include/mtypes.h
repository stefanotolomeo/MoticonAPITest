/* mtypes.h
@start(__macros__)
@define(__codeprefix__)

The code begins in file @subst(__file__) at line @subst(__line__):
@enifed(__codeprefix__)

@define(Code)
@if(show_code)
@code
@fi(show_code)
@if(no_code)

.. Comment
@indent +4
@fi(no_code)
@enifed(Code)

@define(edoc)
@if(show_code)
@edoc
@fi(show_code)
@if(no_code)
@indent -4
@fi(no_code)
@enifed(edoc)

@end(__macros__)

@start()
.. highlight:: c
   :linenothreshold: 5

.. default-domain:: c

###########################
Type definitions (mtypes.h)
###########################

The following types are used to ensure the same datatype and datasizes across
different platforms (i386, x64, ...)

@include(type_definitions)

@*/

#ifndef __MTYPES_H__
#define __MTYPES_H__

#if !defined (__18CXX)
  #if defined (__GNUC__)
  #include <stdint.h>
  #else
  #include <pstdint.h>
  #endif
#endif


/*@rstart(type_definitions)

Data types
==========

The following datatypes are used.


.. type:: uchar_t

@code*/
typedef unsigned char uchar_t;    /*  8 Bit */
/*@edoc

.. type:: uint_t

@code*/
typedef unsigned int uint_t;
/*@edoc

.. type:: short_t

@code*/

typedef short short_t;            /* 16 bit signed */
/*@edoc

.. type:: ushort_t

@code*/
typedef unsigned short ushort_t;  /* 16 bit unsigned */
/*@edoc

.. type:: ulong_t

@code*/
#if defined (__C30) || defined (__18CXX)
typedef unsigned long ulong_t;    /* 32 Bit */
#else
typedef uint32_t ulong_t;    /* 32 Bit */
#endif


/*@edoc

.. type:: data_t

@code*/
typedef unsigned short data_t;    /* 16 bit unsigned */
/*@edoc

.. type:: sdata_t

@code*/
typedef short sdata_t;            /* 16 bit signed */
/*@edoc

.. type:: time_t
.. type:: uslong_t
.. type:: slong_t

@code*/

#if !defined __18CXX
  #include "time.h"
  typedef uint32_t uslong_t;    /* 32 Bit */
  typedef uint32_t slong_t;     /* 32 Bit */
#else
  #include <p18cxxx.h>
  typedef unsigned long time_t;           // 32 bit, sysclock is also ulong_t
  typedef unsigned short long uslong_t;   // 24 bit unsigned
  typedef short long slong_t;             // 24 bit signed
#endif

/*@edoc

.. type:: size32_t

@code*/
#if defined (__C30)
typedef uint32_t size32_t;    /* 32 Bit */
#elif defined (__18CXX)
typedef ulong_t size32_t;     /* 32 Bit */
#else
typedef unsigned int size32_t;
#endif
/*@edoc

.. type:: version_t

@code*/
 typedef size32_t version_t;
/*@(type_definitions)*/

#ifdef __cplusplus
#define _BEGIN_CPP extern "C" {
#define _END_CPP }
#else
#define _BEGIN_CPP
#define _END_CPP
#endif


#if defined(_MSC_VER) && ! defined(MSVC)
#define MSVC
#endif

#if defined(MSVC) && ! defined(TERMINAL)

#ifdef MSLAVE_EXPORTS
#define MSLAVE_API __declspec(dllexport)
#else
#define MSLAVE_API __declspec(dllimport)
#endif

#else

#define MSLAVE_API
#endif /* MSVC */

#endif /*__MTYPES_H__*/
