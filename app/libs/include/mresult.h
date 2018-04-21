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

########################
Result Codes (mresult.h)
########################

Almost all functions of the moticon API returns a standardized result code
of type ``int``. Based on the code's number you can obtain several
informations. All codes are defined in the header file mresult.h.

@if(show_code)
@include(Result Types)
@fi(show_code)
@include(Test Macros)
@if(show_code)
@include(modules)
@include(Result Code Generation)
@fi(show_code)
@include(Result Codes)

@*/

#ifndef __MRESULT_H__
#define __MRESULT_H__


/* @rstart(Result Types)

************
Result Types
************

Result types are encoded in the result codes and informs
if the code an error, a warning or a success code.
The result types are usually used with the :ref:`test-macros`.

@code */
#define MR_T_ERROR         0x8000
#define MR_T_WARNING       0x4000
#define MR_T_NO_SUCCESS    (MR_T_ERROR|MR_T_WARNING)
#define MR_T_SUCCESS       0x0000


/* @rstart(Test Macros)

.. _test-macros:

***********
Test Macros
***********

There are several macros to extract the result type from
the result codes:

@include(MR_IS_ERROR)
@include(MR_IS_WARNING)
@include(MR_SUCCEEDED)

@*/


/* @rstart(MR_IS_ERROR)

.. macro:: MR_IS_ERROR(code)

   Returns true if the code is an error code.

   @subst(Code) */

#define MR_IS_ERROR(code) ((code) & MR_T_ERROR)

/* @rstart(MR_IS_WARNING)

.. macro:: MR_IS_WARNING(code)

   Returns true if the code is a warning code.

   @subst(Code) */

#define MR_IS_WARNING(code) ((code) & MR_T_WARNING)

/* @rstart(MR_SUCCEEDED)

.. macro:: MR_SUCCEEDED(code)

   Returns true if the code is a success code.

   @subst(Code) */

#define MR_SUCCEEDED(code) (((code) & MR_T_NO_SUCCESS) == 0)



/* @rstart(modules)

*******
Modules
*******

Some result codes are specific to a module:

.. macro:: MR_MODULE_ALL

   For codes that can be used for all modules.

   @code */
#define MR_MODULE_ALL   0

/*@edoc

.. macro:: MR_MODULE_ANT

   For codes specific to ANT

   @code */
#define MR_MODULE_ANT   1

/* @edoc

.. macro:: MR_MODULE(x)

   returns the module id of the code.

   @code */
#define MR_MODULE(code)     (((code) >> 8) & 0x3F)



/* @rstart(Result Code Generation)

**********************
Result Code Generation
**********************

Some internal macros to generate result codes.

@code */

#define MR_MOD(module)  (((module) & 0x3F) << 8)
#define MR_CODE(code) ((code) & 0xFF)
#define MR_MAKE_CODE(type, mod, code) ((type) | MR_MOD(mod) | MR_CODE(code))


/* @rstart(Result Codes)

.. _result-codes:

************
Result Codes
************

.. macro:: MR_SUCCESS

   The function succeeded.

   @subst(Code)*/

#define MR_SUCCESS              MR_MAKE_CODE(MR_T_SUCCESS, MR_MODULE_ALL, 0)


/*@subst(edoc)

.. macro:: MR_NO_DATA

   There is no error, but no data is available.

   @subst(Code)*/

#define MR_NO_DATA              MR_MAKE_CODE(MR_T_WARNING, MR_MODULE_ALL, 1)

/*@subst(edoc)

.. macro:: MR_RECOVERING

   The system is recovering, the function cannot be executed.

   @subst(Code)*/

#define MR_RECOVERING           MR_MAKE_CODE(MR_T_WARNING, MR_MODULE_ALL, 2)

/*@subst(edoc)

.. macro:: MR_FAILED

   The function failed executing.

   @subst(Code)*/

#define MR_FAILED               MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 1)

/*@subst(edoc)

.. macro:: MR_OUT_OF_MEMORY

   There is not enough memory to continue.

   @subst(Code)*/

#define MR_OUT_OF_MEMORY        MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 2)

/*@subst(edoc)

.. macro:: MR_FATAL_ERROR

   A fatal error occured. The system has to be reset.

   @subst(Code)*/

#define MR_FATAL_ERROR          MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 3)

/*@subst(edoc)

.. macro:: MR_WRONG_STATE

   You may not call the function, in the current state.

   @subst(Code)*/

#define MR_WRONG_STATE          MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 4)

/*@subst(edoc)

.. macro:: MR_QUEUE_OVERFLOW

   A queue overflow occured, while executing.

   @subst(Code)*/

#define MR_QUEUE_OVERFLOW       MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 5)

/*@subst(edoc)

.. macro:: MR_INVALID_ARG

   A function argument value is invalid.

   @subst(Code)*/

#define MR_INVALID_ARG          MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 6)

/*@subst(edoc)

.. macro:: MR_TIMEOUT

   A function timed out.

   @subst(Code)*/

#define MR_TIMEOUT              MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 7)

/*@subst(edoc)

.. macro:: MR_ABORTED

   A function was aborted by a signal like Control-c.

   @subst(Code)*/

#define MR_ABORTED              MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 8)


/*@subst(edoc)

.. macro:: MR_ASSERT

   An :macro:`ASSERT` was raised.

   @subst(Code)*/

#define MR_ASSERT              MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 9)

/*@subst(edoc)

.. macro:: MR_CHECKSUM_BAD

   A bad checksum in communication function.

   @subst(Code)*/

#define MR_CHECKSUM_BAD       MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 10)

/*@subst(edoc)

.. macro:: MR_NO_LICENSE

   No valid licence available.

   @subst(Code)*/

#define MR_NO_LICENSE         MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 11)


/*@subst(edoc)

.. macro:: MR_NOT_SUPPORTED

   The functionality is not supported.

   @subst(Code)*/

#define MR_NOT_SUPPORTED      MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 12)


/*@subst(edoc)

.. macro:: MR_RECORDING

   The ant device is recording.

   @subst(Code)*/

#define MR_RECORDING        MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ALL, 13)


/*@subst(edoc)

.. macro:: MR_ANT_NO_DEVICE

   The ant device could not be opened.

   @subst(Code)*/

#define MR_ANT_NO_DEVICE        MR_MAKE_CODE(MR_T_ERROR, MR_MODULE_ANT, 0)




/*@*/

#endif /* __MRESULT_H__ */
