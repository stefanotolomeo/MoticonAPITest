/*
mmacros.h
*/

/* mresult.h
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

.. _mmacros:

##################
Macros (mmacros.h)
##################

This file defines some the often use macros MAX, MIN, COUNT
and a exception handling mechanism for c.


@include(Exception handling)
@include(convenience macros)
@include(version macros)
@*/

#ifndef __MMACRO_H__
#define __MMACRO_H__

#ifdef MDEBUG
#include <stdio.h>
#include "mtypes.h"

#if	!defined __18CXX
_BEGIN_CPP;
#endif
extern int __error_line;
int __check_assert(int check, const char* expr, const char* file, int line);
#if	!defined __18CXX
_END_CPP;
#endif

#define CHECK(x) do{error = (x); \
    if (! MR_SUCCEEDED(error)){__error_line = __LINE__; goto _Error;}\
    else{__error_line = -1;}}while(0)

#define WCHECK(x) do{error = (x); \
    if (MR_IS_ERROR(error)){__error_line = __LINE__; goto _Error;}\
    else{__error_line = -1;}}while(0)


#ifndef ASSERT
#define ASSERT(x) CHECK(__check_assert((x), #x, __FILE__, __LINE__))
#endif
#ifndef WASSERT
#define WASSERT(x) __check_assert((x), #x, __FILE__, __LINE__)
/*WASSERT == Weak ASSERT*/
#endif

#else /* MDEBUG */


/* @rstart(Exception handling)

******************
Exception Handling
******************

A simple example demonstarte the use of the exception handling mechanism.

  .. code-block:: c

     void foo() {
       int error; // must be defined

       CHECK(a_function_that_can_go_wrong());

       <other code>

       BEGIN_EXCEPT;
       <Some Error Handling>
       END_EXCEPT;
     }

.. macro:: CHECK(x)

   checks an expression for an error code (see :ref:`result-codes`).
   If the code is a warning or an error, the control flow jumps to
   :macro:`BEGIN_EXCEPT`.

   If debugging code is activated, the macro also records, the function
   and the source line of the exception.

   .. caution::

      To use this macro you have to define a variable ``int error``.

@code */

#define CHECK(x) do{ error = (x); if (! MR_SUCCEEDED(error)) { goto _Error; }} while(0)

/* @edoc

.. macro:: WCHECK(x)

   checks an expression for an error code (see :ref:`result-codes`).
   if the code is an error, the control flow jumps to
   :macro:`BEGIN_EXCEPT`.

   If debugging code is activated, the macro also records, the function
   and the source line of the exception.

   .. caution::

      To use this macro you have to define a variable ``int error``.


@code */

#define WCHECK(x) do{ error = (x); if (MR_IS_ERROR(error)) { goto _Error; }} while(0)

/* @edoc

@start(assert)
..
   .. code-block:: c */

#ifndef ASSERT
#define ASSERT(x)
#endif

#ifndef WASSERT
#define WASSERT(x)
#endif

#endif /* MDEBUG */

/* @(assert)

.. macro:: BEGIN_EXCEPT

   marks the beginning of an exception code area.

@code */

#define BEGIN_EXCEPT goto _GoOn; _Error:

/* @edoc
.. macro:: END_EXCEPT

   marks the end of an exception code area.

@code */

#define END_EXCEPT _GoOn:

/* @edoc
@(Exception handling)

@rstart(convenience macros)

******************
Convenience Macros
******************

.. macro:: MAX(x, y)

   returns the greater value of ``x`` and ``y``.

@code */

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

/* @edoc

.. macro:: MIN(x, y)

   returns the smaller value of ``x`` and ``y``.

@code */

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

/* @edoc

.. macro:: COUNT(seq)

   returns the element count of a sequence.

@code */

#ifndef COUNT
#define COUNT(seq) (sizeof((seq)) / sizeof(seq[0]))
#endif

/* @edoc

@rstart(version macros)

.. _version-macros:

**************
Version Macros
**************

.. macro:: MAKE_VERSION(main, sub, revision)

   returns a coded integer for program version identifiers.

@code c */

#ifndef MAKE_VERSION
#define MAKE_VERSION(main, sub, revision)\
  ((unsigned long)((((unsigned long)main) << 24) | (((unsigned long)sub) << 16) | ((revision) & 0xFFFF)))
#endif

/* @edoc

.. macro:: VERSION_GET_MAIN(version)

   returns the main version number of a version code.

@code */

#ifndef VERSION_GET_MAIN
#define VERSION_GET_MAIN(version) (((version) >> 24) & 0xFF)
#endif


/* @edoc

.. macro:: VERSION_GET_SUB(version)

   returns the inferior version number of a version code.

@code */

#ifndef VERSION_GET_SUB
#define VERSION_GET_SUB(version) (((version) >> 16) & 0xFF)
#endif


/* @edoc

.. macro:: VERSION_GET_REVISION(version)

   returns the revision number of a version code.

@code */

#ifndef VERSION_GET_REVISION
#define VERSION_GET_REVISION(version) ((version) & 0xFFFF)
#endif


/* @edoc

@rstart(plattform macros)

****************
Plattform Macros
****************

The following macros are used for plattform specific compilations

.. macro:: PACKED

   A workaround for microchip's C30 compiler, which doesn not
   support the ``#pragma pack`` directive.

   .. code-block:: c */



/*
C30 documentation:

Note: The device architecture requires that words be aligned on even byte
boundaries, so care must be taken when using the packed attribute to
avoid run-time addressing errors.

alignement should be a power of 2
...
aligned (alignment)
This attribute specifies a minimum alignment for the variable, measured in bytes. The
alignment must be a power of two.
...
*/
#if defined (__C30)
#define PACKED __attribute__((aligned(1), packed))
#else
#define PACKED
#endif


/*
.. Some objects (e.g. queue) have a different behaviour when operated
   in an terminal environment (simulated or real). TERMINAL is always defined
   if compiled on a pic processor. Or can be defined manually to simulate
   terminal behaviour.

   .. code-block:: c */

#if ! defined(TERMINAL)
#if defined(__C30)
#define TERMINAL
#endif
#endif  /* ! defined(TERMINAL) */

/*@*/

#endif /* ifndef __MMACRO_H__ */
