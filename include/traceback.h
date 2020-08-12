#ifndef __TRACEBACK_H__
#define __TRACEBACK_H__

#include <string.h>

#define TB_VERSION "0.1.2"

struct _ERROR_STRUCT;
typedef struct _ERROR_STRUCT TB_ERROR_TYPE;
typedef TB_ERROR_TYPE * TB_RETURN_TYPE;

struct _ERROR_STRUCT {
    char * description;
    char * file;
    unsigned int line;
    TB_ERROR_TYPE * previous_error;
};

#define TB_SUCCESS_VALUE ((TB_ERROR_TYPE *) 0)
#define TB_NO_ERROR_VALUE ((TB_ERROR_TYPE *) 0)
#define TB_FILENAME                                                         \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/* Macro definitions */

/*
    TB_SUCCEED_MACRO()

    Makes the caller function return a `TB_SUCCESS` value.
*/
#define TB_SUCCEED_MACRO() return TB_SUCCESS_VALUE;

/*
    TB_FAIL_MACRO()

    Makes the caller function return a `TB_RETURN_TYPE` structure, and
    populates such structure with debug information: the file and line
    where this macro is invoked.
*/
#define TB_FAIL_MACRO(err_ptr)                                              \
    (err_ptr)->file = TB_FILENAME;                                          \
    (err_ptr)->line = __LINE__;                                             \
    (err_ptr)->previous_error = TB_NO_ERROR_VALUE;                          \
    return (err_ptr);

/*
    TB_FAIL_CUMULATIVE_MACRO()
    
    Makes the caller function return a `TB_RETURN_TYPE` structure (`err_ptr`),
    and links such structure to another one (`cause_ptr`) to form a traceback 
    list.
*/
#define TB_FAIL_CUMULATIVE_MACRO(err_ptr, cause_ptr)                        \
    (err_ptr)->file = TB_FILENAME;                                          \
    (err_ptr)->line = __LINE__;                                             \
    (err_ptr)->previous_error = cause_ptr;                                  \
    return (err_ptr);

/*
    TB_TEST_CUMULATIVE_MACRO()

    In the following example, the function `caller_function` would return
    `&FRAGILE_FUNCTION_FAILED_ERROR` if the function `fragile_function` returns
    a different value than `TB_SUCCESS`. Additionally, the macro appends the
    error information returned by `fragile_function` to the 
    `&FRAGILE_FUNCTION_FAILED_ERROR`, forming a linked list.

    ```c
    TB_ERROR_TYPE FRAGILE_FUNCTION_FAILED_ERROR = {
        .description = "Failed when attempting to run fragile_function."
    };

    TB_RETURN_TYPE caller_function()
    {
        TB_RETURN_TYPE feedback_variable;

        TB_TEST_MACRO(
            fragile_function(),
            feedback_variable,
            &FRAGILE_FUNCTION_FAILED
        );

        TB_SUCCEED();
    }
    ```
*/
#define TB_TEST_CUMULATIVE_MACRO(fn_call, fb_var, err_ptr)                  \
    fb_var = fn_call;                                                       \
    if (fb_var)                                                             \
    {                                                                       \
        TB_FAIL_CUMULATIVE_MACRO(err_ptr, fb_var);                          \
    }

/*
    # TB_TEST_MACRO()

    In the following example, the function `caller_function` returns TB_SUCCESS
    if the function `fragile_function` runs nominally. If `fragile_function`
    fails, then `caller_function` returns the same `TB_RETURN_TYPE` value that
    `fragile_function` returns.

    ```c

    TB_RETURN_TYPE caller_function()
    {
        TB_RETURN_TYPE feedback_variable;

        TB_TEST_MACRO(
            fragile_function(),
            feedback_variable
        );

        TB_SUCCEED();
    }
    ```
*/
#define TB_TEST_MACRO(function_call, feedback_variable)                     \
    feedback_variable = function_call;                                      \
    if (feedback_variable)                                                  \
    {                                                                       \
        TB_FAIL_MACRO(feedback_variable);                                   \
    }


#endif
