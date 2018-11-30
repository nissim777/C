//
// Created by black_knight on 12/03/15.
// nissim levy

#include "Integer.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int NEGATIVE = -1, NATURAL = 0, POSITIVE =1;

IntegerP PointerFromInt(int intNum)
{
    IntegerP myInt = (IntegerP) malloc (sizeof(Integer));
    if (myInt == NULL)
        return NULL;
    myInt->num = intNum;
    return myInt;
}

void freeInteger(IntegerP integerNum)
{
    if (integerNum != NULL)
        free (integerNum);
}

IntegerP multInteger(ConstIntegerP num1, ConstIntegerP num2)
{
    IntegerP newInt = (IntegerP) malloc (sizeof(Integer));
    if (newInt == NULL)
        return NULL;
    newInt->num = num1->num * num2->num;
    return newInt;
}

static int compareInt(IntegerP num1, IntegerP num2) {
    if ((!num1) || (!num2)) {
        return NEGATIVE;
    }

    if (fabs(num1->num - num2->num) < EPSILON) {
        return NATURAL;
    }
    else if (num1->num > num2->num) {
        return POSITIVE;
    }

    return NEGATIVE;
}
int (*getIntCompareFunc())(IntegerP num1, IntegerP num2)
{
    return compareInt;
}