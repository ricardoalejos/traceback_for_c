
#include <stdio.h>
#include <stdint.h>
#include "traceback.h"

TB_RETURN_TYPE function_a(void);
TB_RETURN_TYPE function_b(void);
void show_error(TB_ERROR_TYPE * error);

/* Define custom errors. */

TB_ERROR_TYPE error_a = {
    .description = "Error A."
};

TB_ERROR_TYPE error_b = {
    .description = "Error B."
};

/* 
    Function `function_a` fails with an `error_a` on purpose. 
*/
TB_RETURN_TYPE function_a(void)
{
    TB_FAIL_MACRO(error_a, TB_NO_ERROR_VALUE);
}

/* 
    Function `function_b` fails with an ̀ error_b` if the function `function_a`
    fails. In such a case, `function_b` appends its error data structure to
    whatever error that comes out from `function_a`.
*/
TB_RETURN_TYPE function_b(void)
{
    TB_RETURN_TYPE feedback;
    TB_TEST_MACRO(function_a(), feedback, error_b);
    TB_SUCCEED_MACRO();
}

/*
    Function `show_error` takes the top-level error object `error` and displays
    its details:
        - how deep it is in the feedback trace (0 is the top level),
        - an error identifier,
        - the error location (file and line), and
        - a textual description of the error.
    
    Then it iterates the error list by displaying the details of the previous
    error in the feedback trace. And the iterations stop when the function
    finds the root of the feedback trace (when the next error is
    `TB_NO_ERROR_VALUE`).
*/
void show_error(TB_ERROR_TYPE * error)
{
    TB_ERROR_TYPE * previous_error;
    uint32_t depth;

    previous_error = error;
    depth = 0;
    printf("Traceback:\n");
    while(previous_error != TB_NO_ERROR_VALUE)
    {
        printf(
            "    (%d) ERROR (id:%p) - %s:%d - %s\n",
            depth,
            (void *) previous_error,
            previous_error->file,
            previous_error->line,
            previous_error->description
        );

        previous_error = (TB_ERROR_TYPE *) previous_error->previous_error;
        depth++;
    }

    return;
}

int main(void)
{
    TB_ERROR_TYPE * feedback;

    feedback = function_b();

    show_error(feedback);

    return 0;
}
