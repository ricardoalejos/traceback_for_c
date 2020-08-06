#include "traceback.h"
#include <stdio.h>

/* Error definitions */

TB_ERROR_TYPE cannot_add = {
    .description = "An error happened while adding."
};

TB_ERROR_TYPE division_by_zero = {
    .description = "I can't divide by zero!"
};

TB_ERROR_TYPE cannot_divide = {
    .description = "An error happened while dividing."
};

/* Function declarations */

TB_RETURN_TYPE add(int a, int b, int * result);
TB_RETURN_TYPE divide(int a, int b, int * result);
TB_RETURN_TYPE complex_operation(int a, int b, int c, int * result);
void show_error(TB_ERROR_TYPE * error);

/* Function definitions */

TB_RETURN_TYPE add(int a, int b, int * result)
{
    *result = a + b;
    TB_SUCCEED_MACRO();
}

TB_RETURN_TYPE divide(int a, int b, int * result)
{
    if (b == 0)
    {
        TB_FAIL_MACRO(division_by_zero, TB_NO_ERROR_VALUE);
    }
    *result = a / b;
    TB_SUCCEED_MACRO();
}

TB_RETURN_TYPE complex_operation(int a, int b, int c, int * result)
{
    int partial_addition = 0;
    TB_RETURN_TYPE feedback;
    // Perform addition
    TB_TEST_MACRO(add(a, b, &partial_addition), feedback, cannot_add);
    // Perform division
    TB_TEST_MACRO(divide(partial_addition, c, result), feedback, cannot_divide);
    TB_SUCCEED_MACRO();
}

void show_error(TB_ERROR_TYPE * error)
{
    TB_ERROR_TYPE * previous_error;
    unsigned int depth;

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

/* Application */

int main(void)
{
    int result;
    show_error(complex_operation(1, 2, 0, &result));
    return 0;
}
