#include <string.h>

#define TB_VERSION "0.1.1"

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

#define TB_SUCCEED_MACRO() return TB_SUCCESS_VALUE;

#define TB_FAIL_MACRO(error, pointer_to_previous)                           \
    error.file = TB_FILENAME;                                               \
    error.line = __LINE__;                                                  \
    error.previous_error = pointer_to_previous;                             \
    return &error;

#define TB_TEST_MACRO(function_call, feedback_variable, error)              \
    feedback_variable = function_call;                                      \
    if (feedback_variable)                                                  \
    {                                                                       \
        TB_FAIL_MACRO(error, feedback_variable);                            \
    }
