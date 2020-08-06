
# 1. Introduction

A **traceback list** is the list of error descriptions which give a clue about where in the code an error has happened. Such lists allow us diving into the problem usually towards its root cause.

Our implementation of traceback lists is not (nor pretends to be) perfect. Rather it should be taken as a source of inspiration to develop a more robust/custom error handling mechanism.


## 1.1. Use case: the error-code approach

Consider that we have a function `complex_operation()` which divides the sum of two numbers `a` and `b`, and divides the result by `c`. This function calls other two functions `sum()` and `divide()`. How would we deal with a possible division by zero?

A traditional approach is to use error codes. Assuming that we agree on using the return value of the functions to implement error codes, we could implement our application like in Listing 1.1.1.

```c
enum ERROR_CODES
{
    SUCCESS,
    DIVISION_BY_ZERO
};

enum ERROR_CODES sum(int a, int b, int * result)
{
    *result = a + b;
    return SUCCESS;
}

enum ERROR_CODES divide(int a, int b, int * result)
{
    if (b == 0)
        return DIVISION_BY_ZERO;
    *result = a / b;
    return SUCCESS;
}

enum ERROR_CODES complex_operation(int a, int b, int c, int * result)
{
    int partial_sum = 0;
    enum ERROR_CODES feedback;
    // Perform sum
    feedback = sum(a, b, &partial_result);
    if (feedback != SUCCESS)
        return feedback;
    // Perform division
    feedback = divide(partial_result, c, result);
    if(feedback != SUCCESS)
        return feedback;
    return SUCCESS;
}

```

**Listing 1.1.1.** Implementation of `complex_operation()` using error codes to detect divisions by zero.

As we can see, the implementation of `complex_operation()` becomes long as we add the code to detect possible errors.


## 1.2. Use case: our approach

Consider the same requirements that we had in our last example. However, now we will introduce a set of tools that will make the code easier to write, and easier to read. Moreover, it now provides us with the traceback list which would allow us finding the root cause of the error fastly.

We have written the new implementation in Listing 1.2.1. There, we use a set of definitions which you can find in "include/traceback.h". Notice that we have taken the function return statements away, since they are part of the mechanism that allow us develop the traceback list:

- `TB_FAIL_MACRO()` receives two parameters: the error which the current function should raise in that point of the code, and any error that may have caused it. When there are no causes that we wished to identify, we can just populate the second field with `TB_NO_ERROR_VALUE`.
- `TB_SUCCEED_MACRO()` exits the caller function without signaling any error.

Functions that call other functions that return `TB_RETURN_TYPE` may run the `TB_TEST_MACRO()`, which receives the following parameters:

- the call statement (e.g. `function_to_test(parameters)`),
- a `TB_RETURN_TYPE` variable to temporally store the feedback of the called function, and
- the error that should be appended to the traceback list if the called function returns an error.

In this example, if there is a division by zero in `divide()` while executing `complex_operation()`, then the resulting value of `feedback` contains the information related to both errors: `cannot_divide` (from `complex_operation()`)
, and `division_by_zero()`).

```c
#include "traceback.h"

TB_ERROR_TYPE cannot_add = {.divide: "An error happened while adding."};
TB_ERROR_TYPE division_by_zero = {.description: "I can't divide by zero!"};
TB_ERROR_TYPE cannot_divide = {.divide: "An error happened while dividing."};

TB_RETURN_TYPE add(int a, int b, int * result)
{
    *result = a + b;
    TB_SUCCEED_MACRO();
}

TB_RETURN_TYPE divide(int a, int b, int * result)
{
    if (b == 0)
        TB_FAIL_MACRO(division_by_zero, TB_NO_ERROR_VALUE);
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

```

**Listing 1.2.1.** A demonstration of how we approach error handling with the traceback list.


# 2. Building and running the example

To build the example code ("examples/traceback_test.c"), run the commands in Listing 2.1. After compiling, we should be able to run the application by executing the file in the "./bin" folder. The output of the program should look like the text in Lising 2.2.

```bash
$ cmake .
$ make
```

**Listing 2.1.** Commands to build the executable targets.

```bash
$ ./bin/traceback_test
Traceback:
    (0) ERROR (id:0x404080) - traceback_test.c:36 - Error B.
    (1) ERROR (id:0x404060) - traceback_test.c:25 - Error A.
```

**Listing 2.2.** Output of the example program.

To remove all generated files, run `make sclean`.
